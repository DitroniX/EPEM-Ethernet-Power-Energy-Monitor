/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Libraries
#include <SPI.h>
#include <Wire.h>
#include <driver/adc.h>
#include <ATM90E3x.h>
#include <GyverOLED.h>
#include <Adafruit_NeoPixel.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Application
String AppVersion = "260529"; // Internal Firmware Date Code

// App USER
String LocationName = "Test"; // Enter Name Location of Device such as House, Solar etc.  Used for Serial Monitor and OLED.

// Constants USER
int VoltageRawFactor = 0;                // ADC Raw Adjustment for 2048 @ 1.65V Default 0
float VoltageFactor = 428;               // Adjust as needed for Calculated ADC Voltage Output. This used for the DC Sensing Voltage.  Default 428
const int AverageSamples = 25;           // Average Multi-Samples within Calculate Average Value and Reduce Jitter.  Default 25
const int AverageDelay = 20;             // Average Multi-Sample Delay within Calculate Average Value and Reduce Jitter. Default 20
boolean EnableAveraging = true;          // Set to true to enable averaging (ESP32 DCV) within Calculate Average Value and Reduce Jitter.  Default true
boolean EnableNoiseFilterSquelch = true; // This realtes to NoiseFilterSquelch Threshold.  false returns raw values.  Default true.

// Constants USER
const int LoopDelay = 1;                         // Loop Delay in Seconds.  Default 1.
boolean EnableBasicLoop = false;                  // Set to true to display, in Serial Monitor, loop readings and displaying only one per reset cycle.  Default false.
boolean EnableDisplayBoardConfiguration = false; // Set to true to display, in Serial Monitor, board software configuration Information if DisplayFull is true. Default true.
boolean EnableOLEDLoop = true;                   // Set to true to enable OLED Display in Loop.  Over-ride via I2C Scan.  Check OLED Instance below, for OLED Selection.  Default true.

// OLED Instance. You will need to select your OLED I2C Display.   Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x32, OLED_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x64, OLED_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled; // 0.96"
// GyverOLED<SSH1106_128x64> oled; // 1.1" 1.3"

