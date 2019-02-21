import time
import RPi.GPIO as GPIO
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import datetime
import SMS
#from threading import Thread
#from datetime import timedelta

hvac_power = 'off'
last_temp = 0
last_temp_set = 0
last_home_motion = datetime.datetime.now()


# FUNCTION| RUNS WHEN CLIENT CONNECTS TO SERVER

def on_connect(client, userdata, flags, rc,):
    print 'Connected with result code ' + str(rc)


'''def controlHVAC():
    global hvac_power, last_temp, last_temp_set
    if hvac_power == 'on':
        if last_temp_set < last_temp:
            client.publish('home/fan_power', 'on')
        elif last_temp_set > last_temp:
            client.publish('home/fan_power', 'off')
    elif hvac_power == 'off':
        client.publish('home/fan_power', 'off')'''


'''def automaticLights():
    print("Thread Started")
    global client
    while 1:
        if last_home_motion + timedelta(seconds=20) > datetime.datetime.now():
            difference = (last_home_motion + timedelta(seconds=20) - datetime.datetime.now()).total_seconds()
            print(difference)
            client.publish('home/light_power', 'on')
            time.sleep(difference)
        else:
            client.publish('home/light_power', 'off')
    print('exiting automatic lights')'''


# FUNCTION| RUNS WHEN MESSAGE IS RECEIVED

def on_message(client, userdata, msg):
    message_payload = msg.payload
    new_message = msg.topic
    if new_message == 'home/temperature_power':
        print 'Temperature Status: ' + msg.payload
        if msg.payload == 'on':
            hvac_power = 'on'
        else:
            hvac_power = 'off'
        controlHVAC()
    elif new_message == 'home/temperature_set':

        print 'Temperature Setting: ' + msg.payload
        newTempSet(message_payload)
    elif new_message == 'home/temperature_sensor':

        print 'Temperature: ' + msg.payload
        newTemp(value)
    elif new_message == 'home/light_sensor':

        print 'Light Intensity: ' + msg.payload
        if msg.payload > 50:
            client.publish('home/blind_status', 'open')
        else:
            client.publish('home/blind_status', 'closed')
    elif new_message == 'home/security_motion':

        dt = datetime.datetime.now()
        time = dt.strftime('%d %b %Y') + ' | ' + dt.strftime('%I:%M %p')
        print 'Motion Detected: ' + time
        client.publish('home/last_motion_detected', time)
        if security_power == 'on':
            SMS.send_message('Motion detected at')
            client.publish('home/security_light', 'on')
    elif new_message == 'home/home_motion':

        last_home_motion = datetime.datetime.now()


# CREATE "CLIENT" TO CONNECT TO LOCAL SERVER
client = mqtt.Client() 
client.on_connect = on_connect 
client.on_message = on_message 
client.connect('localhost', 1883, 60) 

'''newThread = Thread(target=automaticLights, args=())
newThread.daemon = True
newThread.start()'''

# CREATE THREAD TO PROCESS MESSAGE QUEUE
client.loop_start()

# SUBSCRIBE TO ALL TOPICS

client.subscribe("#")
#client.subscribe('home/temperature_power')
#client.subscribe('home/temperature_set')
#client.subscribe('home/temperature_sensor')
#client.subscribe('home/light_sensor')
#client.subscribe('home/security_motion')
#client.subscribe('home/home_motion')




while 1:
    time.sleep(10000)
