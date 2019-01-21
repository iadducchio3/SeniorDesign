import time 
import RPi.GPIO as GPIO 
import paho.mqtt.client as mqtt 
import paho.mqtt.publish as publish
 


# FUNCTION| RUNS WHEN CLIENT CONNECTS TO SERVER
def on_connect(client, userdata, flags, rc): 
   print("Connected with result code " + str(rc)) 

# FUNCTION| RUNS WHEN MESSAGE IS RECEIVED
def on_message(client, userdata, msg):
   #TEMPORARY TO SEE WHATS COMING INTO BROKER 
  new_message = msg.topic
  #CHECK THE TOPIC OF THE MESSAGE
  if new_message == 'home/temperature_power':
    print('Temperature Status: '+msg.payload)
  elif new_message == 'home/light_power':
    print('Light Status: '+msg.payload)
  elif new_message == 'home/security_power':
    print('Security Status: '+msg.payload)
  elif new_message == 'home/light_sensor':
    print('Light Intensity: '+msg.payload)
  elif new_message == 'home/temperature_set':
    print('Temperature Setting: '+msg.payload)
  elif new_message == 'home/blind_status':
    print('Blind Status: '+msg.payload)
  elif new_message == 'home/temperature_sensor':
    print('Temperature: '+msg.payload)





# CREATE "CLIENT" TO CONNECT TO LOCAL SERVER
client = mqtt.Client() 
client.on_connect = on_connect 
client.on_message = on_message 
client.connect('localhost', 1883, 60) 

#CREATE THREAD TO PROCESS MESSAGE QUEUE 
client.loop_start() 

#SUBSCRIBE TO ALL TOPICS 
client.subscribe("#")

while 1:
  time.sleep(1000)



