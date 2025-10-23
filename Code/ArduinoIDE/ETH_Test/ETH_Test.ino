
/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the EPEM, Basic Ethernet Test

  Thanks to Johann for his tutorial which may help you https://www.youtube.com/watch?v=_C8r7ypEB7k&ab_channel=JohannFritz

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

#include <ETH.h>
#include <SPI.h>
#include <WiFi.h>

#define ETH_TYPE ETH_PHY_W5500
#define ETH_ADDR 1
#define ETH_IRQ 15  // 4
#define ETH_RST -1

// SPI pins
#define ETH_SPI_SCK 19
#define ETH_SPI_MISO 20
#define ETH_SPI_MOSI 18
#define ETH_CS 14

static bool eth_connected = false;

void onEvent(arduino_event_id_t event, arduino_event_info_t info) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-eth0");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.printf("ETH Got IP: '%s'\n", esp_netif_get_desc(info.got_ip.esp_netif));
      ETH.printTo(Serial);

      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_LOST_IP:
      Serial.println("ETH Lost IP");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void testClient(const char* host, uint16_t port) {
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available())
    ;
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("closing connection\n");
  client.stop();
}

void setup() {
  Serial.begin(115200);
  WiFi.onEvent(onEvent);

  SPI.begin(ETH_SPI_SCK, ETH_SPI_MISO, ETH_SPI_MOSI);
  ETH.begin(ETH_TYPE, ETH_ADDR, ETH_CS, ETH_IRQ, ETH_RST, SPI);
}


void loop() {
  if (eth_connected) {
    testClient("www.google.com", 80);
  }
  delay(10000);
}