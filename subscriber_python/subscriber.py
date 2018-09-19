import paho.mqtt.client as mqtt #import the client1
import json

broker_address="127.0.0.1" 

def on_message(client, userdata, message):
    j = json.loads(str(message.payload.decode("utf-8")))
    out_file = open("data%s.txt" % (message.topic[14:]),"a")

    t = int(j['t0']) * 1000 + int(j['t1'])

    out_file.write("%s %s %s %s\n" % (j['x'], j['y'], j['z'], t))
    print("topic: ", message.topic, "payload: " ,str(message.payload.decode("utf-8")))
    out_file.close()


print("creating new instance")
client = mqtt.Client("S1")
print("connecting to broker")
client.connect(broker_address)
client.on_message=on_message
print("Subscribing to topic","Accelerometri/#")
client.subscribe("Accelerometri/#")

client.loop_forever() #stop the loop