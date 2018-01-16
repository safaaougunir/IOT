#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#define TOLERANCE 10

/***************************************************************************
 * Internet Connectivity Setup - Variables & Functions
 **************************************************************************/
 
// Enter a MAC address for your controller below (newer Ethernet shields
// have a MAC address printed on a sticker on the shield)
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x22, 0x11 };

int sensorPin = 7; // select the input pin for LDR
int sensorValue = 0; // variable to store the value coming from the sensor

int oldsensorValue=0;
int sensorValue=0;

// Set the static IP address to use if the DHCP fails to assign
IPAddress staticIP(10, 0, 0, 20);

// IP address of the Mosquitto server
char server[] = {"m14.cloudmqtt.com"};
int port = 13086;
char topic[] = {"building1/room1/lum"};

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[INFO] Topic : " );
  Serial.println(topic); 
  Serial.print("[INFO] Message :");
  Serial.println(String((char *)payload));

   Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  Serial.println();
 }
}

//creation de l'objet client
EthernetClient ethClient;
PubSubClient pubSubClient(server, port, callback, ethClient);


void connectToInternet()
{
  // Attempt to connect to Ethernet with DHCP
  if (Ethernet.begin(mac) == 0) 
  {
    Serial.print("[ERROR] Failed to Configure Ethernet using DHCP");

    // DHCP failed, attempt to connect to Ethernet with static IP
    Ethernet.begin(mac, staticIP);
  }
  
  // Delay to let Ethernet shield initialize
  delay(1000);

  // Connection successful
  Serial.println("[INFO] Connection Successful");
  Serial.print("");  
  printConnectionInformation();
  Serial.println("-----------------------------------------------");
  Serial.println("");  

}

void printConnectionInformation() 
{
  // Print Connection Information
  Serial.print("[INFO] IP Address: ");
  Serial.println(Ethernet.localIP());
  Serial.print("[INFO] Subnet Mask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("[INFO] Gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("[INFO] DNS: ");
  Serial.println(Ethernet.dnsServerIP());
}

/***************************************************************************
 * Standard Arduino Functions - setup(), loop()
 **************************************************************************/
 
void setup() {
  Serial.begin(9600); //sets serial port for communication
  // Connect Arduino to internet
  connectToInternet();
  //Connect MQTT Broker
  Serial.println("[INFO] Connecting to MQTT Broker");
  if (pubSubClient.connect("arduinoClient", "tojighyl", "G0D6od2oUAKN")) 
  {
    Serial.println("[INFO] Connection to MQTT Broker Successfull");
    
    //pubSubClient.subscribe(topic);
    Serial.println("[INFO] Successfully Subscribed to MQTT Topic ");

    Serial.println("[INFO] Publishing to MQTT Broker");
    //pubSubClient.publish("puublidhingTest", "Test Message");
    pubSubClient.publish("building1/room1/lightLevel",String(sensorValue).c_str(), true);

  }
  else
  {
    Serial.println("[INFO] Connection to MQTT Broker Failed");
    
  }    
}
void loop() {
sensorValue = analogRead(sensorPin); // read the value from the sensor
Serial.println(sensorValue); //prints the values coming from the sensor on the screen
delay(100);

int diffLight = abs(sensorValue - oldsensorValue);

if(diffLight > TOLERANCE)
    {
        oldsensorValue = sensorValue; // only save if the val has changed enough to avoid slowly drifting
        // and so on
        Serial.println("changement Liiiight save");
        pubSubClient.publish("building1/room1/lightLevel",String(sensorValue).c_str(), true);
        
    }     
}
