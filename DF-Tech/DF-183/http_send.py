# http_test.py

import requests
from datetime import datetime
import json
import sys
import os

def resource_path(relative_path):
    base_path = getattr(sys, '_MEIPASS', os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(base_path, relative_path)

def verificar_conexion():
    try:
        request = requests.get("http://www.google.com", timeout=5)
        print("Con conexión a internet.")
        return True
    except (requests.ConnectionError, requests.Timeout):
        print("Sin conexión a internet.")
        print("Ejecutando reconexión Wi-Fi...")
        #wifi.conectar_wifi()
        return False

def cargar_config():
    with open(resource_path("config.json"), "r") as archivo:
        return json.load(archivo)

class Jugada:
    def __init__(self, juego, player_id, puntaje):
        self.juego = juego
        self.player_id = player_id
        self.fecha = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self.puntaje = puntaje

    def to_dict(self):
        return {
            "game": self.juego,
            "player": self.player_id,
            "date": self.fecha,
            "score": self.puntaje
        }

def enviar_jugada(player_id, puntaje):
    
    config = cargar_config()
    game_id = config["game_id"]
    url = config["endpoint_url"]

    game = Jugada(game_id, player_id, puntaje)
    response = requests.post(url, json=game.to_dict())

    if "application/json" in response.headers.get("Content-Type", ""):
        json_recibido = response.json()
        print("\nCódigo HTTP:", response.status_code)
        print("\nDatos recibidos del endpoint:")
        print(json.dumps(json_recibido["json"], indent=4))
    else:
        print(f"\nERROR (Código {response.status_code})")

# Solo se ejecuta si corres este script directamente (no si lo importas)