// RGB
#define PIN_WS2812B 8 // The ESP32-C6 pin GPIO8 connected to WS2812B
Adafruit_NeoPixel ws2812b(1, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// Hardware Serial 1 pins - RS485
#define RXLP 4
#define TXLP 5
#define ENLP 23 // RS485 UART Enable

// OUTPUTS
#define FET 2      // FET
#define INT_ETH 15 // INT_ETH

// App
String AppName;    // App Name - Do Not Change
String AppAcronym; // Board Acronym - Do Not Change

// Board Auto Configuration - Automatic Selection
boolean OLED_Enabled = false; // Auto Enabled if OLED Detected on I2C Scan- Do Not Change

// Constants Application
uint64_t chipid = ESP.getEfuseMac(); // Get ChipID (essentially the MAC address)

// **************** ATM90Ex CALIBRATION SETTINGS GUIDE ****************
// LineFreq = 389 for 50 Hz (World)  4485 for (North America)
// VoltageGain = Depends on Transformer used for Voltage Isolation.  Below values are based on Greenbrook DAT01A (TC TR7) Transformer, set to 12V AC.  https://www.tlc-direct.co.uk/Products/TCTR7.html
// CurrentGainCTx = Depends on CT Clamps Used.  You can sue different on each channel is needed.  Below based on SCT-013-000 100A/50mA

#if ATM90DEVICE == ATM90E32_DEVICE

// The below related to calibration of Voltage inputs (V1, V2 and V3), also Current Clamp inputs (CT1, CT2 and CT3 only).

// Mains Frequency
unsigned short LineFreq = 389; // MMode0 0x33 _lineFreq | Default 389

// Voltage and Current - Overall Gain
// PMPGA 0x17              DDV3V2V1I4I3I2I1  // D = DPGA       4/3/2/1 = V1-4 = I1-4
unsigned short PGAGain = 0b0101010101111111; // PMPGA 0x17  | DPGA Gain = 2 and PGA Gain = 1

// Voltage.
// This is calculated based on the Bell Transformer DAT01 on 12V setting @ ~19V RMS.  Need to allow for +/- ~ 1% Tolerance.
// Calculations: Base value for 240V is 38800.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
// Calculations: Base value for 120V is 20200.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
#if ATM_SINGLEVOLTAGE == true
unsigned short VoltageGain1 = 38300;        // uGain = UgainA | 0x61	0x0002 40500 20000 42620 (10000 = ~60V) Default 38300
unsigned short VoltageGain2 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
unsigned short VoltageGain3 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
#else
unsigned short VoltageGain1 = 38800; // uGain = UgainA | 38800 Default Starting Value
unsigned short VoltageGain2 = 38800; // uGain = UgainB | 38800 Default Starting Value
unsigned short VoltageGain3 = 38800; // uGain = UgainC | 38800 Default Starting Value
#endif

// Current
// This is calculated based on the YDH? 100A/50mA  Need to allow for +/- ~ 1% Tolerance.  Trim values are fine!
// Calculations: Base value for CT100/50 Mid Range 38500.  To increase/decrease change by Approx. ~500 per 0.01A RMS
unsigned short CurrentGainCT1 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
unsigned short CurrentGainCT2 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
unsigned short CurrentGainCT3 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
#endif

#if ATM90DEVICE == ATM90E36_DEVICE

// The below related to calibration of Voltage inputs (V1, V2 and V3), also Current Clamp inputs (CT1, CT2 and CT3 only).

// Mains Frequency
unsigned short LineFreq = 389; // MMode0 0x33 _lineFreq | 389

// Voltage and Current - Overall Gain
// PMPGA 0x17              DDV3V2V1I4I3I2I1  // D = DPGA       4/3/2/1 = V1-4 = I1-4
unsigned short PGAGain = 0b0101010101010101; // PMPGA 0x17  | DPGA Gain = 2 and PGA Gain = 1

// Voltage.
// This is calculated based on the DAT01 on 12V setting @ ~19V RMS.  Need to allow for +/- ~ 1% Tolerance.
// Calculations: Base value for 240V is 20200.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
// Calculations: Base value for 120V is 9700.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
#if ATM_SINGLEVOLTAGE == true
unsigned short VoltageGain1 = 20200;        // uGain = UgainA | 20200 Default Starting Value
unsigned short VoltageGain2 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
unsigned short VoltageGain3 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
#else
unsigned short VoltageGain1 = 20200; // uGain = UgainA | 20200 Default Starting Value
unsigned short VoltageGain2 = 20200; // uGain = UgainB | 20200 Default Starting Value
unsigned short VoltageGain3 = 20200; // uGain = UgainC | 20200 Default Starting Value
#endif

// Current
// This is calculated based on the YDH? 100A/50mA  Need to allow for +/- ~ 1% Tolerance.  Trim values are fine!
// Calculations: Base value for CT100/50 Mid Range 33500.  To increase/decrease change by Approx. ~500 per 0.01A RMS
unsigned short CurrentGainCT1 = 33500; // IgainA 0x62
unsigned short CurrentGainCT2 = 33500; // IgainA 0x65
unsigned short CurrentGainCT3 = 33500; // IgainA 0x6A
unsigned short CurrentGainCTN = 33500; // IgainA 0x6E
#endif

// Variables
float TemperatureC;      // Temperature
float TemperatureF;      // Temperature
int VoltageSensorRaw;    // ADC Raw Voltage Value
float VoltageCalculated; // Calculated Voltage Value
int VoltagePercentage;   // Voltage Percentage
int OLEDCount;           // OLED Information Counter

// Variables ATM
float LineVoltage1, LineVoltage2, LineVoltage3, LineVoltageTotal, LineVoltageAverage;
float LineCurrentCT1, LineCurrentCT2, LineCurrentCT3, LineCurrentCT4, LineCurrentCTN, LineCurrentTotal;
float CalculatedPowerCT1, CalculatedPowerCT2, CalculatedPowerCT3, CalculatedPowerCT4, CalculatedPowerCTN, CalculatedTotalPower;
float ActivePowerCT1, ActivePowerCT2, ActivePowerCT3, TotalActivePower, CalculatedTotalActivePower;
float ActivePowerImportCT1, ActivePowerImportCT2, ActivePowerImportCT3, TotalActivePowerImport;
float ActivePowerExportCT1, ActivePowerExportCT2, ActivePowerExportCT3, TotalActivePowerExport;
float ReactivePowerCT1, ReactivePowerCT2, ReactivePowerCT3, TotalReactivePower, CalculatedTotalReactivePower;
float ApparentPowerCT1, ApparentPowerCT2, ApparentPowerCT3, TotalApparentPower, CalculatedTotalApparentPower;
float TotalActiveFundPower, TotalActiveHarPower;
float PowerFactorCT1, PowerFactorCT2, PowerFactorCT3, TotalPowerFactor;
float PhaseAngleCT1, PhaseAngleCT2, PhaseAngleCT3;
float VoltageHarmonicsCT1, VoltageHarmonicsCT2, VoltageHarmonicsCT3;
float CurrentHarmonicsCT1, CurrentHarmonicsCT2, CurrentHarmonicsCT3;
float ChipTemperature, LineFrequency;

// Variables PCB
float DCVoltage;      // DCV Input Voltage
float PCBTemperature; // PCB NTC Temperature

// **************** Initialize ATM90E32 or ATM90E36 IC Class ****************
ATM90E3x eic{}; //

// **************** ESP32 INPUTS ****************
#define ATM_WO 1
#define DCV_IN 3
#define User_Button 9

// **************** ESP32 OUTPUTS ****************
#define LED_Red 22 // Red LED

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// **************** FUNCTIONS AND ROUTINES ****************

// Print Text to OLED.  Font 1-4.
void OLEDPrint(String TextS, int FontSize = 2, int PosY = 0, boolean FontCentre = true, float DisplayR = 0.6)
{
  int PosX;
  oled.setScale(FontSize);

  if (FontCentre == true)
  {

    // Font Fudge Factor
    unsigned int ValueCount = TextS.length() / 2;

    switch (FontSize)
    {
    case 1:                             // Small Font
      PosX = (58 - ((ValueCount * 5))); // Approximate Middle (Default 5)
      break;
    case 2:                              // Medium Font
      PosX = (58 - ((ValueCount * 10))); // Approximate Middle (Default 10)
      break;
    case 3:                              // Bit Larger Font
      PosX = (58 - ((ValueCount * 17))); // Approximate Middle (Default 17)
      PosY = 0;                          // Y can be 0 or 1.  Leave at 0.
      break;
    case 4:                              // Large Font
      PosX = (58 - ((ValueCount * 23))); // Approximate Middle (Default 23)
      PosY = 0;                          // Y needs to be 0.
      break;
    default:
      PosX = (58 - ((ValueCount * 5))); // Approximate Middle
      FontSize = 1;
      PosY = 0;
      break;
    }

    if (PosX < 0)
      PosX = 0;
  }

  // Hopefully Display Something Meaningful... If not, 42 is a Good Number.
  oled.setCursor(PosX, PosY);
  oled.print(TextS);
} // OLEDPrint

// Initialise OLED
void InitialiseOLED()
{
  if (OLED_Enabled == true || OLED_Enabled == true && EnableOLEDLoop == true)
  {
    Serial.println("\nInitialising OLED Display");

    oled.init();
    oled.clear();

    if (AppAcronym.length() <= 4)
    {
      OLEDPrint(AppAcronym, 4, 0);
    }
    else
    {
      OLEDPrint(AppAcronym, 2, 0);
    }

#if ATM90DEVICE == ATM90E32_DEVICE
    OLEDPrint("ATM90E32", 2, 2);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
    OLEDPrint("ATM90E36", 2, 2);
#endif

    oled.update();
    delay(2000);

    // App and Firmware Version
    oled.clear();
    OLEDPrint(AppAcronym, 2, 0);
    OLEDPrint("v " + AppVersion, 2, 2);
    oled.update();
    delay(2000);

    // App and Location
    oled.clear();
    OLEDPrint(AppAcronym, 2, 0);
    OLEDPrint(LocationName, 2, 2);
    oled.update();
    delay(2000);

    // App and Starting...
    oled.clear();
    OLEDPrint(AppAcronym, 2, 0);
    OLEDPrint("Starting..", 2, 2);
    oled.update();
    delay(500);
  }
} // InitialiseOLED

// RGB
void RGB_Test()
{

  // Turn off RGB for two seconds
  ws2812b.clear();
  ws2812b.show();
  delay(250);

  // RED
  ws2812b.setPixelColor(0, ws2812b.Color(150, 0, 0));
  ws2812b.show();
  delay(250);

  // Turn off RGB for two seconds
  ws2812b.clear();
  ws2812b.show(); // update to the WS2812B Led Strip
  delay(250);

  // BLUE
  ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 150));
  ws2812b.show();
  delay(250);

  // Turn off RGB for two seconds
  ws2812b.clear();
  ws2812b.show();
  delay(250);

  // GREEN
  ws2812b.setPixelColor(0, ws2812b.Color(0, 150, 0));
  ws2812b.show();
  delay(250);

  ws2812b.clear();
  ws2812b.show();
} // RGB

