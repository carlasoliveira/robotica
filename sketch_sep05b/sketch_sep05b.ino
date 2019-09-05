#define LED 23
#define sensor_pin 34

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(sensor_pin,INPUT);
  Serial.begin(9600); // open serial port, set the baud rate as 9600 bps
  
}
void loop() {
  int val;
  val = analogRead(sensor_pin); //connect sensor to Analog 0
  Serial.print(val); //print the value to serial port
  delay(1000);
  Serial.print("  "); //print the value to serial port
  delay(1000);
}
