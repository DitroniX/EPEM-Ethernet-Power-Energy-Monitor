/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Libraries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <driver/adc.h>
#include <ATM90E3x.h>
#include <Hardware.h>

// USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS -> Moved to EPEM_Hardware

// **************** FUNCTIONS AND ROUTINES ****************

// Return Register Values from the ATM90E32, or ATM90E36, - and Display
void DisplayRegisters(boolean DisplayFull = true)
{
  // if (DisplayFull == false) // Display Expanded Information
  //   Serial.println("\nDisplay Output (DisplayRegisters false), Set to Minimised");

  // **************** VOLTAGE ****************

  // Get Mains Voltages
  LineVoltage1 = NoiseFilterSquelch(eic.GetLineVoltage1(), 1);
  LineVoltage2 = NoiseFilterSquelch(eic.GetLineVoltage2(), 1);
  LineVoltage3 = NoiseFilterSquelch(eic.GetLineVoltage3(), 1);
  LineVoltageAverage = 0;
  LineVoltageTotal = 0;

// Calculate Voltage
#if ATM_SINGLEVOLTAGE == true && ATM_SPLITPHASE == true
  Serial.println("* SOFTWARE CONFIGURATION ERROR *\n* You cannot have ATM_SINGLEVOLTAGE and ATM_SPLITPHASE, both Enabled.\n");
#else
#if ATM_SPLITPHASE == true
  LineVoltageTotal = (LineVoltage1 + LineVoltage3) / 2; // Split Single Phase configured, so only 120V per leg
#else
  LineVoltageAverage = (LineVoltage1 + LineVoltage2 + LineVoltage3) / 3; // Voltage should be around 110V, or 220-240, at the AC transformer primary.
#endif
#endif

  if (DisplayFull == true) // Display Expanded Information
  {

    // Mains RMS Voltage
    PrintUnderline("Mains RMS Voltage");

#if ATM_SINGLEVOLTAGE == false

    Serial.println("Voltage 1: " + String(LineVoltage1) + " V");

#if ATM_SPLITPHASE == false
    Serial.println("Voltage 2: " + String(LineVoltage2) + " V");
#endif

    Serial.println("Voltage 3: " + String(LineVoltage3) + " V");

#if ATM_SPLITPHASE == true
    PrintSeparator("Total Split Voltage: " + String(LineVoltageTotal) + " V (V1~X~V3)");
#else
    PrintSeparator("Average Voltage: " + String(LineVoltageAverage) + " V (V1~V2~V3)");
#endif

#else
    PrintSeparator("Voltage 1: " + String(LineVoltage1) + " V (Single Line Voltage Selected)");
#endif
  }

  // **************** CURRENT ****************

  // Get Total Current.  Ignore CT2 if Split Phase
  LineCurrentCT1 = NoiseFilterSquelch(eic.GetLineCurrentCT1());

#if ATM_SPLITPHASE == false
  LineCurrentCT2 = NoiseFilterSquelch(eic.GetLineCurrentCT2());
#else
  LineCurrentCT2 = 0;
#endif

  LineCurrentCT3 = NoiseFilterSquelch(eic.GetLineCurrentCT3());

#if ATM90DEVICE == ATM90E32_DEVICE
  LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3);
#else
  LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3);
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  // E36 Using I4N Input for Current
  LineCurrentCTN = NoiseFilterSquelch(eic.GetLineCurrentCTN());
  LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3);
#else
  LineCurrentCTN = 0;
#endif

  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("");

    // Mains RMS Current
    PrintUnderline("Mains RMS Current");
    Serial.println("Current CT1: " + String(LineCurrentCT1) + " A");

#if ATM_SPLITPHASE == false
    Serial.println("Current CT2: " + String(LineCurrentCT2) + " A");
#endif

    Serial.println("Current CT3: " + String(LineCurrentCT3) + " A");

#if ATM90DEVICE == ATM90E36_DEVICE
    Serial.println("Current CTN: " + String(LineCurrentCTN) + " A");
#endif

#if ATM_SPLITPHASE == true
    Serial.println("Actual Total Current: " + String(LineCurrentTotal) + " A (CT1~X~CT3)");