// Display BIN from Var
void DisplayBIN16(int var)
{
  for (unsigned int i = 0x8000; i; i >>= 1)
  {
    Serial.write(var & i ? '1' : '0');
  }
  Serial.print(" ");
} // DisplayBIN16

// Display Hex from Var
void DisplayHEX(unsigned long var, unsigned char numChars)
{
  unsigned long mask = 0x0000000F;
  mask = mask << 4 * (numChars - 1);

  for (unsigned int i = numChars; i > 0; --i)
  {
    Serial.print(((var & mask) >> (i - 1) * 4), HEX);
    mask = mask >> 4;
  }
  Serial.print(" ");
} // DisplayHEX

// Return Rounded Value to 2 decimals and Remove +/- Noise Threshold.  Default 0.02
float NoiseFilterSquelch(float Value, float Threshold = 0.02, boolean AllowNegative = true, float ValueMax = 0)
{
  if (EnableNoiseFilterSquelch == true) // If false, output is raw value
  {
    Value = roundf(Value * 100) / 100; // Round to 2 Decimal Places

    if (Value >= -Threshold && Value <= Threshold) // Window Comparator / Squelch.  Default 0.2
      Value = 0;

    if (AllowNegative == false && Value < 0) // Negative Value Filter.  Default true
      Value = 0;

    if (ValueMax != 0 && Value > ValueMax) // Maximum Filter.  Default 0
      Value = 0;
  }

  return Value;
} // NoiseFilterSquelch

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max)
{
  return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
}

