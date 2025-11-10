# Programa de conexion a internet via un script de python para raspberry pi

*Fecha de creación: 02/09/2025 -> Tener en cuenta para futuras revisiones!*

Revisar instrucciones de instalación más abajo (realizar las mismas una única vez)

## Librerias utilizadas para la conexion WiFi
* requests 
* json
* sys
* os
* subprocess
* socket

## Instrucciones generales

* Abrir un terminal en la carpeta que contiene el script
* Ejecutar ```~/python wifi.py>```
* El programa verificara que haya conexion a internet, en el caso de que no, probara con las redes otorgadas en el archivo de configuracion wifi.json

* Si logra conectarse a una red, imprimira por pantalla la IP otorgada por DHCP y mostrando a que red logro conectarse.

Tambien, podremos ejecutar el programa desde otro script de la siguiente forma:
```python
  import sys
  import os

  sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), 'DF-185')))

  import wifi

  conexion = wifi.verificar_conexion()

  if conexion:
      print("Conexion establecida")

  else:
      print("No hay conexio, reintentando...")
      wifi.conectar_wifi()
```

## Configuracion del script
Para la configuracion del programa, tenemos este archivo:

* wifi.json

En el archivo config.json le pasaremos la cantidad de redes disponibles a nuestro alcanze.

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
Al archivo de configuracion wifi.json se le pueden agregar tantas redes wifi como el usuario desee.
