import requests
from datetime import datetime
import json
import sys
import os
import wifi

# Compatibilidad con PyInstaller
def resource_path(relative_path):
    base_path = getattr(sys, '_MEIPASS', os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(base_path, relative_path)

# Reintento de conexión
while True:
    try:
        request = requests.get("http://www.google.com", timeout=5)
    except (requests.ConnectionError, requests.Timeout):
        print("Sin conexión a internet.")
        print("Ejecutando reconexión Wi-Fi...")
        wifi.conectar_wifi()  # Asume que wifi.py tiene esta función
    else:
        print("Con conexión a internet.")
        break

# Leer config
with open(resource_path("config.json"), "r") as archivo:
    config = json.load(archivo)

game_id = config["game_id"]
url = config["endpoint_url"]

# Clase de juego
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

# Validar argumentos
if len(sys.argv) == 3:
    player_id = sys.argv[1]
    puntaje = int(sys.argv[2])
else:
    print("Uso: python http-test.py <player_id> <puntaje>")
    sys.exit(1)

# Crear jugada
game = Jugada(game_id, player_id, puntaje)

# Enviar a endpoint
response = requests.post(url, json=game.to_dict())

# Mostrar resultado
if "application/json" in response.headers.get("Content-Type", ""):
    json_recibido = response.json()
    print("\nCódigo HTTP:", response.status_code)
    print("\nDatos recibidos del endpoint:")
    print(json.dumps(json_recibido["json"], indent=4))
else:
    print(f"\nERROR (Código {response.status_code})")