// Calculate Average Value and Reduce Jitter
int CalculateADCAverage(int SensorChannel)
{
  int AverageRAW = 0;
  if (EnableAveraging == true)
  {

    for (int i = 0; i < AverageSamples; i++)
    {
      AverageRAW = AverageRAW + analogRead(SensorChannel);
      delay(AverageDelay);
    }
    AverageRAW = AverageRAW / AverageSamples;
    if (AverageRAW < 2)
      AverageRAW = 0;
  }
  else
  {
    AverageRAW = analogRead(SensorChannel);
  }
  return AverageRAW;
} // CalculateADCAverage

// Rough and Ready Underline Text.
void PrintUnderline(String sText)
{
  int count = 0;
  Serial.println(sText);

  while (count <= sText.length())
  {
    Serial.print("─");
    count++;
  }
  Serial.println("");
} // PrintUnderline

// Equally Rough and Ready Dash Separator
void PrintSeparator(String sText)
{
  int count = 0;

  while (count <= (sText.length() / 2) + 1)
  {
    Serial.print("- ");
    count++;
  }
  Serial.println("");

  Serial.println(sText);
} // PrintSeparator

// I2C Bus Scanner
void ScanI2CBus()
{

  byte error, address;
  int nDevices;

  Serial.println("Scanning I2C Bus for Devices for Auto Configuration ...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    nDevices++;
    if (nDevices == 1)
      Serial.println(" I2C Possible Device(s) found at Address:");
    if (error == 0)
    {
      Serial.print(" * Hex 0x");
      Serial.print(address, HEX);

      Serial.print(" (");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(")\t");

      switch (address)
      {
      case 0x18 ... 0x1F:
        Serial.print(" MCP9808 Temerature Sensor");
        break;
      case 0x20 ... 0x27:
        Serial.print(" GPIO Expander");
        break;
      case 0x28 ... 0x2B:
        Serial.print(" Digital Pot");
        break;
      case 0x38:
        Serial.print(" Humidity/Temperature Sensor");
        break;
      case 0x40 ... 0x47:
        Serial.print(" PCA9685 12-bit PWM Driver");
        break;
      case 0x48 ... 0x49:
        Serial.print(" ADS1115 16-bit ADC");
        break;
      case 0x4B:
        Serial.print(" ADS1115 16-bit ADC");
        break;
      case 0x4A:
        Serial.print(" TMP102 Temperature Sensor");
        break;
      case 0x4C ... 0x4F:
        Serial.print(" Sensor");
        break;
      case 0x3C ... 0x3D:
        Serial.print(" OLED");
        OLED_Enabled = true;
        break;
      case 0x50 ... 0x5F:
        Serial.print(" EEPROM");
        break;
      case 0x60 ... 0x67:
        Serial.print(" MCP4728 12-bit DAC");
        break;
      case 0x68:
        Serial.print(" DS3231SN Accurate RTC");
        break;
      case 0x70:
        break;
      case 0x76 ... 0x77:
        Serial.print(" BMP/BME Temerature/Humidity/Barometric");
        break;
      }
      Serial.println();
    }
    else if (error == 4)
    {
      Serial.print(" * Unknown error at address Decimal ");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(" = Hex 0x");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println(" * No I2C devices found. Possible Hardware Issue?");
  }
  else
  {
    Serial.println(" I2C Bus Scan Complete\n");
  }
} // ScanI2CBus

// Test RGB LEDs
void TestRGB()
{
  Serial.println("Testing LED");

  // Red
  digitalWrite(LED_Red, HIGH);
  delay(500);
  digitalWrite(LED_Red, LOW);
  delay(100);
}

void ConfigureBoard()
{
  Serial.println("Scanning and Configuring Board.. Please Wait..\n");

  pinMode(User_Button, INPUT_PULLUP);

  // Configure GPIO Outputs
  pinMode(LED_Red, OUTPUT);

  // Initialise FET on GP2
  pinMode(FET, OUTPUT);

  // Initialise RS485 UART Enable on GP23
  pinMode(ENLP, OUTPUT);

  // Initialise Ethernet /INT on GP15
  pinMode(INT_ETH, OUTPUT);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Display Hardware Options
  PrintUnderline("Hardware Options and Auto Configuration");

  // Scan I2C Bus and Report Devices
  ScanI2CBus();

  // Auto Configure GPIO Inputs
  pinMode(ATM_WO, INPUT);

  // Header Configure

  AppAcronym = "EPEM E32";
  AppName = AppAcronym + " IoT Power Energy Monitor Controller";

  Serial.println(AppName + "\n");

  Serial.println(AppAcronym + " Register Status and Startup Report");
  Serial.printf("ESP32-C6 Serial ID: %04X", (uint16_t)(chipid >> 32));
  Serial.printf("%08X", (uint32_t)chipid);
  Serial.println("");
  Serial.print("FLEX Lite for EPEM E32 Firmware Version: ");
  Serial.println(AppVersion);
  Serial.print("Board Location: ");
  Serial.println(LocationName);

  // Initialize OLED
  InitialiseOLED();

} // ConfigureBoard

// Display Board Configuration
void DisplayBoardConfiguration()
{

  // Header
  AppAcronym = "EPEM E32";
  AppName = AppAcronym + " IoT Power Energy Monitor Controller";

  Serial.println(AppName + "\n");

  Serial.println(AppAcronym + " Register Status and Startup Report");
  Serial.printf("ESP32-C6 Serial ID: %04X", (uint16_t)(chipid >> 32));
  Serial.printf("%08X", (uint32_t)chipid);
  Serial.println("");
  Serial.print("FLEX Lite for EPEM E32 Firmware Version: ");
  Serial.println(AppVersion);
  Serial.print("Location: ");
  Serial.println(LocationName);
  Serial.println("");

  // Stabalise
  delay(100);

// Display Hardware Configuration for ATM90E32 ATM90E36
#if ATM90DEVICE == ATM90E32_DEVICE
  PrintUnderline("Hardware Configuration for ATM90E32");
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  PrintUnderline("Hardware Configuration for ATM90E36");
#endif

#if ATM_SINGLEVOLTAGE == true
  Serial.println("AC Voltage Inputs:\tSingle Input V1 will be used for Current Phase Calculations");
#else
#if ATM_SPLITPHASE == false // World Single, Three Phase or 3 x Single Phases
  Serial.println("AC Voltage Inputs:\tMulti Separated V1 V2 V3 - 3 Phase, or 3 x Single Phase Configuration.");
#else                       // USA Split Phase 120+120
  Serial.println("AC Voltage Inputs:\tMulti Dual V1 and V3 - USA Configuration");
#endif
#endif

#if ATM_SPLITPHASE == true
  // USA 120+120
  Serial.println("Split AC Voltage:\tDual Input V1 + V3 Enabled - USA Configuration");
  Serial.println("CT Current Clamps:\tPhase Configured for 1 and 3");
#else
  // World
  Serial.println("Split AC Voltage:\tDual  or Split Voltage Input Disabled");
  Serial.println("CT Current Clamps:\tConfigured for 1, 2, 3 Phase");
#endif

  Serial.println("");

  // Registers
  String SysStatus0_Value = String(eic.GetSysStatus0(), HEX); // EMMState0
  SysStatus0_Value.toUpperCase();
  String SysStatus1_Value = String(eic.GetSysStatus1(), HEX); // EMMState1
  SysStatus1_Value.toUpperCase();
  String MeterStatus0_Value = String(eic.GetMeterStatus0(), HEX); // EMMIntState0
  MeterStatus0_Value.toUpperCase();
  String MeterStatus1_Value = String(eic.GetMeterStatus1(), HEX); // EMMIntState1
  MeterStatus1_Value.toUpperCase();

  Serial.println("[Status Values]");
  Serial.println("System Status: \t\tS0:0x" + SysStatus0_Value + "\t\tS1:0x" + SysStatus1_Value);
  Serial.println("Meter Status:  \t\tE0:0x" + MeterStatus0_Value + "\t\tE1:0x" + MeterStatus1_Value);

  // Stabalise
  delay(100);

  // if true the MCU is not getting data from the energy meter
  unsigned short sys0 = eic.GetSysStatus0(); // EMMState0
  if (sys0 == 65535 || sys0 == 0)
  {
    Serial.println("");
    Serial.println("*********************************************************************");
    Serial.print("* Error: Not Correct Data from ATM90E");
    Serial.print(ATM90DEVICE);
    Serial.println(" Energy Meter - Check Device *");
    Serial.println("*********************************************************************");
  }

  Serial.println("");
} // DisplayBoardConfiguration

// Read PCB Thermister
void ReadPCBTemperature()
{

} // ReadPCBTemperature

// Read DCV_IN via VP
void ReadADCVoltage()
{
  // Raw ADC Voltage Output to VoltageSensorRaw
  VoltageSensorRaw = CalculateADCAverage(DCV_IN);
  Serial.print("ADC Raw: ");
  Serial.print(VoltageSensorRaw);
  Serial.print(" > ");

  // Adjust Raw ADC Voltage
  VoltageSensorRaw = VoltageSensorRaw + VoltageRawFactor;
  Serial.print("ADC Adjusted: ");
  Serial.print(VoltageSensorRaw);
  Serial.print(" > ");

  // Calculated ADC Voltage Output
  VoltageCalculated = ((VoltageFactor / 4095) * VoltageSensorRaw);
  Serial.print("Calculated: ");
  Serial.print(VoltageCalculated);
  Serial.println(" V DC");

  DCVoltage = VoltageCalculated;
  Serial.println();
} // ReadADCVoltage
