/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website ditronix.net, github.com/DitroniX and hackster.io/DitroniX
*/

// Libraries
#include <ATM90E3x.h>

ATM90E3x::ATM90E3x(void)
{
  // Begin
}

ATM90E3x::~ATM90E3x()
{
  // End
}

// **************** ATM90E - Communication Establishment ****************
/*
  - Defines Register Mask
  - Treats the Register and SPI Comms
  - Outputs the required value in the register
*/
unsigned short ATM90E3x::CommEnergyIC(unsigned char RW, unsigned short address, unsigned short val)
{
  unsigned char *data = (unsigned char *)&val;
  unsigned char *adata = (unsigned char *)&address;
  unsigned short output;
  unsigned short address1;

  // **************** SPI Settings ESP32 ****************
  SPISettings settings(200000, MSBFIRST, SPI_MODE3);

  // Switch MSB and LSB of value
  output = (val >> 8) | (val << 8);
  val = output;

  // Set R/W flag
  address |= RW << 15;

  // Swap byte address
  address1 = (address >> 8) | (address << 8);
  address = address1;

  // Transmit & Receive Data
  SPI.beginTransaction(settings);

  // Chip enable and wait for SPI activation
  digitalWrite(CS_ATM, LOW);
  delayMicroseconds(10);

  // Write address byte by byte
  for (byte i = 0; i < 2; i++)
  {
    SPI.transfer(*adata);
    adata++;
  }

  /* Must wait 4 us for data to become valid */
  delayMicroseconds(4);

  // READ Data
  // Do for each byte in transfer
  if (RW)
  {
    for (byte i = 0; i < 2; i++)
    {
      *data = SPI.transfer(0x00);
      data++;
    }
  }
  else
  {
    for (byte i = 0; i < 2; i++)
    {
      SPI.transfer(*data);
      data++;
    }
  }

  // Chip enable and wait for transaction to end
  digitalWrite(CS_ATM, HIGH);
  delayMicroseconds(10);

  SPI.endTransaction();

  output = (val >> 8) | (val << 8); // reverse MSB and LSB
  return output;
} // ATM90E3x::CommEnergyIC

// **************** REGISTER FUNCTIONS ****************

int ATM90E3x::Read32Register(signed short regh_addr, signed short regl_addr)
{
  int val, val_h, val_l;
  val_h = CommEnergyIC(READ, regh_addr, 0xFFFF);
  val_l = CommEnergyIC(READ, regl_addr, 0xFFFF);
  val = CommEnergyIC(READ, regh_addr, 0xFFFF);

  val = val_h << 16;
  val |= val_l; // concatenate the 2 registers to make 1 32 bit number

  return (val);
} // ATM90E3x::Read32Register

double ATM90E3x::CalculateVIOffset(unsigned short regh_addr, unsigned short regl_addr /*, unsigned short offset_reg*/)
{
  // for getting the lower registers of Voltage and Current and calculating the offset
  // should only be run when CT sensors are connected to the meter,
  // but not connected around wires
  uint32_t val, val_h, val_l;
  uint16_t offset;
  val_h = CommEnergyIC(READ, regh_addr, 0xFFFF);
  val_l = CommEnergyIC(READ, regl_addr, 0xFFFF);
  val = CommEnergyIC(READ, regh_addr, 0xFFFF);

  val = val_h << 16; // move high register up 16 bits
  val |= val_l;      // concatenate the 2 registers to make 1 32 bit number
  val = val >> 7;    // right shift 7 bits - lowest 7 get ignored - V & I registers need this
  val = (~val) + 1;  // 2s compliment

  offset = val; // keep lower 16 bits
  // CommEnergyIC(WRITE, offset_reg, (signed short)val);
  return uint16_t(offset);
} // ATM90E3x::CalculateVIOffset

double ATM90E3x::CalculatePowerOffset(unsigned short regh_addr, unsigned short regl_addr /*, unsigned short offset_reg*/)
{
  // for getting the lower registers of energy and calculating the offset
  // should only be run when CT sensors are connected to the meter,
  // but not connected around wires
  uint32_t val, val_h, val_l;
  uint16_t offset;
  val_h = CommEnergyIC(READ, regh_addr, 0xFFFF);
  val_l = CommEnergyIC(READ, regl_addr, 0xFFFF);
  val = CommEnergyIC(READ, regh_addr, 0xFFFF);

  val = val_h << 16; // move high register up 16 bits
  val |= val_l;      // concatenate the 2 registers to make 1 32 bit number
  val = (~val) + 1;  // 2s compliment

  offset = val; // keep lower 16 bits
  // CommEnergyIC(WRITE, offset_reg, (signed short)val);
  return uint16_t(offset);
} // ATM90E3x::CalculatePowerOffset

