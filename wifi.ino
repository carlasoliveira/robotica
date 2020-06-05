#include <WiFi.h>
//-------- Configurações de Wi-fi-----------
const char* ssid = "Biblioteca";  // Substitua pelo nome da rede
const char* password =  "";         // Substitua pela senha
int led = 2;                                                // led D2 ESP32 ;

void setup()
{
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  //---- Tenta conectar ao Wi-Fi e a senha ----
  WiFi.begin(ssid, password);
  //---- Enquanto estiver conectando ----
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);                                 // Aguarda meio segundo
    Serial.println("Conectando ao WiFi..");     // Conectando
  }
  Serial.println("Conectado"); // Conectado
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  { // Se o ESP estiver conectado
    digitalWrite(led, 1);                // Liga
    
  }

  else
  { // Se estiver desconectado
    digitalWrite(led, 0);                // Desliga 

  }
}
