//INCLUSAO DAS BIBLIOTECAS
#include <WiFi.h>
#include <PubSubClient.h> //para troca de mensagens
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h> //sensor de umidade e temperatura do ar

//DEFINININDO CONSTANTES (PORTAS QUE SERAO USADOS) 
#define DHTPIN 27     // PINO DE LEITURA
#define DHTTYPE DHT11 // DHT 11
#define LED 33

// Substitua as próximas variáveis pela sua combinação SSID / Senha
const char* ssid = "nome da rede";
const char* password = "senha da rede";

// Adicione seu endereço IP do MQTT Broker, exemplo: 
const char *mqtt_user = "";
const char *mqtt_pass = "";
const int mqtt_port = 0;

// VARIAVEIS PARA AUXILIO NO ENVIO DE MENSAGENS
long lastMsg = 0;
char msg[50];
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE); //configuração do sensor

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  dht.begin();  
  setup_wifi(); 
  client.setServer(mqtt_server, mqtt_port); //configuração da nuvem/servidor
  client.setCallback(callback);
}

void setup_wifi() //configuração da rede WiFi
{
  delay(10);
  // Começamos conectando a uma rede WiFi 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  // teste de conexão com a rede
   
  Serial.println("");  
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* message, unsigned int length) //verifica as mensagens 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  
  Serial.println();
// Sinta-se à vontade para adicionar mais instruções if para controlar mais GPIOs com o MQTT
// Se uma mensagem for recebida no tópico esp32 / output, verifica se a mensagem está "ativada" ou "desativada".
// Altera o estado de saída de acordo com a mensagem
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    
    if(messageTemp == "on") {
      Serial.println("on");  
      digitalWrite(LED, 1); 
    }
    else if(messageTemp == "off") {
      Serial.println("off");
      digitalWrite(LED, 0);
    }
  }
}


void reconnect() //realiza uma nova conexão em caso de falha
{
  // Loop until we're reconnected 
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);  
    }  
  }
}


void loop()
{ 
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.

  float h = dht.readHumidity(); //leitura do sensor
  float t = dht.readTemperature(); //leitura do sensor
  // testa se retorno é valido, caso contrário algo está errado.
  // not a number ( numero nao válido)
  if (isnan(t) || isnan(h)) {
  Serial.println("Failed to read from DHT");
  }
  else {
    long now = millis();  
      
    if (now - lastMsg > 5000) {    
      char tempString[8];   
      lastMsg = now;
      dtostrf(t, 1, 2, tempString); //converte o valor passado em t e armazena em tempString.
      client.publish("esp32/temperature", tempString); //publica informação da temperatura para a interface
      Serial.print("Umidade: ");      
      Serial.print(h);      
      Serial.print(" %t");      
      Serial.print("Temperatura: ");      
      Serial.print(t);      
      Serial.println(" *C  ");
    }
  }
}
