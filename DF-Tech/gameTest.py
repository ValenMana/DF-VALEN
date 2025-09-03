import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), 'DF-183')))
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), 'DF-185')))

import http_send
import wifi

conexion = http_send.verificar_conexion()

id = "lfmvmwik232"
puntaje = 4

if conexion:
    http_send.cargar_config()
    http_send.enviar_jugada(id, puntaje)

else:
    wifi.conectar_wifi()

