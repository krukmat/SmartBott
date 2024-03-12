import time
import pygatt
from datetime import datetime
from simple_salesforce import Salesforce
from salesforce_creds import *
import threading



# Dirección MAC del dispositivo Bluetooth que estamos buscando
TARGET_MAC_ADDRESS = "EC:DA:3B:BB:DD:E2"
# UUID del servicio Bluetooth que estamos buscando
SERVICE_UUID = "FB6D03F3-9F80-411B-BE52-FA618EBFF138"


# Inicializar cliente Salesforce
sf = Salesforce(username=SALESFORCE_USERNAME, password=SALESFORCE_PASSWORD, security_token=SALESFORCE_SECURITY_TOKEN)
last_data = None
lock = threading.Lock()

def handle_data(handle, value_bytes):
    global last_data
    decoded_data = int.from_bytes(value_bytes, byteorder='little') / 100
    current_time = datetime.now().isoformat()
    with lock:
        last_data = (current_time, decoded_data)

if __name__ == "__main__":
    while True:
        try:
            adapter = pygatt.GATTToolBackend()
            adapter.start()
            time.sleep(10)
            print("Bluetooth adapter started")
            device = adapter.connect(TARGET_MAC_ADDRESS)
            time.sleep(10)
            print("Connected to device")
            device.subscribe(SERVICE_UUID, callback=handle_data)
            print("Subscribed to service")
            time.sleep(30)
            with lock:
                if last_data is not None:
                    record = {
                        'Date_Time__c': last_data[0],
                        'Consumption__c': last_data[1]
                    }
                    sf.Liquid_Consumption__c.create(record)
                    print("Datos enviados a Salesforce:", record)
        except Exception as e:
            print("Error:", e)
        finally:
            adapter.stop()
            print("Bluetooth adapter stopped")
        time.sleep(21600)