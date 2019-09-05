#define LED 23
#define sensor_pin 34

const int AirValue = 520;   //you need to replace this value with Value_1 
const int WaterValue = 260;  //you need to replace this value with Value_2 
int intervals = (AirValue - WaterValue)/3;    
int soilMoistureValue = 0; 

void setup(){
  pinMode(LED,OUTPUT);
  pinMode(sensor_pin,INPUT);
  Serial.begin(115200);

}

void loop(){
  digitalWrite(LED,HIGH);
  soilMoistureValue = sensor_pin;  //put Sensor insert into soil 
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals)) { 
  Serial.println("Very Wet"); 
  } 
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals)) { 
  Serial.println("Wet"); 
  } 
  else if(soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals)) { 
  Serial.println("Dry"); 
  } 
  delay(100); 

}
