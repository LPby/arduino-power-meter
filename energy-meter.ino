int port = A0;
int led = 13;
int val = 0;
int buzzer = 3;
int power;
int power_limit = 8000; // 8kW alert
unsigned long old_time;
unsigned long new_time;
char usb_in = 0;

void setup()  { 
  pinMode(led, OUTPUT); 
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);
} 

void loop()  { 
  val = analogRead(port);
  if ( val < 500 )
  {
    new_time = millis();
    digitalWrite(led, HIGH);
    power=3600000/(new_time - old_time);
    Serial.println(power);
    if ( power > power_limit ) tone(buzzer, 600, 100);
    delay(200); //wait for end of pulse (pulse time is 50 ms)
    digitalWrite(led, LOW);
    old_time = new_time;
  }
  if (Serial.available() > 0) {
    Serial.readBytes(usb_in,1);
  }
}
