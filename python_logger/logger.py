import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import datetime

ser = serial.Serial("COM4",115200)

times = []
temperatures = []
pressures = []
humidities = []
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(8, 6))


def parse_line(line):
    parts = line.split(",")
    temperature = float(parts[0])
    pressure = float(parts[1])
    humidity = float(parts[2])
    return temperature, pressure, humidity      


def update(frame):
    line = ser.readline().decode('utf-8').strip()
    print(repr(line))
    temperature, pressure, humidity = parse_line(line)

    times.append(datetime.datetime.now())
    temperatures.append(temperature)
    pressures.append(pressure)
    humidities.append(humidity)
    ax1.clear()
    ax1.plot(times, temperatures)
    ax1.set_ylabel("Temp [°C]")

    ax2.clear()
    ax2.plot(times, pressures)
    ax2.set_ylabel("Pressure [hPa]")

    ax3.clear()
    ax3.plot(times, humidities)
    ax3.set_ylabel("Humidity [%]")
    ax3.set_xlabel("Time")

ani = animation.FuncAnimation(fig, update, interval=1000)
plt.tight_layout()
plt.show()