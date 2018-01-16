#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#define TOLERANCE 10

int state = 0;

/***************************************************************************
 * Internet Connectivity Setup - Variables & Functions
 **************************************************************************/
 
// Enter a MAC address for your controller below (newer Ethernet shields
// have a MAC address printed on a sticker on the shield)
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x22, 0x11 };

int oldVal = 0;
int oldVal1 = 0;

int val;
int val1;



// Set the static IP address to use if the DHCP fails to assign
IPAddress staticIP(10, 0, 0, 20);
// IP address of the Mosquitto server
char server[] = {"m14.cloudmqtt.com"};
int port = 13086;
char topic[] = {"building1/room1/light"};



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

void setup() 
{
  // Initialize serial port
  Serial.begin(9600);

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

void loop() 
{  
     // read the input on analog pin 0:
    val = analogRead(A0);
    val = map(val, 0, 1023, 0, 254);
    Serial.println(val);
    
    delay(15);
  
    int diff = abs(val - oldVal);

    if(diff > TOLERANCE)
    {
        oldVal = val; // only save if the val has changed enough to avoid slowly drifting
        // and so on
        Serial.println("changement save");
        pubSubClient.publish("building1/room1/light",String(val).c_str(), true);
        
    }     
  // Wait for messages from MQTT broker
  //pubSubClient.loop();
  
  }



