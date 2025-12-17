#pragma once

#include <cinttypes>

namespace esphome {
namespace atm90e36 {

/* STATUS REGISTERS */
static const uint16_t ATM90E36_REGISTER_SOFTRESET = 0x00;      // Software Reset
static const uint16_t ATM90E36_REGISTER_SYSSTATUS0 = 0x01;     // System Status 0
static const uint16_t ATM90E36_REGISTER_SYSSTATUS1 = 0x02;     // System Status 1
static const uint16_t ATM90E36_REGISTER_FUNCEN0 = 0x03;        // Function Enable 0
static const uint16_t ATM90E36_REGISTER_FUNCEN1 = 0x04;        // Function Enable 1
static const uint16_t ATM90E36_REGISTER_ZXCONFIG = 0x07;       // Zero-Crossing Config
static const uint16_t ATM90E36_REGISTER_SAGTH = 0x08;          // Voltage Sag Th
static const uint16_t ATM90E36_REGISTER_PHASELOSSTH = 0x09;    // Voltage Phase Losing Th
static const uint16_t ATM90E36_REGISTER_INWARNTH0 = 0x0A;      // Threshold for sampled (from ADC) N line rms current
static const uint16_t ATM90E36_REGISTER_INWARNTH1 = 0x0B;      // Threshold for sampled (from ADC) N line rms current
static const uint16_t ATM90E36_REGISTER_THDNUTH = 0x0C;        // Voltage THD Warning Threshold
static const uint16_t ATM90E36_REGISTER_THDNITH = 0x0D;        // Current THD Warning Threshold
static const uint16_t ATM90E36_REGISTER_DMACTRL = 0x0E;        // DMA Mode Interface Control
static const uint16_t ATM90E36_REGISTER_LASTSPIDATA = 0x0F;    // Last Read/ Write SPI Value

/* LOW POWER MODE REGISTERS - NOT USED */
static const uint16_t ATM90E36_REGISTER_DETECTCTRL = 0x10;
static const uint16_t ATM90E36_REGISTER_DETECTTH1 = 0x11;
static const uint16_t ATM90E36_REGISTER_DETECTTH2 = 0x12;
static const uint16_t ATM90E36_REGISTER_DETECTTH3 = 0x13;
static const uint16_t ATM90E36_REGISTER_PMOFFSETA = 0x14;
static const uint16_t ATM90E36_REGISTER_PMOFFSETB = 0x15;
static const uint16_t ATM90E36_REGISTER_PMOFFSETC = 0x16;
static const uint16_t ATM90E36_REGISTER_PMPGA = 0x17;
static const uint16_t ATM90E36_REGISTER_PMIRMSA = 0x18;
static const uint16_t ATM90E36_REGISTER_PMIRMSB = 0x19;
static const uint16_t ATM90E36_REGISTER_PMIRMSC = 0x1A;
static const uint16_t ATM90E36_REGISTER_PMCONFIG = 0x10B;
static const uint16_t ATM90E36_REGISTER_PMAVGSAMPLES = 0x1C;
static const uint16_t ATM90E36_REGISTER_PMIRMSLSB = 0x1D;

/* CONFIGURATION REGISTERS */
static const uint16_t ATM90E36_REGISTER_CONFIGSTART = 0x30;  // Calibration Start Command
static const uint16_t ATM90E36_REGISTER_PLCONSTH = 0x31;  // High Word of PL_Constant
static const uint16_t ATM90E36_REGISTER_PLCONSTL = 0x32;  // Low Word of PL_Constant
static const uint16_t ATM90E36_REGISTER_MMODE0 = 0x33;    // Metering Mode Config
static const uint16_t ATM90E36_REGISTER_MMODE1 = 0x34;    // PGA Gain Configuration for Current Channels
static const uint16_t ATM90E36_REGISTER_PSTARTTH = 0x35;  // Startup Power Th (P)
static const uint16_t ATM90E36_REGISTER_QSTARTTH = 0x36;  // Startup Power Th (Q)
static const uint16_t ATM90E36_REGISTER_SSTARTTH = 0x37;  // Startup Power Th (S)
static const uint16_t ATM90E36_REGISTER_PPHASETH = 0x38;  // Startup Power Accum Th (P)
static const uint16_t ATM90E36_REGISTER_QPHASETH = 0x39;  // Startup Power Accum Th (Q)
static const uint16_t ATM90E36_REGISTER_SPHASETH = 0x3A;  // Startup Power Accum Th (S)
static const uint16_t ATM90E36_REGISTER_CSO = 0x3B;       // Checksum 0

/* CALIBRATION REGISTERS */
static const uint16_t ATM90E36_REGISTER_CALSTART = 0X40;  // Calibration Start Command
static const uint16_t ATM90E36_REGISTER_POFFSETA = 0x41;  // A Line Power Offset (P)
static const uint16_t ATM90E36_REGISTER_QOFFSETA = 0x42;  // A Line Power Offset (Q)
static const uint16_t ATM90E36_REGISTER_POFFSETB = 0x43;  // B Line Power Offset (P)
static const uint16_t ATM90E36_REGISTER_QOFFSETB = 0x44;  // B Line Power Offset (Q)
static const uint16_t ATM90E36_REGISTER_POFFSETC = 0x45;  // C Line Power Offset (P)
static const uint16_t ATM90E36_REGISTER_QOFFSETC = 0x46;  // C Line Power Offset (Q)
static const uint16_t ATM90E36_REGISTER_PQGAINA = 0x47;   // A Line Calibration Gain
static const uint16_t ATM90E36_REGISTER_PHIA = 0x48;      // A Line Calibration Angle
static const uint16_t ATM90E36_REGISTER_PQGAINB = 0x49;   // B Line Calibration Gain
static const uint16_t ATM90E36_REGISTER_PHIB = 0x4A;      // B Line Calibration Angle
static const uint16_t ATM90E36_REGISTER_PQGAINC = 0x4B;   // C Line Calibration Gain
static const uint16_t ATM90E36_REGISTER_PHIC = 0x4C;      // C Line Calibration Angle
static const uint16_t ATM90E36_REGISTER_CS1 = 0X4D;       // Checksum 1

/* FUNDAMENTAL/HARMONIC ENERGY CALIBRATION REGISTERS */
static const uint16_t ATM90E36_REGISTER_HARMSTART = 0X50;  // Harmonic Calibration Startup Command
static const uint16_t ATM90E36_REGISTER_POFFSETAF = 0x51;  // A Fund Power Offset (P)
static const uint16_t ATM90E36_REGISTER_POFFSETBF = 0x52;  // B Fund Power Offset (P)
static const uint16_t ATM90E36_REGISTER_POFFSETCF = 0x53;  // C Fund Power Offset (P)
static const uint16_t ATM90E36_REGISTER_PGAINAF = 0x54;    // A Fund Power Gain (P)
static const uint16_t ATM90E36_REGISTER_PGAINBF = 0x55;    // B Fund Power Gain (P)
static const uint16_t ATM90E36_REGISTER_PGAINCF = 0x56;    // C Fund Power Gain (P)
static const uint16_t ATM90E36_REGISTER_CS2 = 0X57;        // Checksum 2

/* MEASUREMENT CALIBRATION REGISTERS */
static const uint16_t ATM90E36_REGISTER_ADJSTART = 0X60;  // Measurement Calibration Startup Command
static const uint16_t ATM90E36_REGISTER_UGAINA = 0x61;    // A Voltage RMS Gain
static const uint16_t ATM90E36_REGISTER_IGAINA = 0x62;    // A Current RMS Gain
static const uint16_t ATM90E36_REGISTER_UOFFSETA = 0x63;  // A Voltage Offset
static const uint16_t ATM90E36_REGISTER_IOFFSETA = 0x64;  // A Current Offset
static const uint16_t ATM90E36_REGISTER_UGAINB = 0x65;    // B Voltage RMS Gain
static const uint16_t ATM90E36_REGISTER_IGAINB = 0x66;    // B Current RMS Gain
static const uint16_t ATM90E36_REGISTER_UOFFSETB = 0x67;  // B Voltage Offset
static const uint16_t ATM90E36_REGISTER_IOFFSETB = 0x68;  // B Current Offset
static const uint16_t ATM90E36_REGISTER_UGAINC = 0x69;    // C Voltage RMS Gain
static const uint16_t ATM90E36_REGISTER_IGAINC = 0x6A;    // C Current RMS Gain
static const uint16_t ATM90E36_REGISTER_UOFFSETC = 0x6B;  // C Voltage Offset
static const uint16_t ATM90E36_REGISTER_IOFFSETC = 0x6C;  // C Current Offset
static const uint16_t ATM90E36_REGISTER_IGAINN = 0x6D;    // N Current Gain
static const uint16_t ATM90E36_REGISTER_IOFFSETN = 0x6E;  // N Current Offset
static const uint16_t ATM90E36_REGISTER_CS3 = 0X6F;       // Checksum 3

/* ENERGY REGISTERS */
static const uint16_t ATM90E36_REGISTER_APENERGYT = 0x80;  // Total Forward Active
static const uint16_t ATM90E36_REGISTER_APENERGY = 0x81;   // Forward Active Reg Base
static const uint16_t ATM90E36_REGISTER_APENERGYA = 0x81;  // A Forward Active
static const uint16_t ATM90E36_REGISTER_APENERGYB = 0x82;  // B Forward Active
static const uint16_t ATM90E36_REGISTER_APENERGYC = 0x83;  // C Forward Active
static const uint16_t ATM90E36_REGISTER_ANENERGYT = 0x84;  // Total Reverse Active
static const uint16_t ATM90E36_REGISTER_ANENERGY = 0x85;   // Reverse Active Reg Base
static const uint16_t ATM90E36_REGISTER_ANENERGYA = 0x85;  // A Reverse Active
static const uint16_t ATM90E36_REGISTER_ANENERGYB = 0x86;  // B Reverse Active
static const uint16_t ATM90E36_REGISTER_ANENERGYC = 0x87;  // C Reverse Active
static const uint16_t ATM90E36_REGISTER_RPENERGYT = 0x88;  // Total Forward Reactive
static const uint16_t ATM90E36_REGISTER_RPENERGYA = 0x89;  // A Forward Reactive
static const uint16_t ATM90E36_REGISTER_RPENERGYB = 0x8A;  // B Forward Reactive
static const uint16_t ATM90E36_REGISTER_RPENERGYC = 0x8B;  // C Forward Reactive
static const uint16_t ATM90E36_REGISTER_RNENERGYT = 0x8C;  // Total Reverse Reactive
static const uint16_t ATM90E36_REGISTER_RNENERGYA = 0x8D;  // A Reverse Reactive
static const uint16_t ATM90E36_REGISTER_RNENERGYB = 0x8E;  // B Reverse Reactive
static const uint16_t ATM90E36_REGISTER_RNENERGYC = 0x8F;  // C Reverse Reactive

static const uint16_t ATM90E36_REGISTER_SAENERGYT = 0x90;  // Total Apparent Energy
static const uint16_t ATM90E36_REGISTER_SENERGYA = 0x91;   // A Apparent Energy
static const uint16_t ATM90E36_REGISTER_SENERGYB = 0x92;   // B Apparent Energy
static const uint16_t ATM90E36_REGISTER_SENERGYC = 0x93;   // C Apparent Energy
static const uint16_t ATM90E36_REGISTER_SVENERGYT = 0X94;  // (Vector Sum) Total Apparent Energy

static const uint16_t ATM90E36_REGISTER_ENSTATUS0 = 0X95;  // Metering Status 0
static const uint16_t ATM90E36_REGISTER_ENSTATUS1 = 0X96;  // Metering Status 1
///////////////// 0x97	    // Reserved Register 
static const uint16_t ATM90E36_REGISTER_SVMEANT = 0X98;    // (Vector Sum) Total Apparent Power
static const uint16_t ATM90E36_REGISTER_SVMEANTLSB = 0X99; // lSB of (Vector Sum) Total Apparent Power

/* FUNDAMENTAL / HARMONIC ENERGY REGISTERS */
static const uint16_t ATM90E36_REGISTER_APENERGYTF = 0xA0;  // Total Forward Fund. Energy
static const uint16_t ATM90E36_REGISTER_APENERGYAF = 0xA1;  // A Forward Fund. Energy
static const uint16_t ATM90E36_REGISTER_APENERGYBF = 0xA2;  // B Forward Fund. Energy
static const uint16_t ATM90E36_REGISTER_APENERGYCF = 0xA3;  // C Forward Fund. Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYTF = 0xA4;  // Total Reverse Fund Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYAF = 0xA5;  // A Reverse Fund. Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYBF = 0xA6;  // B Reverse Fund. Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYCF = 0xA7;  // C Reverse Fund. Energy
static const uint16_t ATM90E36_REGISTER_APENERGYTH = 0xA8;  // Total Forward Harm. Energy
static const uint16_t ATM90E36_REGISTER_APENERGYAH = 0xA9;  // A Forward Harm. Energy
static const uint16_t ATM90E36_REGISTER_APENERGYBH = 0xAA;  // B Forward Harm. Energy
static const uint16_t ATM90E36_REGISTER_APENERGYCH = 0xAB;  // C Forward Harm. Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYTH = 0xAC;  // Total Reverse Harm. Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYAH = 0xAD;  // A Reverse Harm. Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYBH = 0xAE;  // B Reverse Harm. Energy
static const uint16_t ATM90E36_REGISTER_ANENERGYCH = 0xAF;  // C Reverse Harm. Energy

/* POWER & P.F. REGISTERS */
static const uint16_t ATM90E36_REGISTER_PMEANT = 0xB0;   // Phase Active Total (P)
static const uint16_t ATM90E36_REGISTER_PMEAN = 0xB1;    // Phase A Active Power start for loop (P)
static const uint16_t ATM90E36_REGISTER_PMEANA = 0xB1;   // Phase A Active Power (P)
static const uint16_t ATM90E36_REGISTER_PMEANB = 0xB2;   // Phase B Active Power (P)
static const uint16_t ATM90E36_REGISTER_PMEANC = 0xB3;   // Phase C Active Power (P)
static const uint16_t ATM90E36_REGISTER_QMEANT = 0xB4;   // Potencia Media Total (Q)
static const uint16_t ATM90E36_REGISTER_QMEAN = 0xB5;    // Reactive Power all Phases (Q)
static const uint16_t ATM90E36_REGISTER_QMEANA = 0xB5;   // Reactive Power A (Q)
static const uint16_t ATM90E36_REGISTER_QMEANB = 0xB6;   // Reactive Power B (Q)
static const uint16_t ATM90E36_REGISTER_QMEANC = 0xB7;   // Reactive Power C (Q)
static const uint16_t ATM90E36_REGISTER_SMEANT = 0xB8;   // Apparent Power All Phases (S)
static const uint16_t ATM90E36_REGISTER_SMEANA = 0xB9;   // Apparent Power A (S)
static const uint16_t ATM90E36_REGISTER_SMEANB = 0xBA;   // Apparent Power B (S)
static const uint16_t ATM90E36_REGISTER_SMEANC = 0xBB;   // Apparent Power C (S)
static const uint16_t ATM90E36_REGISTER_PFMEANT = 0xBC;  // Total Power Factor all Phases
static const uint16_t ATM90E36_REGISTER_PFMEAN = 0xBD;   // Power Factor Start for loop
static const uint16_t ATM90E36_REGISTER_PFMEANA = 0xBD;  // Power Factor A
static const uint16_t ATM90E36_REGISTER_PFMEANB = 0xBE;  // Power Factor B
static const uint16_t ATM90E36_REGISTER_PFMEANC = 0xBF;  // Power Factor C

static const uint16_t ATM90E36_REGISTER_PMEANTLSB = 0xC0;   // Lower Word (Tot. Act. Power)
static const uint16_t ATM90E36_REGISTER_PMEANLSB = 0xC1;    // Lower Word Reg Base (Active Power)
static const uint16_t ATM90E36_REGISTER_PMEANALSB = 0xC1;   // Lower Word (A Act. Power)
static const uint16_t ATM90E36_REGISTER_PMEANBLSB = 0xC2;   // Lower Word (B Act. Power)
static const uint16_t ATM90E36_REGISTER_PMEANCLSB = 0xC3;   // Lower Word (C Act. Power)
static const uint16_t ATM90E36_REGISTER_QMEANTLSB = 0xC4;   // Lower Word (Tot. React. Power)
static const uint16_t ATM90E36_REGISTER_QMEANLSB = 0xC5;    // Lower Word Reg Base (Reactive Power)
static const uint16_t ATM90E36_REGISTER_QMEANALSB = 0xC5;   // Lower Word (A React. Power)
static const uint16_t ATM90E36_REGISTER_QMEANBLSB = 0xC6;   // Lower Word (B React. Power)
static const uint16_t ATM90E36_REGISTER_QMEANCLSB = 0xC7;   // Lower Word (C React. Power)
static const uint16_t ATM90E36_REGISTER_SAMEANTLSB = 0xC8;  // Lower Word (Tot. App. Power)
static const uint16_t ATM90E36_REGISTER_SMEANALSB = 0xC9;   // Lower Word (A App. Power)
static const uint16_t ATM90E36_REGISTER_SMEANBLSB = 0xCA;   // Lower Word (B App. Power)
static const uint16_t ATM90E36_REGISTER_SMEANCLSB = 0xCB;   // Lower Word (C App. Power)

/* FUND/HARM POWER & V/I RMS REGISTERS */
static const uint16_t ATM90E36_REGISTER_PMEANTF = 0xD0;  // Total Active Fund. Power
static const uint16_t ATM90E36_REGISTER_PMEANAF = 0xD1;  // A Active Fund. Power
static const uint16_t ATM90E36_REGISTER_PMEANBF = 0xD2;  // B Active Fund. Power
static const uint16_t ATM90E36_REGISTER_PMEANCF = 0xD3;  // C Active Fund. Power
static const uint16_t ATM90E36_REGISTER_PMEANTH = 0xD4;  // Total Active Harm. Power
static const uint16_t ATM90E36_REGISTER_PMEANH = 0xD5;   // Active Harm. Power Reg Base
static const uint16_t ATM90E36_REGISTER_PMEANAH = 0xD5;  // A Active Harm. Power
static const uint16_t ATM90E36_REGISTER_PMEANBH = 0xD6;  // B Active Harm. Power
static const uint16_t ATM90E36_REGISTER_PMEANCH = 0xD7;  // C Active Harm. Power
static const uint16_t ATM90E36_REGISTER_URMS = 0xD9;     // RMS Voltage Reg Base
static const uint16_t ATM90E36_REGISTER_URMSA = 0xD9;    // A RMS Voltage
static const uint16_t ATM90E36_REGISTER_URMSB = 0xDA;    // B RMS Voltage
static const uint16_t ATM90E36_REGISTER_URMSC = 0xDB;    // C RMS Voltage
static const uint16_t ATM90E36_REGISTER_IRMS = 0xDD;     // RMS Current Reg Base
static const uint16_t ATM90E36_REGISTER_IRMSA = 0xDD;    // A RMS Current
static const uint16_t ATM90E36_REGISTER_IRMSB = 0xDE;    // B RMS Current
static const uint16_t ATM90E36_REGISTER_IRMSC = 0xDF;    // C RMS Current
static const uint16_t ATM90E36_REGISTER_IRMSN = 0xD8;    // Calculated N RMS Current

static const uint16_t ATM90E36_REGISTER_PMEANTFLSB = 0xE0;  // Lower Word (Tot. Act. Fund. Power)
static const uint16_t ATM90E36_REGISTER_PMEANAFLSB = 0xE1;  // Lower Word (A Act. Fund. Power)
static const uint16_t ATM90E36_REGISTER_PMEANBFLSB = 0xE2;  // Lower Word (B Act. Fund. Power)
static const uint16_t ATM90E36_REGISTER_PMEANCFLSB = 0xE3;  // Lower Word (C Act. Fund. Power)
static const uint16_t ATM90E36_REGISTER_PMEANTHLSB = 0xE4;  // Lower Word (Tot. Act. Harm. Power)
static const uint16_t ATM90E36_REGISTER_PMEANHLSB = 0xE5;   // Lower Word (A Act. Harm. Power) Reg Base
static const uint16_t ATM90E36_REGISTER_PMEANAHLSB = 0xE5;  // Lower Word (A Act. Harm. Power)
static const uint16_t ATM90E36_REGISTER_PMEANBHLSB = 0xE6;  // Lower Word (B Act. Harm. Power)
static const uint16_t ATM90E36_REGISTER_PMEANCHLSB = 0xE7;  // Lower Word (C Act. Harm. Power)
///////////////// 0xE8	    // Reserved Register 
static const uint16_t ATM90E36_REGISTER_URMSLSB = 0xE9;     // Lower Word RMS Voltage Reg Base
static const uint16_t ATM90E36_REGISTER_URMSALSB = 0xE9;    // Lower Word (A RMS Voltage)
static const uint16_t ATM90E36_REGISTER_URMSBLSB = 0xEA;    // Lower Word (B RMS Voltage)
static const uint16_t ATM90E36_REGISTER_URMSCLSB = 0xEB;    // Lower Word (C RMS Voltage)
///////////////// 0xEC	    // Reserved Register	
static const uint16_t ATM90E36_REGISTER_IRMSLSB = 0xED;     // Lower Word RMS Current Reg Base
static const uint16_t ATM90E36_REGISTER_IRMSALSB = 0xED;    // Lower Word (A RMS Current)
static const uint16_t ATM90E36_REGISTER_IRMSBLSB = 0xEE;    // Lower Word (B RMS Current)
static const uint16_t ATM90E36_REGISTER_IRMSCLSB = 0xEF;    // Lower Word (C RMS Current)

/* THD, FREQUENCY, ANGLE & TEMPTEMP REGISTERS*/
static const uint16_t ATM90E36_REGISTER_UPEAKA = 0xF1;   // A Voltage Peak
static const uint16_t ATM90E36_REGISTER_UPEAKB = 0xF2;   // B Voltage Peak
static const uint16_t ATM90E36_REGISTER_UPEAKC = 0xF3;   // C Voltage Peak
//////////////// 0xF4	    // Reserved Register	
static const uint16_t ATM90E36_REGISTER_IPEAK = 0xF5;    // Peak Current Reg Base
static const uint16_t ATM90E36_REGISTER_IPEAKA = 0xF5;   // A Current Peak
static const uint16_t ATM90E36_REGISTER_IPEAKB = 0xF6;   // B Current Peak
static const uint16_t ATM90E36_REGISTER_IPEAKC = 0xF7;   // C Current Peak
static const uint16_t ATM90E36_REGISTER_FREQ = 0xF8;     // Frequency
static const uint16_t ATM90E36_REGISTER_PANGLE = 0xF9;   // Mean Phase Angle Reg Base
static const uint16_t ATM90E36_REGISTER_PANGLEA = 0xF9;  // A Mean Phase Angle
static const uint16_t ATM90E36_REGISTER_PANGLEB = 0xFA;  // B Mean Phase Angle
static const uint16_t ATM90E36_REGISTER_PANGLEC = 0xFB;  // C Mean Phase Angle
static const uint16_t ATM90E36_REGISTER_TEMP = 0xFC;     // Measured Temperature
static const uint16_t ATM90E36_REGISTER_UANGLEA = 0xFD;  // A Voltage Phase Angle
static const uint16_t ATM90E36_REGISTER_UANGLEB = 0xFE;  // B Voltage Phase Angle
static const uint16_t ATM90E36_REGISTER_UANGLEC = 0xFF;  // C Voltage Phase Angle

}  // namespace atm90e32
}  // namespace esphome