#else
    PrintSeparator("Actual Total Current: " + String(LineCurrentTotal) + " A (CT1~CT2~CT3)");
#endif

    Serial.println("");
  }

  // **************** POWER ****************

  // Calculated Power CT2ased on V*I
#if ATM_SINGLEVOLTAGE == true // Single Mains Voltage for all Current Measurements
  CalculatedPowerCT1 = LineVoltage1 * LineCurrentCT1;
  CalculatedPowerCT2 = LineVoltage1 * LineCurrentCT2;
  CalculatedPowerCT3 = LineVoltage1 * LineCurrentCT3;

  CalculatedPowerCT4 = LineVoltage1 * LineCurrentCT4;

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Calculated RMS Power");
    Serial.println("Power V1*I1: " + String(CalculatedPowerCT1) + " W\t" + String(CalculatedPowerCT1 / 1000) + " kW");
    Serial.println("Power V1*I2: " + String(CalculatedPowerCT2) + " W\t" + String(CalculatedPowerCT2 / 1000) + " kW");
    Serial.println("Power V1*I3: " + String(CalculatedPowerCT3) + " W\t" + String(CalculatedPowerCT3 / 1000) + " kW");
  }

#if ATM90DEVICE == ATM90E36_DEVICE
  Serial.println("Power V1*I4: " + String(CalculatedPowerCT4) + " W");
#endif
#endif

// Calculate Total Watts
#if ATM_SPLITPHASE == true // USA 120+120
  CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage3 * LineCurrentCT3);

  if (DisplayFull == true) // Display Expanded Information
    PrintSeparator("Calculated Total Power: " + String(CalculatedTotalPower) + " W (CT1~X~CT3)");
#else // World
  CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage2 * LineCurrentCT2) + (LineVoltage3 * LineCurrentCT3);

  if (DisplayFull == true) // Display Expanded Information
    PrintSeparator("Calculated Total Power: " + String(CalculatedTotalPower) + " W (CT1~CT2~CT3)");
#endif

  if (DisplayFull == true) // Display Expanded Information
    Serial.println("");

  // Active Power.  Extra Import/Export values included to provide added detail for Home Automation logging
  String ActiveFlag = "";

  if (DisplayFull == true) // Display Expanded Information
    PrintUnderline("Active Power (Absorbed or Used by the Load)");

  // Active Power CT1 - Always a Postive Number

  ActivePowerCT1 = NoiseFilterSquelch(eic.GetActivePowerCT1(), 0.2);

  ActivePowerImportCT1 = 0;
  ActivePowerExportCT1 = 0;
  ActiveFlag = "";

  if (ActivePowerCT1 > 0)
  {
    ActivePowerImportCT1 = ActivePowerCT1;
    ActiveFlag = "(Import)";
  }

  if (ActivePowerCT1 < 0)
  {
    ActivePowerExportCT1 = -ActivePowerCT1;
    ActiveFlag = "(Export)";
  }

  if (DisplayFull == true) // Display Expanded Information

#if ATM90DEVICE == ATM90E32_DEVICE
    Serial.println("Active Power CT1: " + String(ActivePowerCT1) + " W \t" + ActiveFlag);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
  Serial.println("Active Power CT1: " + String(ActivePowerCT1) + " kW \t" + ActiveFlag);
#endif

  // Active Power CT2 - Always a Postive Number
  ActivePowerCT2 = NoiseFilterSquelch(eic.GetActivePowerCT2(), 0.2);
  ActivePowerImportCT2 = 0;
  ActivePowerExportCT2 = 0;
  ActiveFlag = "";

  if (ActivePowerCT2 > 0)
  {
    ActivePowerImportCT2 = ActivePowerCT2;
    ActiveFlag = "(Import)";
  }

  if (ActivePowerCT2 < 0)
  {
    ActivePowerExportCT2 = -ActivePowerCT2;
    ActiveFlag = "(Export)";
  }
  if (DisplayFull == true) // Display Expanded Information

#if ATM90DEVICE == ATM90E32_DEVICE
    Serial.println("Active Power CT2: " + String(ActivePowerCT2) + " W \t" + ActiveFlag);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
  Serial.println("Active Power CT2: " + String(ActivePowerCT2) + " kW \t" + ActiveFlag);
