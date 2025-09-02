# Programa de conexion y verificacion de la recepcion de datos via post HTTP
### Por el momento estamos usando un endpoint fake: postman-echo / httpbin
*Fecha de creación: 02/09/2025 -> Tener en cuenta para futuras revisiones!*

Revisar instrucciones de instalación más abajo (realizar las mismas una única vez)

## Librerias utilizadas para la conexion WiFi y HTTP
* requests 
* datetime
* json
* sys
* os
* threading
* subprocess

## Instrucciones generales

* Abrir un terminal en la carpeta que contiene el script
* Ejecutar ```~/python http-send.py "ID_JUGADOR" puntaje>```
* Al ejecutar el programa nos devolvera por la consola si hay conexion a internet, en el caso de que no haya, intentara conectarse a alguna red especificada en el archivo wifi.json
* Si se logra conectar a internet enviara por el metodo POST:

    * ID-JUEGO
    * ID-JUGADOR
    * Fecha y Hora
    * Puntaje

* Si logra una comunicacion correcta, imprimira por pantalla la respuesta del servidor HTTP, en el caso de postman-echo el cual es el que utilizamos para hacer las pruebas nos devuelve el mismo json el cual mandamos.

* Posterior a eso, en la consola nos indicara el status code del servidor, en caso de exito o falla.
## Configuracion del script
Tenemos 2 archivos de configuracion para el programa:

* config.json
* wifi.json

En el archivo config.json le pasaremos al script 2 datos, nuestro servidor endpoint (ej: www.server.com) y el id del juego(ej: "Octogono" "Aros" etc..)
```json
{
  "game_id": "Octogono",
  "endpoint_url": "https://postman-echo.com/post"
}
```

En el archivo wifi.json le pasaremos la cantidad de redes wifi que hayan disponibles de la siguiente forma:
```json
{
  "redes": [
    {
      "ssid": "Red1",
      "password": "Password1"
    },
    {
      "ssid": "Red2",
      "password": "Password2"
    },
    {
      "ssid": "Red3",
      "password": "Password3"
    }
  ]
}
```
A este archivo se le pueden agregar la cantidad de redes como desee el usuario.

