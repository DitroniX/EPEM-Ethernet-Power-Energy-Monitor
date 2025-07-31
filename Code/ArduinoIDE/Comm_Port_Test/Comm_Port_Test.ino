/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the EPEM, TWO UART Ports Basic Test

  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * ditronix.net
  * github.com/DitroniX
  * github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor
  * github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/wiki
  * hackster.io/DitroniX/epem-ethernet-power-energy-monitor-8b9043
  * kickstarter.com/projects/ditronix/epem-ethernet-power-energy-monitor

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  For board configuration, see https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/wiki/Arduino-IDE

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Libraries

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Hardware Serial 0 GPIO Pins
#define RXD0 17
#define TXD0 16

// Hardware Serial 1 GPIO Pins
#define RXLP 4
#define TXLP 5

// **************** FUNCTIONS AND ROUTINES ****************

// **************** SETUP ****************
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART U0
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0);   // U0
  while (!Serial)
    ;
  Serial.println("");

  // Initialise UART U1 LP
  Serial1.begin(9600, SERIAL_8N1, RXLP, TXLP);  //LP
  while (!Serial)
    ;
  Serial.println("");  

  Serial.println("EPEM - Example Code");
}

// **************** LOOP ****************
void loop() {

  // Print to USB COM UART Port
  Serial.println("Serial U0 (USB COM Port)");

  // Print to RS-485 UART Port
  Serial1.println("Serial LP (RS485 Port)");

  delay(100);
}