#endif

  // Active Power CT3 - Always a Postive Number
  ActivePowerCT3 = NoiseFilterSquelch(eic.GetActivePowerCT3(), 0.2);
  ActivePowerImportCT3 = 0;
  ActivePowerExportCT3 = 0;
  ActiveFlag = "";

  if (ActivePowerCT3 > 0)
  {
    ActivePowerImportCT3 = ActivePowerCT3;
    ActiveFlag = "(Import)";
  }

  if (ActivePowerCT3 < 0)
  {
    ActivePowerExportCT3 = -ActivePowerCT3;
    ActiveFlag = "(Export)";
  }

  if (DisplayFull == true) // Display Expanded Information

#if ATM90DEVICE == ATM90E32_DEVICE
    Serial.println("Active Power CT3: " + String(ActivePowerCT3) + " W \t" + ActiveFlag);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
  Serial.println("Active Power CT3: " + String(ActivePowerCT3) + " kW \t" + ActiveFlag);
#endif

  // Total Active Power C@T
  TotalActivePower = NoiseFilterSquelch(eic.GetTotalActivePower(), 0.2);
  ActiveFlag = "";

  TotalActivePowerImport = ((TotalActivePower > 0) ? TotalActivePower : 0);
  TotalActivePowerExport = ((TotalActivePower < 0) ? -TotalActivePower : 0);
  ActiveFlag = ((TotalActivePower > 0) ? "(Import)" : ((TotalActivePower < 0) ? "(Export)" : "(ZERO)"));

  CalculatedTotalActivePower = (ActivePowerCT1 + ActivePowerCT2 + ActivePowerCT3);

  if (DisplayFull == true) // Display Expanded Information
  {

#if ATM90DEVICE == ATM90E32_DEVICE
    PrintSeparator("Total Active Power: " + String(TotalActivePower) + " W \t" + ActiveFlag);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
    PrintSeparator("Total Active Power: " + String(TotalActivePower) + " kW \t" + ActiveFlag);
#endif

    PrintSeparator("Calculated Total Active Power: " + String(CalculatedTotalActivePower) + " W (CT1~CT2~CT3)");
    Serial.println("");
  }

  // Re-Active Power
  ReactivePowerCT1 = NoiseFilterSquelch(eic.GetReactivePowerCT1());
  ReactivePowerCT2 = NoiseFilterSquelch(eic.GetReactivePowerCT2());
  ReactivePowerCT3 = NoiseFilterSquelch(eic.GetReactivePowerCT3());
  TotalReactivePower = NoiseFilterSquelch(eic.GetTotalReactivePower());
  CalculatedTotalReactivePower = (ReactivePowerCT1 + ReactivePowerCT2 + ReactivePowerCT3);

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Re-Active Power");
    Serial.println("ReActive Power CT1: " + String(ReactivePowerCT1) + " VAR");
    Serial.println("ReActive Power CT2: " + String(ReactivePowerCT2) + " VAR");
    Serial.println("ReActive Power CT3: " + String(ReactivePowerCT3) + " VAR");
    PrintSeparator("Total ReActive Power: " + String(TotalReactivePower) + " VAR (CT1~CT2~CT3)");

    PrintSeparator("Calculated Total Reactive Power: " + String(CalculatedTotalReactivePower) + " W (CT1~CT2~CT3)");
    Serial.println("");
  }

  // Apparent Power Flowing - Voltage and Current (V * I)
  ApparentPowerCT1 = NoiseFilterSquelch(eic.GetApparentPowerCT1());
  ApparentPowerCT2 = NoiseFilterSquelch(eic.GetApparentPowerCT2());
  ApparentPowerCT3 = NoiseFilterSquelch(eic.GetApparentPowerCT3());
  TotalApparentPower = NoiseFilterSquelch(eic.GetTotalApparentPower());
  CalculatedTotalApparentPower = (ApparentPowerCT1 + ApparentPowerCT2 + ApparentPowerCT3);

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Apparent Power (Total Amount of Power Flowing from Source to Load)");
    Serial.println("Apparent Power CT1: " + String(ApparentPowerCT1) + " VA");
    Serial.println("Apparent Power CT2: " + String(ApparentPowerCT2) + " VA");
    Serial.println("Apparent Power CT3: " + String(ApparentPowerCT3) + " VA");
    PrintSeparator("Total Apparent Power: " + String(TotalApparentPower) + " VA (CT1~CT2~CT3)");

    PrintSeparator("Calculated Total Apparent Power: " + String(CalculatedTotalApparentPower) + " VA (CT1~CT2~CT3)");
    Serial.println("\n");
  }

