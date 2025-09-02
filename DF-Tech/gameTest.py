import subprocess

id_jugador = "120dpl3nf032lk"
puntaje = "27"

# Fin de la jugada, se envian los datos al servidor

comando = ["python", "DF-183/http-send.py", id_jugador, puntaje]

# Ejecutar el script y capturar su salida
resultado = subprocess.run(comando, capture_output=True, text=True)

