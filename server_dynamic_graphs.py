import serial
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Конфигурация последовательного порта
port = "COM11"  # USB порт для Arduino
baudrate = 9600

# Названия графиков
titles = [
    "Высота от времени",
    "Температура от времени (высоты)",
    "Влажность от времени (высоты)",
    "Давление от времени (высоты)",
    "Уровень дыма от времени (высоты)",
    "Уровень метана от времени (высоты)",
    "Уровень CO2 от времени (высоты)"
]

def process_data(data_str):
    try:
        data_values = [int(val) for val in data_str.strip().split(';')]
        timestamp = data_values[0]  # Время
        sensor_readings = data_values[1:]  # Показания датчиков
        return timestamp, sensor_readings
    except ValueError:
        print("Ошибка: Неверный формат данных. Убедитесь, что данные разделены точкой с запятой.")
        return None, None

# Инициализация хранилища данных
num_sensors = 7  # Количество датчиков
sensor_data = {sensor_num: [[], []] for sensor_num in range(1, num_sensors + 1)}
last_valid_readings = [None] * num_sensors  # Последние корректные значения

# Настройка графиков
figures = []
axes = []
for i in range(num_sensors):
    fig, ax = plt.subplots()
    fig.canvas.manager.set_window_title(titles[i])
    figures.append(fig)
    axes.append(ax)

def update_graphs(frame):
    ylabel = ["Высота", "Температура", "Влажность", "Давление", "Уровень дыма", "Уровень метана", "Уровень CO2"]
    data_str = ser.readline().decode("utf-8")
    timestamp, readings = process_data(data_str)
    
    if timestamp is not None and readings is not None:
        for i, reading in enumerate(readings):
            sensor_num = i + 1
            if reading > 0:
                sensor_data[sensor_num][0].append(timestamp)
                sensor_data[sensor_num][1].append(reading)
                last_valid_readings[i] = reading
            elif last_valid_readings[i] is not None:
                sensor_data[sensor_num][0].append(timestamp)
                sensor_data[sensor_num][1].append(last_valid_readings[i])
            
            ax = axes[i]
            ax.clear()
            ax.plot(sensor_data[sensor_num][0], sensor_data[sensor_num][1])
            ax.set_title(titles[i])
            ax.set_xlabel("Время (высота)")
            ax.set_ylabel(ylabel[i])

# Настройка последовательного порта
ser = serial.Serial(port, baudrate)
time.sleep(2)  # Пауза для установления соединения

# Запуск анимации
animations = [animation.FuncAnimation(fig, update_graphs, interval=3000) for fig in figures]
plt.show()
