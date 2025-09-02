Programa de conexion y verificacion de la recepcion de datos via post HTTP (por el momento estamos usando un endpoint fake: postman-echo)

El programa se ejecuta pasandole 2 parametros: python http-send.py "ID-JUGADOR" puntaje

Para la configuracion de cada juego y la url del endpoint deseado, modificamos el archivo de configuracion config.json

El programa tiene como extra de que si no hay conexiona a internet intenta las redes otorgadas en el archivo wifi.json, modificar en caso de cambio de redes.
