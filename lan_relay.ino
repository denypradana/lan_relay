
#include <SPI.h>
#include <Ethernet.h>

int pinRelay = 2;
int pos = 0;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   
byte ip[] = { 192, 168, 2, 90 };                    
byte gateway[] = { 192, 168, 2, 1 };                 
byte subnet[] = { 255, 255, 255, 0 };                  
EthernetServer server(8888);                           
String readString;
String statusRelay;

void setup() {
  
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, HIGH);
  statusRelay="OFF";

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (readString.length() < 100) {

          readString += c;

        }


        if (c == '\n') {
          Serial.println(readString);
          //html file
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          client.println("<meta http-equiv='refresh' content='1' >");
          client.println("<TITLE>Smart HomeProjects</TITLE>");
          client.println("<style>");
          client.println(".button{");
          /*ubah warna background*/
          client.println("background-color: #424eee;");
          /*ubah warna teks*/
          client.println("color: white;");
          /*hilangkan garis bawah pada teks*/
          client.println("text-decoration: none;");
          /*teks rata tengah*/
          client.println("text-align: center;");
          /*atur jarak margin/luar*/
          client.println("margin: 5px 10px;");
          /*atur jarak padding/dalam*/
          client.println("padding: 10px 15px;");
          /*ubah tampilan kursor*/
          client.println("cursor: pointer;");
          /*membuat efek bayangan*/
          client.println("box-shadow: 0px 8px 16px 0px rgba(0, 0, 0, 0.2);");
          client.println("}");
          client.println("</style>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Smart Home</H1>");
          client.println("<hr />");
          client.println("<br />");
          client.println("<H2>RUANG_TENGAH</H2>");
          client.println("<br />");
          //BUTTON FOR FIRST LED
          client.println("<a href=\"/?button1on\" class=\"button\"\">Turn On Relay 1</a>");
          client.println("<a href=\"/?button1off\" class=\"button\"\">Turn Off Relay 1</a><br />");
          client.println("<br />");
          client.println(statusRelay);
          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          //stopping client
          client.stop();
          //controls the Arduino if you press the buttons
          if (readString.indexOf("?button1on") > 0) {
            digitalWrite(pinRelay, LOW);
            statusRelay="ON";
          }
          if (readString.indexOf("?button1off") > 0) {
            digitalWrite(pinRelay, HIGH);
            statusRelay="OFF";
          }


          //clearing string for next read
          readString = "";

        }
      }
    }
  }
}

