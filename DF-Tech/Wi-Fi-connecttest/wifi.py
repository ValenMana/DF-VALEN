import subprocess
import json

with open("data.json", "r") as f:
    data = json.load(f)

def conectar_wifi(ssid, password):
    print(f"Intentando conectar a {ssid}...")
    result = subprocess.run(
        ["nmcli", "dev", "wifi", "connect", ssid, "password", password],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    if result.returncode == 0:
        print(f"Conectado a {ssid} exitosamente!")
        return True
    else:
        print(f"Error al conectar a {ssid}: {result.stderr.strip()}")
        return False

for red in data["redes"]:
    if conectar_wifi(red["ssid"], red["password"]):
        break
else:
    print("No se pudo conectar a ninguna red Wi-Fi.")