#if ATM90DEVICE == ATM90E32_DEVICE
  if (DisplayFull == true) // Display Expanded Information
    PrintUnderline("Other Power Information");

  // Total Fundamental Power
  TotalActiveFundPower = NoiseFilterSquelch(eic.GetTotalActiveFundPower());
  if (DisplayFull == true) // Display Expanded Information
    Serial.println("Total Fundamental Power: " + String(TotalActiveFundPower) + " pH(t)\n");

  // Total Harmonic Power
  TotalActiveHarPower = NoiseFilterSquelch(eic.GetTotalActiveHarPower());
  if (DisplayFull == true) // Display Expanded Information
    Serial.println("Total Harmonic Power: " + String(TotalActiveHarPower) + " ");

#endif

  if (DisplayFull == true)

    // **************** OTHER ****************

    // Power Factor
    PowerFactorCT1 = NoiseFilterSquelch(eic.GetPowerFactorCT1(), 0.01, false);
  PowerFactorCT2 = NoiseFilterSquelch(eic.GetPowerFactorCT2(), 0.01, false);
  PowerFactorCT3 = NoiseFilterSquelch(eic.GetPowerFactorCT3(), 0.01, false);
  TotalPowerFactor = NoiseFilterSquelch(eic.GetTotalPowerFactor(), 0, false);

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Power Factor");
    Serial.println("Power Factor CT1: " + String(PowerFactorCT1) + " PF");
    Serial.println("Power Factor CT2: " + String(PowerFactorCT2) + " PF");
    Serial.println("Power Factor CT3: " + String(PowerFactorCT3) + " PF");
    PrintSeparator("Total Power Factor: " + String(TotalPowerFactor) + " PF");
    Serial.println("");
  }

  // Phase Angle
  PhaseAngleCT1 = NoiseFilterSquelch(eic.GetPhaseCT1(), 0, true, 180);
  PhaseAngleCT2 = NoiseFilterSquelch(eic.GetPhaseCT2(), 0, true, 180);
  PhaseAngleCT3 = NoiseFilterSquelch(eic.GetPhaseCT3(), 0, true, 180);

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Phase Angle");
    Serial.println("Phase Angle CT1: " + String(PhaseAngleCT1));
    Serial.println("Phase Angle CT2: " + String(PhaseAngleCT2));
    Serial.println("Phase Angle CT3: " + String(PhaseAngleCT3));
    Serial.println("");
  }

#if ATM90DEVICE == ATM90E36_DEVICE
  // Voltage Harmonics
  VoltageHarmonicsCT1 = NoiseFilterSquelch(eic.GetVHarmCT1(), 0, true, 180);
  VoltageHarmonicsCT2 = NoiseFilterSquelch(eic.GetVHarmCT2(), 0, true, 180);
  VoltageHarmonicsCT3 = NoiseFilterSquelch(eic.GetVHarmCT3(), 0, true, 180);

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Voltage Harmonics");
    Serial.println("Voltage Harmonics CT1: " + String(VoltageHarmonicsCT1));
    Serial.println("Voltage Harmonics CT2: " + String(VoltageHarmonicsCT2));
    Serial.println("Voltage Harmonics CT3: " + String(VoltageHarmonicsCT3));
    Serial.println("");
  }

  // Current Harmonics
  CurrentHarmonicsCT1 = NoiseFilterSquelch(eic.GetCHarmCT1(), 0, true, 180);
  CurrentHarmonicsCT2 = NoiseFilterSquelch(eic.GetCHarmCT2(), 0, true, 180);
  CurrentHarmonicsCT3 = NoiseFilterSquelch(eic.GetCHarmCT3(), 0, true, 180);

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Current Harmonics");
    Serial.println("Current Harmonics CT1: " + String(CurrentHarmonicsCT1));
    Serial.println("Current Harmonics CT2: " + String(CurrentHarmonicsCT2));
    Serial.println("Current Harmonics CT3: " + String(CurrentHarmonicsCT3));
    Serial.println("");
  }
