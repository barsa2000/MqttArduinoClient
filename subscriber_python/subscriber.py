import paho.mqtt.client as mqtt
import json
import os

# indirizzo IP del broker MQTT
broker_address="127.0.0.1" 

# verifica dell'esistenza della cartella "data", se non presente creazione 
# della stessa
if not os.path.isdir("data"):
	os.mkdir("data")

#funzione richiamata alla ricezione di un nuovo messaggio MQTT
def on_message(client, userdata, message):
    # formattazione dei dati ricevuto in una struttura JSON
    j = json.loads(str(message.payload.decode("utf-8")))
    # apertura file per il salvataggio dei dati ricevuto
    out_file = open("data/acc%s.txt" % (message.topic[14:]),"a")
    # calcolo del timestamp
    t = int(j['t0']) * 1000 + int(j['t1'])
    # salvataggio su file dei dati ricevuti
    out_file.write("%s %s %s %s\n" % (j['x'], j['y'], j['z'], t))
    print(
        "topic: ", 
        message.topic, 
        "payload: " ,
        str(message.payload.decode("utf-8"))
    )
    # chiusura del file
    out_file.close()

# creazione di una nuova istanza MQTT
print("creating new instance")
client = mqtt.Client("S1")
# connessione al broker MQTT
print("connecting to broker")
client.connect(broker_address)
# impostazione della funzione da richiamare in caso di arrivo di nuovi messaggi
client.on_message=on_message
# iscrizione all'argomento desiderato
print("Subscribing to topic","Accelerometri/#")
client.subscribe("Accelerometri/#")

# avvio del ciclo ricezione/invio MQTT
client.loop_forever()