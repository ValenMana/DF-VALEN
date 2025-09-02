import subprocess
import json
import os
import time
import threading

# Función para leer el archivo JSON
def leer_configuracion():
    if not os.path.exists("wifi.json"):
        print("El archivo de configuración 'wifi.json' no se encuentra.")
        return None
    
    try:
        with open("wifi.json", "r") as f:
            return json.load(f)
    except json.JSONDecodeError:
        print("Error al leer el archivo JSON. Asegúrate de que esté bien formado.")
        return None

# Función para intentar conectar a la red Wi-Fi
def conectar_wifi(ssid, password, timeout=10):
    print(f"Intentando conectar a {ssid}...")

    # Esta función intentará ejecutar el comando nmcli, pero con un timeout
    def run_command():
        nonlocal result
        result = subprocess.run(
            ["nmcli", "dev", "wifi", "connect", ssid, "password", password],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
        )

    # Crear el hilo que ejecutará la conexión
    result = None
    thread = threading.Thread(target=run_command)
    thread.start()

    # Esperar el tiempo de timeout especificado
    thread.join(timeout)

    # Si el hilo aún está activo después del timeout, detenerlo
    if thread.is_alive():
        print(f"Timeout alcanzado al intentar conectar a {ssid}. Intentando otra red...")
        # Terminamos el hilo de manera "suave"
        thread.join(0)  # Asegurarse de que el hilo se termine correctamente
        return False
    else:
        if result.returncode == 0:
            print(f"Conectado a {ssid} exitosamente!")
            return True
        else:
            print(f"Error al conectar a {ssid}: {result.stderr.strip()}")
            # Imprimir también stdout si deseas más detalles
            print(f"Detalles de la conexión: {result.stdout.strip()}")
            return False

# Leer las redes desde el archivo de configuración
data = leer_configuracion()

if data:
    for red in data.get("redes", []):
        if conectar_wifi(red["ssid"], red["password"]):
            break
    else:
        print("No se pudo conectar a ninguna red Wi-Fi.")
        
