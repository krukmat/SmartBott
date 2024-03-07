import time
import pygatt

# Dirección MAC del dispositivo Bluetooth que estamos buscando
TARGET_MAC_ADDRESS = "EC:DA:3B:BB:DD:E2"
# UUID del servicio Bluetooth que estamos buscando
SERVICE_UUID = "19b10002-e8f2-537e-4f6c-d104768a1214"

def handle_data(handle, value_bytes):
    decoded_data = int.from_bytes(value_bytes, byteorder='little')  # Tomar solo los 2 primeros caracteres
    print(decoded_data)

if __name__ == "__main__":
    while True:
        try:
            adapter = pygatt.GATTToolBackend()
            adapter.start()
            time.sleep(10)
            print("Start")
            device = adapter.connect(TARGET_MAC_ADDRESS)
            time.sleep(10)
            print("connect")
            device.subscribe(SERVICE_UUID, callback=handle_data)
            print("done")
        except:
            pass
        finally:
            adapter.stop()
        time.sleep(60)