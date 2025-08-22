#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#define CS_PIN 4 // SD 카드의 CS 핀
#define FILENAME F("index.htm") // 제공할 HTML 파일 이름

byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0x53, 0x07}; // MAC 주소
IPAddress ip(192, 168, 0, 50); // IP 주소
EthernetServer server(80); // 서버 포트 80

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initializing SD card..."));

  if (!SD.begin(CS_PIN)) {
    Serial.println(F("Card failed, or not present"));
    while (1);
  }
  Serial.println(F("card initialized."));

  if (!SD.exists(FILENAME)) {
    Serial.println(F("ERROR - Can't find index.htm file!"));
    while (1);
  }
  Serial.println(F("SUCCESS - Found index.htm file."));

  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);

        if (c == '\n' && currentLineIsBlank) {
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println(F("Connection: close"));
          client.println();

          File webFile = SD.open(FILENAME);
          if (webFile) {
            while (webFile.available()) {
              client.write(webFile.read());
            }
            webFile.close();
          }
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}
