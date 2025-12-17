#include "atm90e36.h"
#include <cinttypes>
#include <cmath>
#include <numbers>
#include "esphome/core/log.h"

namespace esphome {
namespace atm90e36 {

static const char *const TAG = "atm90e36";
//Loop called by normal ESP32 Loop()
void ATM90E36Component::loop() {
  if (this->get_publish_interval_flag_()) {
    this->set_publish_interval_flag_(false);
    for (uint8_t phase = 0; phase < 3; phase++) {
      if (this->phase_[phase].voltage_sensor_ != nullptr)
        this->phase_[phase].voltage_ = this->get_phase_voltage_(phase);

      if (this->phase_[phase].current_sensor_ != nullptr)
        this->phase_[phase].current_ = this->get_phase_current_(phase);

      if (this->phase_[phase].power_sensor_ != nullptr)
        this->phase_[phase].active_power_ = this->get_phase_active_power_(phase);

      if (this->phase_[phase].power_factor_sensor_ != nullptr)
        this->phase_[phase].power_factor_ = this->get_phase_power_factor_(phase);

      if (this->phase_[phase].reactive_power_sensor_ != nullptr)
        this->phase_[phase].reactive_power_ = this->get_phase_reactive_power_(phase);

      if (this->phase_[phase].apparent_power_sensor_ != nullptr)
        this->phase_[phase].apparent_power_ = this->get_phase_apparent_power_(phase);

      if (this->phase_[phase].forward_active_energy_sensor_ != nullptr)
        this->phase_[phase].forward_active_energy_ = this->get_phase_forward_active_energy_(phase);

      if (this->phase_[phase].reverse_active_energy_sensor_ != nullptr)
        this->phase_[phase].reverse_active_energy_ = this->get_phase_reverse_active_energy_(phase);

      if (this->phase_[phase].phase_angle_sensor_ != nullptr)
        this->phase_[phase].phase_angle_ = this->get_phase_angle_(phase);

      if (this->phase_[phase].harmonic_active_power_sensor_ != nullptr)
        this->phase_[phase].harmonic_active_power_ = this->get_phase_harmonic_active_power_(phase);

      if (this->phase_[phase].peak_current_sensor_ != nullptr)
        this->phase_[phase].peak_current_ = this->get_phase_peak_current_(phase);

      // After the local store is collected we can publish them trusting they are within +-1 hardware sampling
      if (this->phase_[phase].voltage_sensor_ != nullptr)
        this->phase_[phase].voltage_sensor_->publish_state(this->get_local_phase_voltage_(phase));

      if (this->phase_[phase].current_sensor_ != nullptr)
        this->phase_[phase].current_sensor_->publish_state(this->get_local_phase_current_(phase));

      if (this->phase_[phase].power_sensor_ != nullptr)
        this->phase_[phase].power_sensor_->publish_state(this->get_local_phase_active_power_(phase));

      if (this->phase_[phase].power_factor_sensor_ != nullptr)
        this->phase_[phase].power_factor_sensor_->publish_state(this->get_local_phase_power_factor_(phase));

      if (this->phase_[phase].reactive_power_sensor_ != nullptr)
        this->phase_[phase].reactive_power_sensor_->publish_state(this->get_local_phase_reactive_power_(phase));

      if (this->phase_[phase].apparent_power_sensor_ != nullptr)
        this->phase_[phase].apparent_power_sensor_->publish_state(this->get_local_phase_apparent_power_(phase));

      if (this->phase_[phase].forward_active_energy_sensor_ != nullptr) {
        this->phase_[phase].forward_active_energy_sensor_->publish_state(
            this->get_local_phase_forward_active_energy_(phase));
      }

      if (this->phase_[phase].reverse_active_energy_sensor_ != nullptr) {
        this->phase_[phase].reverse_active_energy_sensor_->publish_state(
            this->get_local_phase_reverse_active_energy_(phase));
      }

      if (this->phase_[phase].phase_angle_sensor_ != nullptr)
        this->phase_[phase].phase_angle_sensor_->publish_state(this->get_local_phase_angle_(phase));

      if (this->phase_[phase].harmonic_active_power_sensor_ != nullptr) {
        this->phase_[phase].harmonic_active_power_sensor_->publish_state(
            this->get_local_phase_harmonic_active_power_(phase));
      }

      if (this->phase_[phase].peak_current_sensor_ != nullptr)
        this->phase_[phase].peak_current_sensor_->publish_state(this->get_local_phase_peak_current_(phase));
    }
    if (this->freq_sensor_ != nullptr)
      this->freq_sensor_->publish_state(this->get_frequency_());

    if (this->chip_temperature_sensor_ != nullptr)
      this->chip_temperature_sensor_->publish_state(this->get_chip_temperature_());
  }
}

void ATM90E36Component::update() {
  this->set_publish_interval_flag_(true);
  this->status_clear_warning();

#ifdef USE_TEXT_SENSOR
  this->check_phase_status();
  this->check_freq_status();
#endif
}

//Setup called by ESP32 setup()
void ATM90E36Component::setup() {
	this->spi_setup();
	this->cs_summary_ = this->cs_->dump_summary();
	const char *cs = this->cs_summary_.c_str();

	uint16_t mmode0 = 0x87;  // 3P4W 50Hz
	uint16_t high_thresh = 0;
	uint16_t low_thresh = 0;
	

	if (line_freq_ == 60) {
		mmode0 |= 1 << 12;  // sets 12th bit to 1, 60Hz
	// for freq threshold registers
		high_thresh = 6300;  // 63.00 Hz
		low_thresh = 5700;   // 57.00 Hz
	} else {
		high_thresh = 5300;  // 53.00 Hz
		low_thresh = 4700;   // 47.00 Hz
	}

	if (current_phases_ == 2) {
		mmode0 |= 1 << 8;  // sets 8th bit to 1, 3P3W
		mmode0 |= 0 << 1;  // sets 1st bit to 0, phase b is not counted into the all-phase sum energy/power (P/Q/S)
	}
	
	//Calulate MMMode1 taking current_gain gain_dpga and gain_voltage
	pga_cal = pga_current_;
	pga_cal |= pga_voltage_ << 8;
	pga_cal |= dpga_gain_ << 14;			

	this->write16_(ATM90E36_REGISTER_SOFTRESET, 0x789A, false);  // Perform soft reset
	delay(6);                                                    // Wait for the minimum 5ms + 1ms
	this->write16_(ATM90E36_REGISTER_FUNCEN0, 0x0000); // Voltage sag
	this->write16_(ATM90E36_REGISTER_FUNCEN1, 0x0000); // Voltage sag
	this->write16_(ATM90E36_REGISTER_SAGTH, calculate_voltage_threshold(50.0, this->phase_[0].voltage_gain_, 1)); // Voltage sag
	this->write16_(ATM90E36_REGISTER_CONFIGSTART, 0x5678);		// Start Config of chip
	this->write16_(ATM90E36_REGISTER_PLCONSTH, 0x0861);       	// PL Constant MSB (default) = 140625000
	this->write16_(ATM90E36_REGISTER_PLCONSTL, 0xC468);       	// PL Constant LSB (default)
	this->write16_(ATM90E36_REGISTER_MMODE0, mmode0);         	// Mode Config (frequency set in main program) mmode0
	this->write16_(ATM90E36_REGISTER_MMODE1, pga_cal);      	// PGA Gain Configuration for Current Channels 0x5555
	this->write16_(ATM90E36_REGISTER_PSTARTTH, 0x0000);       	// All Active Startup Power Threshold - 0.02A/0.00032 = 7500
	this->write16_(ATM90E36_REGISTER_QSTARTTH, 0x0000);       	// All Reactive Startup Power Threshold - 50%
	this->write16_(ATM90E36_REGISTER_SSTARTTH, 0x0000);       	// All Reactive Startup Power Threshold - 50%
	this->write16_(ATM90E36_REGISTER_PPHASETH, 0x0000);       	// Each Phase Active Phase Threshold - 0.002A/0.00032 = 750
	this->write16_(ATM90E36_REGISTER_QPHASETH, 0x0000);       	// Each phase Reactive Phase Threshold - 10%
	this->write16_(ATM90E36_REGISTER_SPHASETH, 0x0000);    		// Apparent  Phase Threshold
	this->write16_(ATM90E36_REGISTER_CSO, 0x4741);      		// Checksum 0

	this->write16_(ATM90E36_REGISTER_CALSTART, 0x5678);   		// Metering calibration startup
	this->write16_(ATM90E36_REGISTER_PQGAINA, 0x0000); 			// CT1 line active power offset
	this->write16_(ATM90E36_REGISTER_PHIA, 0x0000); 			// CT1 line reactive power offset
	this->write16_(ATM90E36_REGISTER_PQGAINB, 0x0000); 			// CT2 line active power offset
	this->write16_(ATM90E36_REGISTER_PHIB, 0x0000); 			// CT2 line reactive power offset
	this->write16_(ATM90E36_REGISTER_PQGAINC, 0x0000); 			// CT3 line active power offset
	this->write16_(ATM90E36_REGISTER_PHIC, 0x0000); 			// CT3 line reactive power offset
	this->write16_(ATM90E36_REGISTER_POFFSETA, 0x0000);    		// Line calibration gain
	this->write16_(ATM90E36_REGISTER_QOFFSETA, 0x0000);     	// Line calibration angle
	this->write16_(ATM90E36_REGISTER_POFFSETB, 0x0000);    		// Line calibration gain
	this->write16_(ATM90E36_REGISTER_QOFFSETB, 0x0000);     	// Line calibration angle
	this->write16_(ATM90E36_REGISTER_POFFSETC, 0x0000);    		// Line calibration gain
	this->write16_(ATM90E36_REGISTER_QOFFSETC, 0x0000);    		// Line calibration angle
	this->write16_(ATM90E36_REGISTER_CS1, 0x0000);      		// Checksum 1

	// **************** FUNDAMENTAL/HARMONIC & ENERGY CALIBRATION REGISTERS ****************
	this->write16_(ATM90E36_REGISTER_HARMSTART, 0x5678);   		// Metering calibration startup
	this->write16_(ATM90E36_REGISTER_POFFSETAF, 0x0000); 		// CT1 Fund. active power offset
	this->write16_(ATM90E36_REGISTER_POFFSETBF, 0x0000); 		// CT2 Fund. active power offset
	this->write16_(ATM90E36_REGISTER_POFFSETCF, 0x0000); 		// CT3 Fund. active power offset
	this->write16_(ATM90E36_REGISTER_PGAINAF, 0x0000);   		// CT1 Fund. active power gain
	this->write16_(ATM90E36_REGISTER_PGAINBF, 0x0000);   		// CT2 Fund. active power gain
	this->write16_(ATM90E36_REGISTER_PGAINCF, 0x0000);   		// CT3 Fund. active power gain
	this->write16_(ATM90E36_REGISTER_CS2, 0x0000);       		// Checksum 2

	// **************** MEASUREMENT CALIBRATION REGISTERS ****************
	this->write16_(ATM90E36_REGISTER_ADJSTART, 0x5678); // Measurement calibration
	
	this->write16_(ATM90E36_REGISTER_UGAINA, this->phase_[0].voltage_gain_); 	// CT1 SVoltage RMS gain 
	this->write16_(ATM90E36_REGISTER_IGAINA, this->phase_[0].ct_gain_); 		// CT1 line current gain. 
	this->write16_(ATM90E36_REGISTER_UOFFSETA, 0x0000);  						// CT1 Voltage offset
	this->write16_(ATM90E36_REGISTER_IOFFSETA, 0x0000);  						// CT1 line current offset
	
	this->write16_(ATM90E36_REGISTER_UGAINB, this->phase_[1].voltage_gain_); 	// CT2 SVoltage RMS gain 
	this->write16_(ATM90E36_REGISTER_IGAINB, this->phase_[1].ct_gain_); 		// CT2 line current gain. 
	this->write16_(ATM90E36_REGISTER_UOFFSETB, 0x0000);  						// CT2 Voltage offset
	this->write16_(ATM90E36_REGISTER_IOFFSETB, 0x0000);  						// CT2 line current offset
	
	this->write16_(ATM90E36_REGISTER_UGAINC, this->phase_[2].voltage_gain_); 	// CT3 SVoltage RMS gain 
	this->write16_(ATM90E36_REGISTER_IGAINC, this->phase_[2].ct_gain_); 		// CT3 line current gain. 
	this->write16_(ATM90E36_REGISTER_UOFFSETA, 0x0000);  						// CT3 Voltage offset
	this->write16_(ATM90E36_REGISTER_IOFFSETA, 0x0000);  						// CT3 line current offset
	// N
	this->write16_(ATM90E36_REGISTER_IGAINN, 0xFD7F); 							// D line current gain

	this->write16_(ATM90E36_REGISTER_CS3, 0x02F6); 								// Checksum 3
}

void ATM90E36Component::dump_config() {
  ESP_LOGCONFIG("", "ATM90E36:");
  LOG_PIN("  CS Pin: ", this->cs_);
  if (this->is_failed()) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Voltage A", this->phase_[PHASEA].voltage_sensor_);
  LOG_SENSOR("  ", "Current A", this->phase_[PHASEA].current_sensor_);
  LOG_SENSOR("  ", "Power A", this->phase_[PHASEA].power_sensor_);
  LOG_SENSOR("  ", "Reactive Power A", this->phase_[PHASEA].reactive_power_sensor_);
  LOG_SENSOR("  ", "Apparent Power A", this->phase_[PHASEA].apparent_power_sensor_);
  LOG_SENSOR("  ", "PF A", this->phase_[PHASEA].power_factor_sensor_);
  LOG_SENSOR("  ", "Active Forward Energy A", this->phase_[PHASEA].forward_active_energy_sensor_);
  LOG_SENSOR("  ", "Active Reverse Energy A", this->phase_[PHASEA].reverse_active_energy_sensor_);
  LOG_SENSOR("  ", "Harmonic Power A", this->phase_[PHASEA].harmonic_active_power_sensor_);
  LOG_SENSOR("  ", "Phase Angle A", this->phase_[PHASEA].phase_angle_sensor_);
  LOG_SENSOR("  ", "Peak Current A", this->phase_[PHASEA].peak_current_sensor_);
  LOG_SENSOR("  ", "Voltage B", this->phase_[PHASEB].voltage_sensor_);
  LOG_SENSOR("  ", "Current B", this->phase_[PHASEB].current_sensor_);
  LOG_SENSOR("  ", "Power B", this->phase_[PHASEB].power_sensor_);
  LOG_SENSOR("  ", "Reactive Power B", this->phase_[PHASEB].reactive_power_sensor_);
  LOG_SENSOR("  ", "Apparent Power B", this->phase_[PHASEB].apparent_power_sensor_);
  LOG_SENSOR("  ", "PF B", this->phase_[PHASEB].power_factor_sensor_);
  LOG_SENSOR("  ", "Active Forward Energy B", this->phase_[PHASEB].forward_active_energy_sensor_);
  LOG_SENSOR("  ", "Active Reverse Energy B", this->phase_[PHASEB].reverse_active_energy_sensor_);
  LOG_SENSOR("  ", "Harmonic Power B", this->phase_[PHASEB].harmonic_active_power_sensor_);
  LOG_SENSOR("  ", "Phase Angle B", this->phase_[PHASEB].phase_angle_sensor_);
  LOG_SENSOR("  ", "Peak Current B", this->phase_[PHASEB].peak_current_sensor_);
  LOG_SENSOR("  ", "Voltage C", this->phase_[PHASEC].voltage_sensor_);
  LOG_SENSOR("  ", "Current C", this->phase_[PHASEC].current_sensor_);
  LOG_SENSOR("  ", "Power C", this->phase_[PHASEC].power_sensor_);
  LOG_SENSOR("  ", "Reactive Power C", this->phase_[PHASEC].reactive_power_sensor_);
  LOG_SENSOR("  ", "Apparent Power C", this->phase_[PHASEC].apparent_power_sensor_);
  LOG_SENSOR("  ", "PF C", this->phase_[PHASEC].power_factor_sensor_);
  LOG_SENSOR("  ", "Active Forward Energy C", this->phase_[PHASEC].forward_active_energy_sensor_);
  LOG_SENSOR("  ", "Active Reverse Energy C", this->phase_[PHASEC].reverse_active_energy_sensor_);
  LOG_SENSOR("  ", "Harmonic Power C", this->phase_[PHASEC].harmonic_active_power_sensor_);
  LOG_SENSOR("  ", "Phase Angle C", this->phase_[PHASEC].phase_angle_sensor_);
  LOG_SENSOR("  ", "Peak Current C", this->phase_[PHASEC].peak_current_sensor_);
  LOG_SENSOR("  ", "Frequency", this->freq_sensor_);
  LOG_SENSOR("  ", "Chip Temp", this->chip_temperature_sensor_);
}

float ATM90E36Component::get_setup_priority() const { return setup_priority::IO; }

// R/C registers can conly be cleared after the LastSPIData register is updated (register 78H)
// Peakdetect period: 05H. Bit 15:8 are PeakDet_period in ms. 7:0 are Sag_period
// Default is 143FH (20ms, 63ms)
uint16_t ATM90E36Component::read16_(uint16_t a_register) {
  this->enable();
  delay_microseconds_safe(10);  // min delay between CS low and first SCK is 200ns - 1us is plenty
  uint8_t addrh = (1 << 7) | ((a_register >> 8) & 0x03);
  uint8_t addrl = (a_register & 0xFF);
  uint8_t data[4] = {addrh, addrl, 0x00, 0x00};
  this->transfer_array(data, 4);
  uint16_t output = encode_uint16(data[2], data[3]);
  ESP_LOGVV(TAG, "read16_ 0x%04" PRIX16 " output 0x%04" PRIX16, a_register, output);
  delay_microseconds_safe(10);  // allow the last clock to propagate before releasing CS
  this->disable();
  delay_microseconds_safe(1);  // meet minimum CS high time before next transaction
  return output;
}

void ATM90E36Component::write16_(uint16_t a_register, uint16_t val, bool validate) {
  ESP_LOGVV(TAG, "write16_ 0x%04" PRIX16 " val 0x%04" PRIX16, a_register, val);
  uint8_t addrh = ((a_register >> 8) & 0x03);
  uint8_t addrl = (a_register & 0xFF);
  uint8_t data[4] = {addrh, addrl, uint8_t((val >> 8) & 0xFF), uint8_t(val & 0xFF)};
  this->enable();
  delay_microseconds_safe(1);  // ensure CS setup time
  this->write_array(data, 4);
  delay_microseconds_safe(1);  // allow clock to settle before raising CS
  this->disable();
  delay_microseconds_safe(1);  // ensure minimum CS high time
  if (validate)
    this->validate_spi_read_(val, "write16()");
}

float ATM90E36Component::get_local_phase_voltage_(uint8_t phase) { return this->phase_[phase].voltage_; }

float ATM90E36Component::get_local_phase_current_(uint8_t phase) { return this->phase_[phase].current_; }

float ATM90E36Component::get_local_phase_active_power_(uint8_t phase) { return this->phase_[phase].active_power_; }

float ATM90E36Component::get_local_phase_reactive_power_(uint8_t phase) { return this->phase_[phase].reactive_power_; }

float ATM90E36Component::get_local_phase_apparent_power_(uint8_t phase) { return this->phase_[phase].apparent_power_; }

float ATM90E36Component::get_local_phase_power_factor_(uint8_t phase) { return this->phase_[phase].power_factor_; }

float ATM90E36Component::get_local_phase_forward_active_energy_(uint8_t phase) {
  return this->phase_[phase].forward_active_energy_;
}

float ATM90E36Component::get_local_phase_reverse_active_energy_(uint8_t phase) {
  return this->phase_[phase].reverse_active_energy_;
}

float ATM90E36Component::get_local_phase_angle_(uint8_t phase) { return this->phase_[phase].phase_angle_; }

float ATM90E36Component::get_local_phase_harmonic_active_power_(uint8_t phase) {
  return this->phase_[phase].harmonic_active_power_;
}

float ATM90E36Component::get_local_phase_peak_current_(uint8_t phase) { return this->phase_[phase].peak_current_; }

float ATM90E36Component::get_phase_voltage_(uint8_t phase) {
  uint16_t voltage = this->read16_(ATM90E36_REGISTER_URMS + phase);
  this->validate_spi_read_(voltage, "get_phase_voltage()");
  
  if (voltage < 50) {
	  voltage = 0;
  }
  
  return (float) voltage / 100;
}

float ATM90E36Component::get_phase_voltage_avg_(uint8_t phase) {
  const uint8_t reads = 10;
  uint32_t accumulation = 0;
  uint16_t voltage = 0;
  for (uint8_t i = 0; i < reads; i++) {
    voltage = this->read16_(ATM90E36_REGISTER_URMS + phase);
    this->validate_spi_read_(voltage, "get_phase_voltage_avg_()");
    accumulation += voltage;
  }
  voltage = accumulation / reads;
  this->phase_[phase].voltage_ = (float) voltage / 100;
  return this->phase_[phase].voltage_;
}

float ATM90E36Component::get_phase_current_avg_(uint8_t phase) {
  const uint8_t reads = 10;
  uint32_t accumulation = 0;
  uint16_t current = 0;
  for (uint8_t i = 0; i < reads; i++) {
    current = this->read16_(ATM90E36_REGISTER_IRMS + phase);
    this->validate_spi_read_(current, "get_phase_current_avg_()");
    accumulation += current;
  }
  current = accumulation / reads;
  this->phase_[phase].current_ = (float) current / 1000;
  return this->phase_[phase].current_;
}

float ATM90E36Component::get_phase_current_(uint8_t phase) {
  const uint16_t current = this->read16_(ATM90E36_REGISTER_IRMS + phase);
  this->validate_spi_read_(current, "get_phase_current_()");
  return (float) current / 1000;
}

float ATM90E36Component::get_phase_active_power_(uint8_t phase) {
  float val = (float)this->read16_(ATM90E36_REGISTER_PMEAN + phase);
  
  if (val >= 0xFFFF) {
	  val = 0;
  }
  
  return val;
}

float ATM90E36Component::get_phase_reactive_power_(uint8_t phase) {
	int16_t val = (int16_t)this->read16_(ATM90E36_REGISTER_QMEAN + phase);
	return (float)val / 1000;
}

float ATM90E36Component::get_phase_apparent_power_(uint8_t phase) {
  const int val = this->read16_(ATM90E36_REGISTER_SMEANT + phase);
  return val;
}

float ATM90E36Component::get_phase_power_factor_(uint8_t phase) {
  uint16_t powerfactor = this->read16_(ATM90E36_REGISTER_PFMEAN + phase);  // unsigned to compare to lastspidata
  this->validate_spi_read_(powerfactor, "get_phase_power_factor_()");
  return (float) ((int16_t) powerfactor) / 1000;  // make it signed again
}

float ATM90E36Component::get_phase_forward_active_energy_(uint8_t phase) {
  const uint16_t val = this->read16_(ATM90E36_REGISTER_APENERGY + phase);
  if ((UINT32_MAX - this->phase_[phase].cumulative_forward_active_energy_) > val) {
    this->phase_[phase].cumulative_forward_active_energy_ += val;
  } else {
    this->phase_[phase].cumulative_forward_active_energy_ = val;
  }
  // 0.01CF resolution = 0.003125 Wh per count
  return ((float) this->phase_[phase].cumulative_forward_active_energy_ * (10.0f / 3200.0f));
}

float ATM90E36Component::get_phase_reverse_active_energy_(uint8_t phase) {
  const uint16_t val = this->read16_(ATM90E36_REGISTER_ANENERGY + phase);
  if (UINT32_MAX - this->phase_[phase].cumulative_reverse_active_energy_ > val) {
    this->phase_[phase].cumulative_reverse_active_energy_ += val;
  } else {
    this->phase_[phase].cumulative_reverse_active_energy_ = val;
  }
  // 0.01CF resolution = 0.003125 Wh per count
  return ((float) this->phase_[phase].cumulative_reverse_active_energy_ * (10.0f / 3200.0f));
}

float ATM90E36Component::get_phase_harmonic_active_power_(uint8_t phase) {
  int16_t val = (int16_t)this->read16_(ATM90E36_REGISTER_PMEANH + phase);
  return (float)val;
}

float ATM90E36Component::get_phase_angle_(uint8_t phase) {
  int16_t val = (int16_t)this->read16_(ATM90E36_REGISTER_PANGLE + phase) / 10.0;
  return (val > 180) ? (float) (val - 360.0f) : (float) val;
}

float ATM90E36Component::get_phase_peak_current_(uint8_t phase) {
  int16_t val = (float) this->read16_(ATM90E36_REGISTER_IPEAK + phase);
  if (!this->peak_current_signed_)
    val = std::abs(val);
  // phase register * phase current gain value  / 1000 * 2^13
  return (val * this->phase_[phase].ct_gain_ / 8192000.0);
}

float ATM90E36Component::get_frequency_() {
  const uint16_t freq = this->read16_(ATM90E36_REGISTER_FREQ);
  return (float) freq / 100;
}

float ATM90E36Component::get_chip_temperature_() {
  const uint16_t ctemp = this->read16_(ATM90E36_REGISTER_TEMP);
  return (float) ctemp;
}


#ifdef USE_TEXT_SENSOR
void ATM90E36Component::check_phase_status() {
	SysStatus0 state0;
	SysStatus1 state1;
	
	state0.data = this->read16_(ATM90E36_REGISTER_SYSSTATUS0);
	state1.data = this->read16_(ATM90E36_REGISTER_SYSSTATUS1);
	
	std::string status;
	
	if (state0.bits.SagWarn) {
		status += "Voltage Sag; ";
	}
	if (state1.bits.THDUOv) {
		status += "Over Voltage; ";
	}
	
	auto *sensor = this->phase_status_text_sensor_;
	if (sensor == nullptr)
		return;

	if (!status.empty()) {
		status.pop_back();  // remove space
		status.pop_back();  // remove semicolon
		ESP_LOGW(TAG, "%s: %s", sensor->get_name().c_str(), status.c_str());
		sensor->publish_state(status);
	} else {
		sensor->publish_state("Okay");
	}
}

void ATM90E36Component::check_freq_status() {
	SysStatus0 state0;
	
	state0.data = this->read16_(ATM90E36_REGISTER_SYSSTATUS0);
	
	std::string freq_status;

	if (state0.bits.SagWarn) {
		freq_status = "LOW";
	} else {
		freq_status = "Normal";
	}
	if (this->freq_status_text_sensor_ != nullptr) {
		if (freq_status == "Normal") {
		  ESP_LOGD(TAG, "Frequency status: %s", freq_status.c_str());
		} else {
		  ESP_LOGW(TAG, "Frequency status: %s", freq_status.c_str());
		}
		this->freq_status_text_sensor_->publish_state(freq_status);
	}

}
#endif

uint16_t ATM90E36Component::calculate_voltage_threshold(int line_freq, uint16_t ugain, float multiplier) {
  // this assumes that 60Hz electrical systems use 120V mains,
  // which is usually, but not always the case
  float nominal_voltage = (line_freq == 60) ? 120.0f : 220.0f;
  float target_voltage = nominal_voltage * multiplier;

  float peak_01v = target_voltage * 100.0f * std::numbers::sqrt2_v<float>;  // convert RMS → peak, scale to 0.01V
  float divider = (2.0f * ugain) / 32768.0f;

  float threshold = peak_01v / divider;

  return static_cast<uint16_t>(threshold);
}

bool ATM90E36Component::validate_spi_read_(uint16_t expected, const char *context) {
  uint16_t last = this->read16_(ATM90E36_REGISTER_LASTSPIDATA);
  if (last != expected) {
    if (context != nullptr) {
      ESP_LOGW(TAG, "[%s] SPI read mismatch: expected 0x%04X, got 0x%04X", context, expected, last);
    } else {
      ESP_LOGW(TAG, "SPI read mismatch: expected 0x%04X, got 0x%04X", expected, last);
    }
    return false;
  }
  return true;
}

}  // namespace atm90e36
}  // namespace esphome
