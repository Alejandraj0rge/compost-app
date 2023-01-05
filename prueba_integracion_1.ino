#include <WiFi.h>
#include <PubSubClient.h>
#include "max6675.h"
#include <MHZ19.h>

// WiFi
const char *ssid = "CLAROCT3HU"; // Enter your WiFi name
const char *password = "485754439296819E";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "mqtt.flespi.io";
const char *topic = "my/topic";
const char *mqtt_username = "oUHiR7Jm1EnQT0ufn2simx5O5Cyc6fOk0jsHe2R6u4McS7h5Xxb7V6LOw9OqcqB0";
const char *mqtt_password = "";
const int mqtt_port = 1883;

//sensor de temperatura
int thermoDO = 19;
int thermoCS = 5;
int thermoCLK = 18;

//timer
hw_timer_t * timer = NULL;

//ACTUADORES

int FAN = 26;
int PUMP = 25;
int MOTOR = 33;

//leds de status
int STATUSLED = 2;
int WIFISTATUSLED = 13;


//banderas
int mesofila = 0;
int termofila = 0;
int notificationSent = 0;

WiFiClient espClient;
PubSubClient client(espClient);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
MHZ19 mhz19;  

void IRAM_ATTR onTimer(){
  timerEnd(timer);
  digitalWrite(FAN, LOW);
  timer = NULL;
}

void setup() {
  pinMode(FAN, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(STATUSLED, OUTPUT);
  pinMode(WIFISTATUSLED, OUTPUT);
  
   Serial.begin(115200);
   delay(500);

  //conectando a la red wifi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(WIFISTATUSLED, HIGH);
       delay(500);
       Serial.println("Connecting to WiFi..");
       digitalWrite(WIFISTATUSLED, LOW);
   }
   
   Serial.println("Connected to the WiFi network");
  
   mhz19.begin(Serial1);
   mhz19.autoCalibration();
   
   //conectando al broker mqtt
   client.setServer(mqtt_broker, mqtt_port);
   client.setCallback(callback);
   
  while (!client.connected()) {
     String client_id = "mqtt-board-panel-ba36589a";
     Serial.printf("\n The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
  }
}
 
void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
 }
 Serial.println();
 Serial.println("-----------------------");
}

void loop() {
  digitalWrite(STATUSLED, HIGH);

  
  //READ HUMIDITY
  Serial.print("H% = "); 
  float humidity = analogRead(36);
  float humidityPercent = map(humidity,4025,0,0,100);
  Serial.println(humidityPercent);
  delay(1000);
  client.publish(topic, ("humidity," + String(humidityPercent)).c_str());
  delay(500);

  if(humidity < 40){
    turn_on("motor");
    turn_on("pump");
  }

  float pH = (3.3 * (analogRead(35) *(3.3/4095.0)));
  Serial.print("pH% = "); 
  Serial.println(pH);
  delay(500);
  client.publish(topic, ("ph," + String(pH)).c_str());
  delay(500);

  if(!((pH > 5) && (pH < 8)) && !notificationSent){
    client.publish(topic, "notification, ph muy bajo");
  }

  float CO2 = mhz19.getCO2();
  delay(500);
  client.publish(topic, ("co2," + String(CO2)).c_str());

  if(!( (CO2 > 729) && (CO2 < 763) )){
    client.publish(topic, "notification, co2 muy bajo");
 }
    
  //READ TEMP
  Serial.print("C = "); 
  float temp = thermocouple.readCelsius();
  Serial.println(temp);
  delay(1000);
  client.publish(topic, ("temperature," + String(temp)).c_str());

    //ESTA EN ETAPA MESOFILA
   if(temp > 20 && temp < 30 && mesofila == 0){
      delay(1000);
      client.publish(topic,"etapa,mesofila");
   }

   //ESTA EN ETAPA TERMOFILA
   if(temp > 31 && temp < 40 && termofila == 0){
        delay(1000);
        mesofila = 1;
        client.publish(topic,"etapa,termofila");
    }

  //START FAN IF COMPOST TOO WARM
  if(temp >= 60 && !timer){
    turn_on("fan");
  }

  //IS COMPOST COOLING
  if(mesofila == 1){
    if(temp > 31 && temp < 40){
        termofila = 1;
        client.publish(topic,"stage,enfiamiento");
    }
  }

//EL COMPOST ESTA MADURANDO
  if(termofila == 1){
    if(temp > 20 && temp < 30){
      client.publish(topic,"stage,maduracion");
      delay(10000);
      client.publish(topic, "notification, Tu compost esta en estado de maduracion y esta listo para ser utilizado");
    }
  }

  delay(1000);
  
  digitalWrite(STATUSLED, LOW);
  client.loop();
}

void turn_on(String actuator)
{
  if(actuator == "pump"){
    if((!digitalRead(PUMP))){
      digitalWrite(PUMP, HIGH);
      delay(1000);
      digitalWrite(PUMP, LOW);
    }
  }

  if(actuator == "fan"){
    digitalWrite(FAN, HIGH);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 60000000, true);
    timerAlarmEnable(timer);
    Serial.println("start timer");
  }

  if(actuator == "motor"){
    if((!digitalRead(MOTOR))){
      digitalWrite(MOTOR, HIGH);
      delay(5000);
      digitalWrite(MOTOR, LOW);
    }
  }
}
