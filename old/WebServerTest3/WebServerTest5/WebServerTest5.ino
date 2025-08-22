#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 199);
EthernetServer server(2000);

void setup() {
  Ethernet.init(10);
  Serial.begin(9600);
  while (!Serial) {;}
  Serial.println(F("start"));
  Ethernet.begin(mac, ip);
  randomSeed(analogRead(0));  // 난수 시드 초기화
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("error"));
    while (true) {
      delay(1);
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("error"));
  }
  server.begin();
  Serial.print(F("server is at "));
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println(F("new client"));
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println(F("Connection: close"));
          client.println(F("Refresh: 5"));
          client.println();

          client.println(F("<!DOCTYPE HTML>"));
          client.println(F("<html>"));
          client.println(F("<head>"));
          client.println(F("<meta charset='UTF-8'>"));
          client.println(F("<title>너또로또</title>"));
          client.println(F("<style>"));
          client.println(F("body { font-family: 'Segoe UI', sans-serif; background-color: #f0f4f8; color: #333; margin: 0; padding: 0; }"));
          client.println(F("header { background-color: #0078D7; color: white; padding: 20px; text-align: center; }"));
          client.println(F("main { padding: 30px; text-align: center; }"));
          client.println(F("footer { background-color: #eee; padding: 10px; text-align: center; font-size: 12px; color: #666; }"));
          client.println(F("button { padding: 10px 20px; font-size: 16px; background-color: #0078D7; color: white; border: none; border-radius: 5px; cursor: pointer; }"));
          client.println(F("button:hover { background-color: #005fa3; }"));
          client.println(F("</style>"));
          client.println(F("</head>"));
          client.println(F("<body>"));
          client.println(F("<header><h1>너또로또</h1></header>"));
          client.println(F("<main>"));
          client.println(F("<p><strong>오늘의 로또 번호:</strong></p>"));
          client.println(F("<p id='lotto' style='font-size:24px; color:#0078D7;'>버튼을 눌러 확인하세요</p>"));
          client.println(F("<button onclick='generateLotto()'>로또 번호 생성</button>"));
          client.println(F("<script>"));
          client.println(F("function generateLotto() {"));
          client.println(F("  let numbers = [];"));
          client.println(F("  while (numbers.length < 6) {"));
          client.println(F("    let num = Math.floor(Math.random() * 45) + 1;"));
          client.println(F("    if (!numbers.includes(num)) numbers.push(num);"));
          client.println(F("  }"));
          client.println(F("  document.getElementById('lotto').innerText = numbers.join(' , ');"));
          client.println(F("}"));
          client.println(F("</script>"));
          client.println(F("</main>"));
          client.println(F("<footer>&copy; 2025 Server</footer>"));
          client.println(F("</body>"));
          client.println(F("</html>"));

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
    Serial.println(F("end"));
  }
}
