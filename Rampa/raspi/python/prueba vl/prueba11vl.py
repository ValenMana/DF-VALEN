import time
import board
import busio
import digitalio
import adafruit_vl53l0x

# Pines GPIO usados para XSHUT (máximo 11)
xshut_gpio_nums = [4, 17, 27, 22, 5, 6, 13, 19, 26, 18, 23]

# Inicializar I2C
i2c = busio.I2C(board.SCL, board.SDA)

# Inicializar controladores XSHUT
xshut_controls = []
for pin_num in xshut_gpio_nums:
    pin = digitalio.DigitalInOut(getattr(board, f"D{pin_num}"))
    pin.direction = digitalio.Direction.OUTPUT
    pin.value = False  # Apagar todos
    xshut_controls.append(pin)

time.sleep(0.1)

# Detectar sensores conectados
sensores_disponibles = []

print("Detectando sensores conectados...")
for i, xshut in enumerate(xshut_controls):
    # Apagar todos
    for x in xshut_controls:
        x.value = False
    time.sleep(0.01)

    # Encender el actual
    xshut.value = True
    time.sleep(0.05)

    try:
        sensor = adafruit_vl53l0x.VL53L0X(i2c)
        distancia = sensor.range  # Probar lectura
        sensores_disponibles.append(i)
        print(f"Sensor #{i} detectado OK: {distancia} mm")
        del sensor
    except Exception as e:
        print(f"Sensor #{i} no detectado.")
        xshut.value = False

time.sleep(0.5)

print(f"\nSensores conectados: {len(sensores_disponibles)}")
print(f"Índices activos: {sensores_disponibles}")
print("-" * 30)

# Bucle principal: solo recorre los sensores detectados
while True:
    distancias = []

    for i in sensores_disponibles:
        # Apagar todos
        for x in xshut_controls:
            x.value = False
        time.sleep(0.01)

        # Encender el que toca
        xshut_controls[i].value = True
        time.sleep(0.05)

        try:
            sensor = adafruit_vl53l0x.VL53L0X(i2c)
            distancia = sensor.range
            del sensor
        except Exception:
            distancia = -1
        distancias.append(distancia)

    # Mostrar resultados
    for idx, d in zip(sensores_disponibles, distancias):
        print(f"Sensor {idx}: {d} mm")
    print("-" * 30)
    time.sleep(0.3)
