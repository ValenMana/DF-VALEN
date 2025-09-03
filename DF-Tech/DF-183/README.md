# Programa de conexion y verificacion de la recepcion de datos via post HTTP
### Por el momento estamos usando un endpoint fake: postman-echo / httpbin
*Fecha de creación: 02/09/2025 -> Tener en cuenta para futuras revisiones!*

Revisar instrucciones de instalación más abajo (realizar las mismas una única vez)

## Librerias utilizadas para la conexion HTTP
* requests 
* datetime
* json
* sys
* os

## Instrucciones generales

* Abrir un terminal en la carpeta que contiene el script
* Ejecutar ```~/python http-send.py "ID_JUGADOR" puntaje>```
* El programa enviara por el metodo POST los siguientes datos almacenados en una Clase:

    * ID-JUEGO
    * ID-JUGADOR
    * Fecha y Hora
    * Puntaje

* Si logra una comunicacion correcta, imprimira por pantalla la respuesta del servidor HTTP, en el caso de postman-echo el cual es el que utilizamos para hacer las pruebas nos devuelve el mismo json el cual mandamos.

* Posterior a eso, en la consola nos indicara el status code del servidor, en caso de exito o falla.

Tambien, podremos ejecutar el programa desde otro script de la siguiente forma:
```python
  import sys
  import os

  sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), 'DF-185')))

  import http_send

  id = "ID-JUGADOR"
  puntaje = 4

  http_send.enviar_jugada(id, puntaje)

```

## Configuracion del script
Para la configuracion del programa, tenemos este archivo:

* config.json

En el archivo config.json le pasaremos al script 2 datos, nuestro servidor endpoint (ej: www.server.com) y el id del juego(ej: "Octogono" "Aros" etc..)
```json
{
  "game_id": "Octogono",
  "endpoint_url": "https://postman-echo.com/post"
}
```