double ATM90E3x::CalibrateVI(unsigned short reg, unsigned short actualVal)
{
  // input the Voltage or Current register, and the actual value that it should be
  // actualVal can be from a calibration meter or known value from a power supply
  uint16_t gain, val, m, gainReg;
  // sample the reading
  val = CommEnergyIC(READ, reg, 0xFFFF);
  val += CommEnergyIC(READ, reg, 0xFFFF);
  val += CommEnergyIC(READ, reg, 0xFFFF);
  val += CommEnergyIC(READ, reg, 0xFFFF);

  // get value currently in gain register
  switch (reg)
  {
  case URMSCT1:
  {
    gainReg = UGainCT1;
  }
  case URMSCT2:
  {
    gainReg = UGainCT2;
  }
  case URMSCT3:
  {
    gainReg = UGainCT3;
  }
  case IRMSCT1:
  {
    gainReg = IGainCT1;
  }
  case IRMSCT2:
  {
    gainReg = IGainCT2;
  }
  case IRMSCT3:
  {
    gainReg = IGainCT3;
  }
  }

  gain = CommEnergyIC(READ, gainReg, 0xFFFF);
  m = actualVal;
  m = ((m * gain) / val);
  gain = m;

  // write new value to gain register
  CommEnergyIC(WRITE, gainReg, gain);

  return (gain);
} // ATM90E3x::CalibrateVI

#if ATM90DEVICE == ATM90E36_DEVICE
/* Checksum Error Function */
bool ATM90E3x::calibrationError()
{
  bool CS0, CS1, CS2, CS3;
  unsigned short systemstatus0 = GetSysStatus0();

  if (systemstatus0 & 0x4000)
  {
    CS0 = true;
  }
  else
  {
    CS0 = false;
  }

  if (systemstatus0 & 0x1000) // DW Updated 15th June 23, thanks to AM for highlighing this 'possible' typo in TD code.
  {
    CS1 = true;
  }
  else
  {
    CS1 = false;
  }
  if (systemstatus0 & 0x0400)
  {
    CS2 = true;
  }
  else
  {
    CS2 = false;
  }
  if (systemstatus0 & 0x0100)
  {
    CS3 = true;
  }
  else
  {
    CS3 = false;
  }

  Serial.print("Checksum 0: ");
  Serial.print(CS0);
  Serial.print("\tChecksum 1: ");
  Serial.println(CS1);
  Serial.print("Checksum 2: ");
  Serial.print(CS2);
  Serial.print("\tChecksum 3: ");
  Serial.println(CS3);

  if (CS0 || CS1 || CS2 || CS3)
    return (true);
  else
    return (false);
}
#endif

// **************** PARAMETER FUNCTIONS ****************
// Returns Main Electrical Parameters, such as: Voltage, Current, Power, Energy, and Frequency, and Temperature

// **************** VOLTAGE
double ATM90E3x::GetLineVoltage1()
{
  unsigned short voltage = CommEnergyIC(READ, URMSCT1, 0xFFFF);
  return (double)voltage / 100;
}
double ATM90E3x::GetLineVoltage2()
{
  unsigned short voltage = CommEnergyIC(READ, URMSCT2, 0xFFFF);
  return (double)voltage / 100;
}
double ATM90E3x::GetLineVoltage3()
{
  unsigned short voltage = CommEnergyIC(READ, URMSCT3, 0xFFFF);
  return (double)voltage / 100;
}

// **************** CURRENT
double ATM90E3x::GetLineCurrentCT1()
{
  unsigned short current = CommEnergyIC(READ, IRMSCT1, 0xFFFF);
  return (double)current / 1000;
}
double ATM90E3x::GetLineCurrentCT2()
{
  unsigned short current = CommEnergyIC(READ, IRMSCT2, 0xFFFF);
  return (double)current / 1000;
}
double ATM90E3x::GetLineCurrentCT3()
{
  unsigned short current = CommEnergyIC(READ, IRMSCT3, 0xFFFF);
  return (double)current / 1000;
}

double ATM90E3x::GetLineCurrentCTN()
{
  unsigned short current = CommEnergyIC(READ, IRMSN, 0xFFFF);
  return (double)current / 1000;
}

// **************** ACTIVE POWER
#if ATM90DEVICE == ATM90E32_DEVICE

