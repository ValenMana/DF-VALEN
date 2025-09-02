import requests
from datetime import datetime
import json
import sys
import os


ejecutado = False
while True:
    try:
        request = requests.get("http://www.google.com", timeout=5)
    except (requests.ConnectionError, requests.Timeout):
        print("Sin conexión a internet.")
        print("Ejecutando script para reconectar...")
        os.system('python "Wi-Fi-connect/wifi.py"')      
        print("Reintentando conexión...")
    else:
        print("Con conexión a internet.")
        break

with open("config.json", "r") as archivo:
    config = json.load(archivo)

game_id = config["game_id"]
url = config["endpoint_url"]



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

if len(sys.argv) == 3:
    
    Jugada.player_id = sys.argv[1]
    Jugada.puntaje = sys.argv[2]
    
    # Imprimir los valores recibidos
    print(f"ID del Jugador: {Jugada.player_id}")
    print(f"Puntaje: {Jugada.puntaje}")


game = Jugada(game_id, Jugada.player_id, Jugada.puntaje)


response = requests.post(url, json=game.to_dict())


if "application/json" in response.headers.get("Content-Type", ""):
    json_recibido = response.json()
    
    print("\n Código HTTP:", response.status_code) 
    print("\n Datos recibidos del endpoint:")
    print(json.dumps(json_recibido["json"], indent=4))  
    
else:
    print(f"\nERROR (Código {response.status_code})")

