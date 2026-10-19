import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import datetime
import csv
import os

ser = serial.Serial("COM5",115200, timeout=2)
file_exists = os.path.exists("data.csv")
csv_file = open("data.csv", "a", newline="")
csv_writer = csv.writer(csv_file)
if not file_exists:
    csv_writer.writerow(["timestamp", "temperature", "pressure", "humidity"])

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

    # nothing came in (timeout)
    if line == "":
        return

    # firmware sends ERR,<code> when the sensor fails
    if line.startswith("ERR"):
        print("sensor error:", line)
        return

    try:
        temperature, pressure, humidity = parse_line(line)
    except (ValueError, IndexError):
        print("bad line, skipping:", line)
        return

    now = datetime.datetime.now()
    times.append(now)
    temperatures.append(temperature)
    pressures.append(pressure)
    humidities.append(humidity)
    csv_writer.writerow([now, temperature, pressure, humidity])
    csv_file.flush()
    ax1.clear()
    ax1.plot(times, temperatures)
    ax1.set_ylabel("Temp [°C]")

    ax2.clear()
    ax2.plot(times, pressures)
    ax2.ticklabel_format(axis="y", useOffset=False)
    ax2.set_ylim(990, 1030)
    ax2.set_ylabel("Pressure [hPa]")


    ax3.clear()
    ax3.plot(times, humidities)
    ax3.set_ylabel("Humidity [%]")
    ax3.set_xlabel("Time")

ani = animation.FuncAnimation(fig, update, interval=1000)
plt.tight_layout()
plt.show()