double ATM90E3x::GetActivePowerCT1()
{
  int val = Read32Register(PmeanCT1, PmeanCT1LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetActivePowerCT2()
{
  int val = Read32Register(PmeanCT2, PmeanCT2LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetActivePowerCT3()
{
  int val = Read32Register(PmeanCT3, PmeanCT3LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetTotalActivePower()
{
  int val = Read32Register(PmeanT, PmeanTLSB);
  return (double)val * 0.00032;
}

// Active Fundamental Power
double ATM90E3x::GetTotalActiveFundPower()
{
  int val = Read32Register(PmeanTF, PmeanTFLSB);
  return (double)val * 0.00032;
}

// Active Harmonic Power
double ATM90E3x::GetTotalActiveHarPower()
{
  int val = Read32Register(PmeanTH, PmeanTHLSB);
  return (double)val * 0.00032;
}
#endif

#if ATM90DEVICE == ATM90E36_DEVICE

double ATM90E3x::GetActivePowerCT1()
{
  signed short apower = (signed short)CommEnergyIC(READ, PmeanCT1, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetActivePowerCT2()
{
  signed short apower = (signed short)CommEnergyIC(READ, PmeanCT2, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetActivePowerCT3()
{
  signed short apower = (signed short)CommEnergyIC(READ, PmeanCT3, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetTotalActivePower()
{
  signed short apower = (signed short)CommEnergyIC(READ, PmeanT, 0xFFFF);
  return (double)apower / 250;
}
#endif

// **************** REACTIVE POWER ****************

#if ATM90DEVICE == ATM90E32_DEVICE
double ATM90E3x::GetReactivePowerCT1()
{
  int val = Read32Register(QmeanCT1, QmeanCT1LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetReactivePowerCT2()
{
  int val = Read32Register(QmeanCT2, QmeanCT2LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetReactivePowerCT3()
{
  int val = Read32Register(QmeanCT3, QmeanCT3LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetTotalReactivePower()
{
  int val = Read32Register(QmeanT, QmeanTLSB);
  return (double)val * 0.00032;
}

#endif

#if ATM90DEVICE == ATM90E36_DEVICE
double ATM90E3x::GetReactivePowerCT1()
{
  signed short apower = (signed short)CommEnergyIC(READ, QmeanCT1, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetReactivePowerCT2()
{
  signed short apower = (signed short)CommEnergyIC(READ, QmeanCT2, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetReactivePowerCT3()
{
  signed short apower = (signed short)CommEnergyIC(READ, QmeanCT3, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetTotalReactivePower()
{
  signed short apower = (signed short)CommEnergyIC(READ, QmeanT, 0xFFFF);
  return (double)apower / 250;
}
#endif

// **************** APPARENT POWER ****************
#if ATM90DEVICE == ATM90E32_DEVICE
double ATM90E3x::GetApparentPowerCT1()
{
  int val = Read32Register(SmeanCT1, SmeanCT1LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetApparentPowerCT2()
{
  int val = Read32Register(SmeanCT2, SmeanCT2LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetApparentPowerCT3()
{
  int val = Read32Register(SmeanCT3, SmeanCT3LSB);
  return (double)val * 0.00032;
}
double ATM90E3x::GetTotalApparentPower()
{
  int val = Read32Register(SmeanT, SAmeanTLSB);
  return (double)val * 0.00032;
}
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
double ATM90E3x::GetApparentPowerCT1()
{
  signed short apower = (signed short)CommEnergyIC(READ, SmeanCT1, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetApparentPowerCT2()
{
  signed short apower = (signed short)CommEnergyIC(READ, SmeanCT2, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetApparentPowerCT3()
{
  signed short apower = (signed short)CommEnergyIC(READ, SmeanCT3, 0xFFFF);
  return (double)apower / 1000;
}
double ATM90E3x::GetTotalApparentPower()
{
  signed short apower = (signed short)CommEnergyIC(READ, SmeanT, 0xFFFF);
  return (double)apower / 250;
}
#endif

// **************** FREQUENCY ****************
double ATM90E3x::GetFrequency()
{
  unsigned short freq = CommEnergyIC(READ, Freq, 0xFFFF);
  return (double)freq / 100;
}

// **************** POWER FACTOR ****************
double ATM90E3x::GetPowerFactorCT1()
{
  signed short pf = (signed short)CommEnergyIC(READ, PFmeanCT1, 0xFFFF);
  return (double)pf / 1000;
}
double ATM90E3x::GetPowerFactorCT2()
{
  signed short pf = (signed short)CommEnergyIC(READ, PFmeanCT2, 0xFFFF);
  return (double)pf / 1000;
}
double ATM90E3x::GetPowerFactorCT3()
{
  signed short pf = (signed short)CommEnergyIC(READ, PFmeanCT3, 0xFFFF);
  return (double)pf / 1000;
}
double ATM90E3x::GetTotalPowerFactor()
{
  signed short pf = (signed short)CommEnergyIC(READ, PFmeanT, 0xFFFF);
  return (double)pf / 1000;
}

#if ATM90DEVICE == ATM90E36_DEVICE

// **************** Voltage Harmonics ****************
double ATM90E3x::GetVHarmCT1()
{
  unsigned short value = CommEnergyIC(READ, THDNUCT1, 0xFFFF);
  return (double)value;
}
double ATM90E3x::GetVHarmCT2()
{
  unsigned short value = CommEnergyIC(READ, THDNUCT2, 0xFFFF);
  return (double)value;
}
double ATM90E3x::GetVHarmCT3()
{
  unsigned short value = CommEnergyIC(READ, THDNUCT3, 0xFFFF);
  return (double)value;
}

// **************** Current Harmonics ****************
double ATM90E3x::GetCHarmCT1()
{
  unsigned short value = CommEnergyIC(READ, THDNICT1, 0xFFFF);
  return (double)value;
}
double ATM90E3x::GetCHarmCT2()
{
  unsigned short value = CommEnergyIC(READ, THDNICT2, 0xFFFF);
  return (double)value;
}
double ATM90E3x::GetCHarmCT3()
{
  unsigned short value = CommEnergyIC(READ, THDNICT3, 0xFFFF);
  return (double)value;
}
#endif

// **************** MEAN PHASE ANGLE ****************
double ATM90E3x::GetPhaseCT1()
{
  unsigned short angleA = (unsigned short)CommEnergyIC(READ, PAngleCT1, 0xFFFF);
  return (double)angleA / 10;
}
double ATM90E3x::GetPhaseCT2()
{
  unsigned short angleB = (unsigned short)CommEnergyIC(READ, PAngleCT2, 0xFFFF);
  return (double)angleB / 10;
}
double ATM90E3x::GetPhaseCT3()
{
  unsigned short angleC = (unsigned short)CommEnergyIC(READ, PAngleCT3, 0xFFFF);
  return (double)angleC / 10;
}

// **************** TEMPERATURE ****************
double ATM90E3x::GetTemperature()
{
  short int atemp = (short int)CommEnergyIC(READ, Temp, 0xFFFF);
  return (double)atemp;
}

// **************** GET REGISTER VALUE ****************
double ATM90E3x::GetValueRegister(unsigned short registerRead)
{
  return (double)CommEnergyIC(READ, registerRead, 0xFFFF); // returns value register
}

// REGULAR ENERGY MEASUREMENT

// **************** FORWARD ACTIVE ENERGY ****************
// these registers accumulate energy and are cleared after being read
double ATM90E3x::GetImportEnergy()
{
  unsigned short ienergyT = CommEnergyIC(READ, APenergyT, 0xFFFF);
  // unsigned short ienergyCT1 = CommEnergyIC(READ, APenergyCT1, 0xFFFF);
  // unsigned short ienergyCT2 = CommEnergyIC(READ, APenergyCT2, 0xFFFF);
  // unsigned short ienergyCT3 = CommEnergyIC(READ, APenergyCT3, 0xFFFF);

  // unsigned short renergyT = CommEnergyIC(READ, RPenergyT, 0xFFFF);
  // unsigned short renergyCT1 = CommEnergyIC(READ, RPenergyCT1, 0xFFFF);
  // unsigned short renergyCT2 = CommEnergyIC(READ, RPenergyCT2, 0xFFFF);
  // unsigned short renergyCT3 = CommEnergyIC(READ, RPenergyCT3, 0xFFFF);

  // unsigned short senergyT = CommEnergyIC(READ, SAenergyT, 0xFFFF);
  // unsigned short senergyCT1 = CommEnergyIC(READ, SenergyCT1, 0xFFFF);
  // unsigned short senergyCT2 = CommEnergyIC(READ, SenergyCT2, 0xFFFF);
  // unsigned short senergyCT3 = CommEnergyIC(READ, SenergyCT3, 0xFFFF);

#if ATM90DEVICE == ATM90E32_DEVICE
  return (double)ienergyT / 100 / 3200; // returns kWh
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  return (double)(((double)ienergyT / 32) * 3600); // returns kWh
#endif
}

// **************** FORWARD REACTIVE ENERGY ****************
#if ATM90DEVICE == ATM90E32_DEVICE
double ATM90E3x::GetImportReactiveEnergy()
{
  unsigned short renergyT = CommEnergyIC(READ, RPenergyT, 0xFFFF);
  return (double)renergyT / 100 / 3200; // returns kWh
}
// unsigned short renergyCT1 = CommEnergyIC(READ, RPenergyCT1, 0xFFFF);
// unsigned short renergyCT2 = CommEnergyIC(READ, RPenergyCT2, 0xFFFF);
// unsigned short renergyCT3 = CommEnergyIC(READ, RPenergyCT3, 0xFFFF);

// **************** APPARENT ENERGY ****************
double ATM90E3x::GetImportApparentEnergy()
{
  unsigned short senergyT = CommEnergyIC(READ, SAenergyT, 0xFFFF);
  return (double)senergyT / 100 / 3200; // returns kWh
}
// unsigned short senergyCT1 = CommEnergyIC(READ, SenergyCT1, 0xFFFF);
// unsigned short senergyCT2 = CommEnergyIC(READ, SenergyCT2, 0xFFFF);
// unsigned short senergyCT3 = CommEnergyIC(READ, SenergyCT3, 0xFFFF);
#endif

// **************** REVERSE ACTIVE ENERGY ****************
double ATM90E3x::GetExportEnergy()
{
  unsigned short eenergyT = CommEnergyIC(READ, ANenergyT, 0xFFFF);
  // unsigned short eenergyCT1 = CommEnergyIC(READ, ANenergyCT1, 0xFFFF);
  // unsigned short eenergyCT2 = CommEnergyIC(READ, ANenergyCT2, 0xFFFF);
  // unsigned short eenergyCT3 = CommEnergyIC(READ, ANenergyCT3, 0xFFFF);

  // unsigned short reenergyT = CommEnergyIC(READ, RNenergyT, 0xFFFF);
  // unsigned short reenergyCT1 = CommEnergyIC(READ, RNenergyCT1, 0xFFFF);
  // unsigned short reenergyCT2 = CommEnergyIC(READ, RNenergyCT2, 0xFFFF);
  // unsigned short reenergyCT3 = CommEnergyIC(READ, RNenergyCT3, 0xFFFF);

#if ATM90DEVICE == ATM90E32_DEVICE
  // unsigned short eenergyCT1 = CommEnergyIC(READ, ANenergyCT1, 0xFFFF);
  // unsigned short eenergyCT2 = CommEnergyIC(READ, ANenergyCT2, 0xFFFF);
  // unsigned short eenergyCT3 = CommEnergyIC(READ, ANenergyCT3, 0xFFFF);
  return (double)eenergyT / 100 / 3200; // returns kWh
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  return (double)(((double)eenergyT / 32) * 3600); // returns kWh
#endif
}

// **************** REVERSE REACTIVE ENERGY ****************
#if ATM90DEVICE == ATM90E32_DEVICE
double ATM90E3x::GetExportReactiveEnergy()
{
  unsigned short reenergyT = CommEnergyIC(READ, RNenergyT, 0xFFFF);
  return (double)reenergyT / 100 / 3200; // returns kWh
}
// unsigned short reenergyCT1 = CommEnergyIC(READ, RNenergyCT1, 0xFFFF);
// unsigned short reenergyCT2 = CommEnergyIC(READ, RNenergyCT2, 0xFFFF);
// unsigned short reenergyCT3 = CommEnergyIC(READ, RNenergyCT3, 0xFFFF);
#endif

// **************** System Status Registers ****************
#if ATM90DEVICE == ATM90E32_DEVICE
unsigned short ATM90E3x::GetSysStatus0()
{
  return CommEnergyIC(READ, EMMIntState0, 0xFFFF);
}
unsigned short ATM90E3x::GetSysStatus1()
{
  return CommEnergyIC(READ, EMMIntState1, 0xFFFF);
}
unsigned short ATM90E3x::GetMeterStatus0()
{
  return CommEnergyIC(READ, EMMState0, 0xFFFF);
}
unsigned short ATM90E3x::GetMeterStatus1()
{
  return CommEnergyIC(READ, EMMState1, 0xFFFF);
}
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
unsigned short ATM90E3x::GetSysStatus0()
{
  return CommEnergyIC(READ, SysStatus0, 0xFFFF);
}
unsigned short ATM90E3x::GetSysStatus1()
{
  return CommEnergyIC(READ, SysStatus1, 0xFFFF);
}
unsigned short ATM90E3x::GetMeterStatus0()
{
  return CommEnergyIC(READ, EnStatus0, 0xFFFF);
}
unsigned short ATM90E3x::GetMeterStatus1()
{
  return CommEnergyIC(READ, EnStatus1, 0xFFFF);
}
#endif

// **************** Configuration ****************
// Use SPI MODE 0 for the ATM90E3x ??????

#if ATM90DEVICE == ATM90E32_DEVICE || ATM_SPLITPHASE == true
void ATM90E3x::begin(unsigned short lineFreq, unsigned short pgagain, unsigned short uGainCT1, unsigned short uGainCT2, unsigned short uGainCT3, unsigned short iGainCT1, unsigned short iGainCT2, unsigned short iGainCT3)
#endif

#if ATM90DEVICE == ATM90E36_DEVICE && ATM_SPLITPHASE == false
    void ATM90E3x::begin(unsigned short lineFreq, unsigned short pgagain, unsigned short uGainCT1, unsigned short uGainCT2, unsigned short uGainCT3, unsigned short iGainCT1, unsigned short iGainCT2, unsigned short iGainCT3, unsigned short igainN)
#endif
{
  _lineFreq = lineFreq; // frequency of power
  _pgagain = pgagain;   // PGA Gain for current channels#

#if ATM_SINGLEVOLTAGE == true
  _uGainCT1 = uGainCT1; // Voltage RMS Gain
  _uGainCT2 = uGainCT1; // Duplicate uGainCT1 Values to _uGainCT2 and _uGainCT3.
  _uGainCT3 = uGainCT1; // Duplicate uGainCT1 Values to _uGainCT2 and _uGainCT3.
#else
  _uGainCT1 = uGainCT1; // Voltage 1 RMS Gain
  _uGainCT2 = uGainCT2; // Voltage 2 RMS Gain
  _uGainCT3 = uGainCT3; // Voltage 3 RMS Gain
#endif

  _iGainCT1 = iGainCT1; // CT1
  _iGainCT2 = iGainCT2; // CT2 - not used for single split phase meter
  _iGainCT3 = iGainCT3; // CT3 or CT2 for single split phase meter

#if ATM90DEVICE == ATM90E36_DEVICE && ATM_SPLITPHASE == false
  _igainN = igainN; // CTN (Used on the ATM90E36)
#endif

  pinMode(CS_ATM, OUTPUT);

  /* Enable SPI */
  SPI.begin(SCLK, MISO, MOSI); // (SCK, MISO, MOSI, SS)

  Serial.print("Connecting to the ");

#if ATM90DEVICE == ATM90E32_DEVICE
  Serial.println("ATM90E32");
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  Serial.println("ATM90E36");
#endif

  Serial.println("====================================");

  // calculation for voltage sag threshold - assumes we do not want to go under 90v for split phase and 190v otherwise
  // determine proper low and high frequency threshold
  unsigned short vSagTh;
  unsigned short sagV;
  unsigned short FreqHiThresh;
  unsigned short FreqLoThresh;
  if (_lineFreq == 4485 || _lineFreq == 5231)
  {
    sagV = 90;
    FreqHiThresh = 61 * 100;
    FreqLoThresh = 59 * 100;
  }
  else
  {
    sagV = 190;
    FreqHiThresh = 51 * 100;
    FreqLoThresh = 49 * 100;
  }

  vSagTh = (sagV * 100 * sqrt(2)) / (2 * _uGainCT1 / 32768);

  // **************** Initialize registers ****************
  CommEnergyIC(WRITE, SoftReset, 0x789A); // 70 Perform soft reset

#if ATM90DEVICE == ATM90E36_DEVICE
  // SagTh = Vth * 100 * sqrt(2) / (2 * Ugain / 32768)
  CommEnergyIC(WRITE, FuncEn0, 0x0000); // Voltage sag
  CommEnergyIC(WRITE, FuncEn1, 0x0000); // Voltage sag
  CommEnergyIC(WRITE, SagTh, 0x0001);   // Voltage sag threshold
#endif

  // **************** Status registers ****************
#if ATM90DEVICE == ATM90E32_DEVICE
  CommEnergyIC(WRITE, CfgRegAccEn, 0x55AA); // 7F enable register config access
  CommEnergyIC(WRITE, MeterEn, 0x0001);     // 00 Enable Metering

  CommEnergyIC(WRITE, SagPeakDetCfg, 0x143F);  // 05 Sag and Voltage peak detect period set to 20ms
  CommEnergyIC(WRITE, SagTh, vSagTh);          // 08 Voltage sag threshold
  CommEnergyIC(WRITE, FreqHiTh, FreqHiThresh); // 0D High frequency threshold
  CommEnergyIC(WRITE, FreqLoTh, FreqLoThresh); // 0C Lo frequency threshold
  CommEnergyIC(WRITE, EMMIntEn0, 0xB76F);      // 75 Enable interrupts
  CommEnergyIC(WRITE, EMMIntEn1, 0xDDFD);      // 76 Enable interrupts
  CommEnergyIC(WRITE, EMMIntState0, 0x0001);   // 73 Clear interrupt flags
  CommEnergyIC(WRITE, EMMIntState1, 0x0001);   // 74 Clear interrupt flags
  CommEnergyIC(WRITE, ZXConfig, 0xD654);       // 07 ZX2, ZX1, ZX0 pin config - set to current channels, all polarity
#endif

// **************** Set metering config values (CONFIG) ****************
#if ATM90DEVICE == ATM90E32_DEVICE
  CommEnergyIC(WRITE, PLconstH, 0x0861);  // 31 PL Constant MSB (default) - Meter Constant = 3200 - PL Constant = 140625000
  CommEnergyIC(WRITE, PLconstL, 0xC468);  // 32 PL Constant LSB (default) - this is 4C68 in the application note, which is incorrect
  CommEnergyIC(WRITE, MMode0, _lineFreq); // 33 Mode Config (frequency set in main program)
  CommEnergyIC(WRITE, MMode1, _pgagain);  // 34 PGA Gain Configuration for Current Channels - 0x002A (x4) // 0x0015 (x2) // 0x0000 (1x)
  CommEnergyIC(WRITE, PStartTh, 0x1D4C);  // 35 All phase Active Startup Power Threshold - 50% of startup current = 0.02A/0.00032 = 7500
  CommEnergyIC(WRITE, QStartTh, 0x1D4C);  // 36 All phase Reactive Startup Power Threshold
  CommEnergyIC(WRITE, SStartTh, 0x1D4C);  // 37 All phase Apparent Startup Power Threshold
  CommEnergyIC(WRITE, PPhaseTh, 0x02EE);  // 38 Each phase Active Phase Threshold = 10% of startup current = 0.002A/0.00032 = 750
  CommEnergyIC(WRITE, QPhaseTh, 0x02EE);  // 39 Each phase Reactive Phase Threshold
  CommEnergyIC(WRITE, SPhaseTh, 0x02EE);  // 3A Each phase Apparent Phase Threshold
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  CommEnergyIC(WRITE, ConfigStart, 0x5678); // Metering calibration startup (factory 0x6886)
  CommEnergyIC(WRITE, PLconstH, 0x0861);    // PL Constant MSB (default)
  CommEnergyIC(WRITE, PLconstL, 0xC468);    // PL Constant LSB (default)
  CommEnergyIC(WRITE, MMode0, _lineFreq);   // 0x1087.  Mode Config (60 Hz, 3P4W)
  CommEnergyIC(WRITE, MMode1, _pgagain);    // 0x1500.  0x5555 (x2) // 0x0000 (1x)
  CommEnergyIC(WRITE, PStartTh, 0x0000);    // Active Startup Power Threshold
  CommEnergyIC(WRITE, QStartTh, 0x0000);    // Reactive Startup Power Threshold
  CommEnergyIC(WRITE, SStartTh, 0x0000);    // Apparent Startup Power Threshold
  CommEnergyIC(WRITE, PPhaseTh, 0x0000);    // Active Phase Threshold
  CommEnergyIC(WRITE, QPhaseTh, 0x0000);    // Reactive Phase Threshold
  CommEnergyIC(WRITE, SPhaseTh, 0x0000);    // Apparent  Phase Threshold
  CommEnergyIC(WRITE, CSZero, 0x4741);      // Checksum 0
#endif

// **************** Set Metering Calibration Register Values (CALIBRATION) ****************
#if ATM90DEVICE == ATM90E32_DEVICE
  CommEnergyIC(WRITE, PoffsetCT1, 0x0000); // 41 A line active power offset FFDC
  CommEnergyIC(WRITE, QoffsetCT1, 0x0000); // 42 A line reactive power offset
  CommEnergyIC(WRITE, PoffsetCT2, 0x0000); // 43 B line active power offset
  CommEnergyIC(WRITE, QoffsetCT2, 0x0000); // 44 B line reactive power offset
  CommEnergyIC(WRITE, PoffsetCT3, 0x0000); // 45 C line active power offset
  CommEnergyIC(WRITE, QoffsetCT3, 0x0000); // 46 C line reactive power offset
  CommEnergyIC(WRITE, PQGainCT1, 0x0000);  // 47 Line calibration gain
  CommEnergyIC(WRITE, PhiCT1, 0x0000);     // 48 Line calibration angle
  CommEnergyIC(WRITE, PQGainCT2, 0x0000);  // 49 Line calibration gain
  CommEnergyIC(WRITE, PhiCT2, 0x0000);     // 4A Line calibration angle
  CommEnergyIC(WRITE, PQGainCT3, 0x0000);  // 4B Line calibration gain
  CommEnergyIC(WRITE, PhiCT3, 0x0000);     // 4C Line calibration angle
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  CommEnergyIC(WRITE, CalStart, 0x5678); // Metering calibration startup

  CommEnergyIC(WRITE, PoffsetCT1, 0x0000); // CT1 line active power offset
  CommEnergyIC(WRITE, QoffsetCT1, 0x0000); // CT1 line reactive power offset
  CommEnergyIC(WRITE, PoffsetCT2, 0x0000); // CT2 line active power offset
  CommEnergyIC(WRITE, QoffsetCT2, 0x0000); // CT2 line reactive power offset
  CommEnergyIC(WRITE, PoffsetCT3, 0x0000); // CT3 line active power offset
  CommEnergyIC(WRITE, QoffsetCT3, 0x0000); // CT3 line reactive power offset
  CommEnergyIC(WRITE, GainCT1, 0x0000);    // Line calibration gain
  CommEnergyIC(WRITE, PhiCT1, 0x0000);     // Line calibration angle
  CommEnergyIC(WRITE, GainCT2, 0x0000);    // Line calibration gain
  CommEnergyIC(WRITE, PhiCT2, 0x0000);     // Line calibration angle
  CommEnergyIC(WRITE, GainCT3, 0x0000);    // Line calibration gain
  CommEnergyIC(WRITE, PhiCT3, 0x0000);     // Line calibration angle

  CommEnergyIC(WRITE, CSOne, 0x0000); // Checksum 1
#endif

// **************** FUNDAMENTAL/HARMONIC & ENERGY CALIBRATION REGISTERS ****************
#if ATM90DEVICE == ATM90E32_DEVICE
  CommEnergyIC(WRITE, PoffsetCT1F, 0x0000); // 51 CT1 Fund. active power offset
  CommEnergyIC(WRITE, PoffsetCT2F, 0x0000); // 52 CT2 Fund. active power offset
  CommEnergyIC(WRITE, PoffsetCT3F, 0x0000); // 53 CT3 Fund. active power offset
  CommEnergyIC(WRITE, PGainCT1F, 0x0000);   // 54 CT1 Fund. active power gain
  CommEnergyIC(WRITE, PGainCT2F, 0x0000);   // 55 CT2 Fund. active power gain
  CommEnergyIC(WRITE, PGainCT3F, 0x0000);   // 56 CT3 Fund. active power gain
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  CommEnergyIC(WRITE, HaRMStart, 0x5678);   // Metering calibration startup
  CommEnergyIC(WRITE, PoffsetCT1F, 0x0000); // CT1 Fund. active power offset
  CommEnergyIC(WRITE, PoffsetCT2F, 0x0000); // CT2 Fund. active power offset
  CommEnergyIC(WRITE, PoffsetCT3F, 0x0000); // CT3 Fund. active power offset
  CommEnergyIC(WRITE, PGainCT1F, 0x0000);   // CT1 Fund. active power gain
  CommEnergyIC(WRITE, PGainCT2F, 0x0000);   // CT2 Fund. active power gain
  CommEnergyIC(WRITE, PGainCT3F, 0x0000);   // CT3 Fund. active power gain
  CommEnergyIC(WRITE, CSTwo, 0x0000);       // Checksum 2
#endif

// **************** MEASUREMENT CALIBRATION REGISTERS ****************
#if ATM90DEVICE == ATM90E32_DEVICE
  // A
  CommEnergyIC(WRITE, UGainCT1, _uGainCT1); // 61 CT1 Voltage RMS gain
  CommEnergyIC(WRITE, IGainCT1, _iGainCT1); // 62 CT1 line current gain
  CommEnergyIC(WRITE, UoffsetCT1, 0x0000);  // 63 CT1 Voltage offset - 61A8
  CommEnergyIC(WRITE, IoffsetCT1, 0x0000);  // 64 CT1 line current offset - FE60
  // B
  CommEnergyIC(WRITE, UGainCT2, _uGainCT2); // 65 CT2 Voltage RMS gain
  CommEnergyIC(WRITE, IGainCT2, _iGainCT2); // 66 CT2 line current gain
  CommEnergyIC(WRITE, UoffsetCT2, 0x0000);  // 67 CT2 Voltage offset - 1D4C
  CommEnergyIC(WRITE, IoffsetCT2, 0x0000);  // 68 CT2 line current offset - FE60
  // C
  CommEnergyIC(WRITE, UGainCT3, _uGainCT3); // 69 CT3 Voltage RMS gain
  CommEnergyIC(WRITE, IGainCT3, _iGainCT3); // 6A CT3 line current gain
  CommEnergyIC(WRITE, UoffsetCT3, 0x0000);  // 6B CT3 Voltage offset - 1D4C
  CommEnergyIC(WRITE, IoffsetCT3, 0x0000);  // 6C CT3 line current offset
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  CommEnergyIC(WRITE, AdjStart, 0x5678); // Measurement calibration

  // A
  CommEnergyIC(WRITE, UGainCT1, _uGainCT1); // CT1 SVoltage RMS gain
  CommEnergyIC(WRITE, IGainCT1, _iGainCT1); // CT1 line current gain.
  CommEnergyIC(WRITE, UoffsetCT1, 0x0000);  // CT1 Voltage offset
  CommEnergyIC(WRITE, IoffsetCT1, 0x0000);  // CT1 line current offset
  // B
  CommEnergyIC(WRITE, UGainCT2, _uGainCT2); // CT2 Voltage RMS gain.
  CommEnergyIC(WRITE, IGainCT2, _iGainCT2); // CT2 line current gain
  CommEnergyIC(WRITE, UoffsetCT2, 0x0000);  // CT2 Voltage offset
  CommEnergyIC(WRITE, IoffsetCT2, 0x0000);  // CT2 line current offset
  // C
  CommEnergyIC(WRITE, UGainCT3, _uGainCT3); // CT3 Voltage RMS gain
  CommEnergyIC(WRITE, IGainCT3, _iGainCT3); // CT3 line current gain
  CommEnergyIC(WRITE, UoffsetCT3, 0x0000);  // CT3 Voltage offset
  CommEnergyIC(WRITE, IoffsetCT3, 0x0000);  // CT3 line current offset
  // N
  CommEnergyIC(WRITE, IgainN, 0xFD7F); // D line current gain

  CommEnergyIC(WRITE, CSThree, 0x02F6); // Checksum 3
#endif

#if ATM90DEVICE == ATM90E32_DEVICE
  Serial.print("ATM90E32");
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  Serial.print("ATM90E36");
#endif

  Serial.println(" Connected - OK");

// **************** DEFINE EMM STATUS REGISTERS ****************
#if ATM90DEVICE == ATM90E32_DEVICE
  CommEnergyIC(WRITE, CfgRegAccEn, 0x0000); // 7F end configuration
#endif
}

int ATM90E3x::ReadRegister(signed short regh_addr, signed short regl_addr, String reg_desc)
{

  Serial.print("0x");
  if (regh_addr < 0x10)
    Serial.print('0');
  Serial.print(regh_addr, HEX);

  Serial.print("\t\t");

  // int val = Read32Register(regh_addr, 0xFFFF);

  // Serial.print("0x" + val);

  // String RegisterChar = String(val);
  // RegisterChar.toUpperCase();
  // Serial.print("0x" + RegisterChar);

  // Serial.print(regh_addr, HEX);

  // Serial.print(",\t");

  // String Register_Value = String(val);
  // Register_Value.toUpperCase();

  // if (val < 0x10)
  //   Serial.print('0');
  // Serial.print(val, HEX);

  Serial.print(Read32Register(regh_addr, 0xFFFF), HEX);

  // Serial.print("\t");

  // Serial.print(Register_Value);

  Serial.print("\t\t\t" + reg_desc);
  Serial.print("\n\n");

  return (0);
}

unsigned short ATM90E3x::GetRegisters()
{

  Serial.println("ATM Specific Registers");
  Serial.println("Address\t\tValue\t\tDescription");

  int val = 0;
  val = ReadRegister(0x30, 0xffff, "#");
  // val = ReadRegister(0x30, 0xffff, "#");

  return (0);
}
