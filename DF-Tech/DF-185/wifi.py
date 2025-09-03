import subprocess
import json
import os
import sys
import requests
import socket

def resource_path(relative_path):
    base_path = getattr(sys, '_MEIPASS', os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(base_path, relative_path)

def conectar_wifi():
    try:
        with open(resource_path("data.json"), "r") as f:
            redes = json.load(f)["redes"]
    except Exception as e:
        print("Error leyendo data.json:", e)
        return

    for red in redes:
        ssid = red["ssid"]
        password = red["password"]
        print(f"Intentando conectar a {ssid}...")
        result = subprocess.run(
            ["nmcli", "dev", "wifi", "connect", ssid, "password", password],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
        )
        if result.returncode == 0:
            print(f"Conectado a {ssid}")
            print("IP local:", obtener_ip_local())
            return
    print("No se pudo conectar a ninguna red.")

def obtener_ip_local():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("8.8.8.8", 80))
        ip_local = s.getsockname()[0]
    except Exception as e:
        ip_local = "No disponible"
    finally:
        s.close()
    return ip_local


def verificar_conexion():
    try:
        request = requests.get("http://www.google.com", timeout=5)
        print("Con conexión a internet.")
        

        print("IP local:", obtener_ip_local())

        return True
    except (requests.ConnectionError, requests.Timeout):
        print("Sin conexión a internet.")
        print("Ejecutando reconexión Wi-Fi...")
        return False

if __name__ == "__main__":
    if verificar_conexion():
        print("Ya estás conectado.")
    else:
        conectar_wifi()