#endif

  if (DisplayFull == true) // Display Expanded Information
    PrintUnderline("Other Information");

  // Chip Temperature
  ChipTemperature = NoiseFilterSquelch(eic.GetTemperature());

  if (DisplayFull == true) // Display Expanded Information
    Serial.println("Chip Temperature: " + String(ChipTemperature) + " °C\n");

  // Line Frequency
  LineFrequency = NoiseFilterSquelch(eic.GetFrequency());

  if (DisplayFull == true) // Display Expanded Information
    Serial.println("Mains Frequency: " + String(LineFrequency) + " Hz\n");

  // Read PCB DCV_IN Voltage
  if (DisplayFull == true)
    ReadADCVoltage(); // Read AC>DC Input Voltage
}

// **************** SETUP ****************
void setup()
{

  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1); // 115200
  // Serial.begin();
  while (!Serial)
    ;
  Serial.println("");

  // Initialise UART 1 - RS485 Port
  Serial1.begin(9600, SERIAL_8N1, RXLP, TXLP); // LP
  while (!Serial)
    ;
  Serial.println("UART 1 Opened (RS485 Port)\n");

  Serial.println("FLEX Lite - Integrated Firmware for DitroniX Power Energy Monitor SDK Boards\n");

  // Board Port Configuration
  ConfigureBoard();

  // Display Wi-Fi Options
  Serial.println("");
  PrintUnderline("Wi-Fi Options");

  // Display Software Options
  Serial.println("");
  PrintUnderline("Software Options");

  // Initialise RGB
  ws2812b.begin(); // Initialize WS2812B
  ws2812b.clear(); // Clear
  RGB_Test();

// ****************  Initialise the ATM90E3x & Pass related calibrations to its library ****************

// LineFreq = lineFreq, PGAGain = pgagain, VoltageGainX = ugain, CurrentGainCT1 = igainCT1, CurrentGainCT2 = igainCT2, CurrentGainCT3 = igainCT3, CurrentGainCTN = igainCTN
#if ATM90DEVICE == ATM90E32_DEVICE && ATM_SPLITPHASE == true // Not Fully Tested.
// the 2nd (B) current channel is not used with the split phase meter
#if ATM_SINGLEVOLTAGE == true
  eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, 0, CurrentGainCT2);
#else
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, 0, CurrentGainCT2);
#endif
#endif

#if ATM90DEVICE == ATM90E32_DEVICE && ATM_SPLITPHASE == false
#if ATM_SINGLEVOLTAGE == true
  // eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain1, VoltageGain1, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
#else
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
#endif
#endif

#if ATM90DEVICE == ATM90E36_DEVICE && ATM_SPLITPHASE == false
#if ATM_SINGLEVOLTAGE == true
  // eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain1, VoltageGain1, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
#else
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
#endif
#endif

  Serial.println("\n" + AppAcronym + " Hardware Setup and Power-Up Test - Board Initialized");

  delay(1000);

  Serial.println("");

  DisplayBoardConfiguration(); // Display once

  DisplayRegisters(); // Display once

  // App Complete
  if (OLED_Enabled == true)
  {
    oled.clear();
    OLEDPrint(AppAcronym, 2, 0);
    OLEDPrint("Test Mode", 2, 2);
    oled.update();
    delay(500);
  }

  // Display ATM Specific Registers
  // eic.GetRegisters();

} // Setup

