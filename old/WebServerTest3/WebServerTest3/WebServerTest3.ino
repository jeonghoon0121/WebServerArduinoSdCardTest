/*
http://192.168.0.199:2000/
 */

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
  Serial.println("start");
  Ethernet.begin(mac, ip);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("error");
    while (true) {
      delay(1);
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("error");
  }
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<meta charset='UTF-8'>");
          client.println("<title>너또로또</title>");
          client.println("<style>");
          client.println("body { font-family: 'Segoe UI', sans-serif; background-color: #f0f4f8; color: #333; margin: 0; padding: 0; }");
          client.println("header { background-color: #0078D7; color: white; padding: 20px; text-align: center; }");
          client.println("main { padding: 30px; text-align: center; }");
          client.println("footer { background-color: #eee; padding: 10px; text-align: center; font-size: 12px; color: #666; }");
          client.println("button { padding: 10px 20px; font-size: 16px; background-color: #0078D7; color: white; border: none; border-radius: 5px; cursor: pointer; }");
          client.println("button:hover { background-color: #005fa3; }");
          client.println("</style>");
          client.println("</head>");
          client.println("<body>");
          client.println("<header><h1>너또로또</h1></header>");
          client.println("<main>");
          client.println("<p>너또로또1</p>");
          client.println("<p>너또로또2</p>");
          client.println("<button onclick=\"alert('너또로또')\">너또로또</button>");
          client.println("</main>");
          client.println("<footer>&copy; 2025 Server</footer>");
          client.println("</body>");
          client.println("</html>");

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("end");
  }
}
