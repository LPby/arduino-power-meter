#include <Ethernet.h>
#include <SPI.h>

int port = A0;
int led = 13;
int val = 0;
int buzzer = 3;
long power;
int power_limit = 8000; // 8kW alert
unsigned long old_time;
unsigned long new_time;
byte mac[] = {0x90,0xA2,0xDA,0x0E,0xF1,0x92};
IPAddress ip(192,168,99,110);
EthernetServer server(80);

void setup()  { 
  pinMode(led, OUTPUT); 
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);
  delay(1000);
  Serial.println("Getting IP address...");
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac, ip);
  }
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  delay(1000);
} 


void loop()  { 
  val = analogRead(port);
  if ( val < 500 )
  {
    new_time = millis();
    digitalWrite(led, HIGH);
    power=3600*1000*1000/(1600*(new_time - old_time)); //W = (n * 3600)/(Imp * t), кВт
    //    1h   ms   kW    pps   time(ms)
    Serial.println(power);
    if ( power > power_limit ) tone(buzzer, 1400, 100);
    delay(50); //wait for end of pulse (pulse time >=30 ms)
    digitalWrite(led, LOW);
    old_time = new_time;
  }
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    while (client.connected()) {
      if (client.available()) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 5");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("Power: ");
          client.println(power);
          client.println("</html>");
          break;
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
}
