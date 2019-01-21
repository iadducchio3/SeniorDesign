import time 
import RPi.GPIO as GPIO 
import paho.mqtt.client as mqtt 
import paho.mqtt.publish as publish
 
# GLOBAL MOST RECENT PUBLISHED WITH INITIAL VALUES
#light_status = ("status/light","OFF")
#hvac = ("status/hvac","OFF")
#temperature = ("sensor/temperature","50")
#light_intesity = ("sensor/light_intesity","50")
#blinds = ("status/blinds","0")
# ADD OTHER STUFF HERE 


# FUNCTION| RUNS WHEN CLIENT CONNECTS TO SERVER
def on_connect(client, userdata, flags, rc): 
   print("Connected with result code " + str(rc)) 
   #PUBLISH ALL RECENT FOR NEW CLIENT TO SEE
   #client.publish(light_status)
   #client.publish(hvac)
   #client.publish(temperature)
   #client.publish(light_intesity)
   #client.publsih(blinds)

# FUNCTION| RUNS WHEN MESSAGE IS RECEIVED
def on_message(client, userdata, msg):
   #TEMPORARY TO SEE WHATS COMING INTO BROKER 
	print(msg.topic+" "+str( msg.payload)) 
   
  #CHECK THE TOPIC OF THE MESSAGE
  if msg.topic == 'status/':
	#NEED IDEA TO OVERRIDE SENSORS ON USER INPUT
		print("User Input: " + msg.topic + str(msg.payload))
  elif msg.topic == 'status/':
  	#NEED IDEA TO OVERRIDE SENSORS ON USER INPUT
  	print("User Input: " + msg.topic + str(msg.payload))

  elif msg.topic == 'sensor/':
  	print(msg.topic)
  elif msg.topic == 'sensor/':
  	print(msg.topic)


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



