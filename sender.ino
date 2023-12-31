#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

char ssid[] = "Aadarsh";    // your network SSID (name)
char pass[] = "123456789";    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "mqtt-dashboard.com";
int        port     = 1883;
const char topic[]  = "Aadarsh-wave";

const long interval = 1000;
unsigned long previousMillis = 0;

//defines variables 
int count = 0;
const int trig = 3;
const int echo = 4;
float timer;
float distance;
 

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

 
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  Serial.print("Message sent: To started");  //print some text in Serial Monitor
  mqttClient.beginMessage(topic);
  mqttClient.print("Hello Aryaman");
  // mqttClient.print(count);
  mqttClient.endMessage();
  
}

void loop() 
{
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();
  pinMode(trig, OUTPUT);
  //Clears the trigPin condition
  digitalWrite(trig,LOW);
  delayMicroseconds(2);

  //Sets the trigPin HIGH(ACTIVE) for 10 microseconds
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  pinMode(echo, INPUT);
  
  timer = pulseIn(echo,HIGH); //Reads the ecopin, return the wave travel time in microseconds
  distance=timer*0.034 / 2;   //Calculating the distance

  if(distance < 20)
  {
    Serial.println("Wave detected: ");
    Serial.println();
    
    mqttClient.beginMessage(topic);
    mqttClient.print("Name: Aryaman");
    mqttClient.print("Distance: ");
    mqttClient.print(distance);
    Serial.println(distance);
    Serial.println("cm");
    mqttClient.endMessage();
  }
    Serial.println();
}