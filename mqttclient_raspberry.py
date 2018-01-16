#Ce programme est executé sur une rasbpberry afin de controler les ampoules Philips Hue .
#C'est un client Mqtt sbscriber 

import requests
import json

import paho.mqtt.client as mqtt
import os, urlparse
import RPi.GPIO as GPIO

url = "http://192.168.0.100/api/Zv3lLGZVeJEqngcCHfoKtKYMRmGDy-wi6LwSuxvt/lights/10/state"


# Define event callbacks
def on_connect(client, userdata, flags, rc):
    print ("on_connect:: Connected with result code "+ str ( rc ) )
    print("rc: " + str(rc))



def on_message(client, obj, msg):
    print ("on_message:: this means  I got a message from brokerfor this topic")
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    ###Here i have to control the hue
    topic=msg.topic
    message=msg.payload
    if message=="ON" :
        data = {"on":True, "sat":254, "bri":254,"hue":5000}
    elif message=="OFF" :
        data = {"on":False}
    r = requests.put(url, json.dumps(data), timeout=5)
        
    
    

def on_publish(client, obj, mid):
    print("mid: " + str(mid))

def on_subscribe(client, obj, mid, granted_qos):
    print("This means broker has acknowledged my subscribe request")
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(client, obj, level, string):
    print(string)

mqttc = mqtt.Client()
# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe

# Uncomment to enable debug messages
#mqttc.on_log = on_log

# Connect
mqttc.username_pw_set("ymvyzjem","pVqcmYxCk3-U")
mqttc.connect('m14.cloudmqtt.com',12760,60)

mqttc.subscribe ("building1/room1/light" ,0 )

# Publish a message
#mqttc.publish("koukou", "kharaa")

# Continue the network loop, exit when an error occurs
rc = 0
while rc == 0:
    rc = mqttc.loop()
print("rc: " + str(rc))