// **************** LOOP ****************
void loop() // This loop tests the various GPIO ports and also outputs read data.
{

  String sDIR = "";
  String LineMessage;

  // RS485 Test Message TX Output
  // if EnableBasicLoop == False, then reset the board to refresh values.
  LineMessage = "Mains RMS Voltage 1: " + String(LineVoltage1) + " V AC\n";
  Serial1.print(LineMessage);

  LineMessage = "Mains Current CT1: " + String(LineCurrentCT1) + " A AC\n";
  Serial1.print(LineMessage);

  Serial1.write("Hello World - Test\n\n");

  // Cycle FET Output (Seen on LED)
  digitalWrite(FET, HIGH);
  delay(50);
  digitalWrite(FET, LOW);

  // Cycle RS485 RX EN Output (Seen on LED)
  digitalWrite(ENLP, LOW);
  delay(50);
  digitalWrite(ENLP, HIGH);

  // Cycle INT_ETH Output (Seen on LED)
  digitalWrite(INT_ETH, HIGH);
  delay(50);
  digitalWrite(INT_ETH, LOW);

  // Simply DisplayRegisters on Loop if Enabled
  if (EnableBasicLoop == true)
  {
    DisplayRegisters();
    Serial.println("- - - / / - - -\n");
  }

  // Display Values to OLED Display
  if (EnableOLEDLoop == true && OLED_Enabled == true)
  {

    DisplayRegisters(false); // Refresh Values and Display.  Default false = Mute Expanded Info to Serial

    // Update the OLED Display
    oled.clear();
    oled.setScale(2);

    if (LineVoltageAverage > 80 || LineVoltage1 > 80) // Arbitrary Value
    {

      oled.setCursor(0, 2);

      // Reset Counter
      if (OLEDCount >= 3)
        OLEDCount = 0;

      OLEDCount++;

      // Check Active Power and Display on Each Channel
      oled.printf("%d: ", OLEDCount);

      switch (OLEDCount)
      {
      case 1:
        if (ActivePowerImportCT1 > 0)
        {
#if ATM90DEVICE == ATM90E32_DEVICE
          oled.printf("%.0f W", ActivePowerImportCT1);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
          oled.printf("%.0f W", ActivePowerImportCT1 * 1000);
#endif
          sDIR = "Impt";
        }
        else if (ActivePowerExportCT1 > 0)
        {
#if ATM90DEVICE == ATM90E32_DEVICE
          oled.printf("%.0f W", ActivePowerExportCT1);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
          oled.printf("%.0f W", ActivePowerExportCT1 * 1000);
#endif
          sDIR = "Expt";
        }
        else
        {
          oled.printf("0 W");
          sDIR = "Zero";
        }
        break;

      case 2:
        if (ActivePowerImportCT2 > 0)
        {
#if ATM90DEVICE == ATM90E32_DEVICE
          oled.printf("%.0f W", ActivePowerImportCT2);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
          oled.printf("%.0f W", ActivePowerImportCT2 * 1000);
#endif
          sDIR = "Impt";
        }
        else if (ActivePowerExportCT2 > 0)
        {
#if ATM90DEVICE == ATM90E32_DEVICE
          oled.printf("%.0f W", ActivePowerExportCT2);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
          oled.printf("%.0f W", ActivePowerExportCT2 * 1000);
#endif
          sDIR = "Expt";
        }
        else
        {
          oled.printf("0 W");
          sDIR = "Zero";
        }
        break;

      case 3:
        if (ActivePowerImportCT3 > 0)
        {
#if ATM90DEVICE == ATM90E32_DEVICE
          oled.printf("%.0f W", ActivePowerImportCT3);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
          oled.printf("%.0f W", ActivePowerImportCT3 * 1000);
#endif
          sDIR = "Impt";
        }
        else if (ActivePowerExportCT3 > 0)
        {
#if ATM90DEVICE == ATM90E32_DEVICE
          oled.printf("%.0f W", ActivePowerExportCT3);
#endif
#if ATM90DEVICE == ATM90E36_DEVICE
          oled.printf("%.0f W", ActivePowerExportCT3 * 1000);
#endif
          sDIR = "Expt";
        }
        else
        {
          oled.printf("0 W");
          sDIR = "Zero";
        }
        break;
      }

      // Header
      oled.setCursor(0, 0);
      oled.print(LineVoltage1, 0);
      oled.println(" V " + sDIR);
    }
    else
    {
      // Houston, We may have a probem
      oled.clear();
      OLEDPrint("Please Chk", 2, 0);
      OLEDPrint("Line Volts", 2, 2);
      oled.update();

      delay(1500);
    }

    // Write to OLED
    oled.update();
  }

  // Heatbeat LED
  digitalWrite(LED_Red, HIGH);
  delay(50);
  digitalWrite(LED_Red, LOW);

  // Loop Delay
  delay(LoopDelay * 1000);
}
