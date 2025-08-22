#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 199);
EthernetServer server(2000);

const int chipSelect = 4;  // SD 카드 CS 핀 (보드에 따라 다를 수 있음)

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}

  Ethernet.init(10);  // Ethernet CS 핀
  if (!Ethernet.begin(mac)) {
    Serial.println("Ethernet 초기화 실패");
    while (true);
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("SD 카드 초기화 실패");
    while (true);
  }

  server.begin();
  Serial.print("서버 IP: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("클라이언트 접속");

    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank) {
          // HTTP 헤더 전송
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html; charset=UTF-8");
          client.println("Connection: close");
          client.println();

          // SD 카드에서 HTML 파일 읽기
          File file = SD.open("ind.html");
          if (file) {
            while (file.available()) {
              client.write(file.read());
            }
            file.close();
          } else {
            client.println("<h1>404 - 파일을 찾을 수 없습니다</h1>");
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
    Serial.println("클라이언트 연결 종료");
  }
}
