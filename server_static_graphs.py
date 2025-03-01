import matplotlib.pyplot as plt

# Массивы для хранения данных
time_data = []  # Время
altitude = []  # Высота
temperature = []  # Температура
humidity = []  # Влажность
pressure = []  # Давление
smoke = []  # Уровень дыма
methane = []  # Уровень метана
co2 = []  # Уровень CO2

# Заполнение массивов данных из файла
with open('data.txt', 'r') as file:
    lines = file.readlines()
    for line in lines:
        line_data = line.strip().split(';')
        if len(line_data) == 8:  # Убеждаемся, что строка содержит все данные
            try:
                time_data.append(float(line_data[0]))
                altitude.append(float(line_data[1]))
                temperature.append(float(line_data[2]))
                humidity.append(float(line_data[3]))
                pressure.append(float(line_data[4]))
                smoke.append(float(line_data[5]))
                methane.append(float(line_data[6]))
                co2.append(float(line_data[7]))
            except ValueError:
                print(f"Ошибка обработки строки: {line}")

# Данные для графиков
graph_data = [
    (altitude, 'Высота', 'График зависимости высоты от времени'),
    (temperature, 'Температура', 'График зависимости температуры от времени'),
    (humidity, 'Влажность', 'График зависимости влажности от времени'),
    (pressure, 'Давление', 'График зависимости давления от времени'),
    (smoke, 'Уровень дыма', 'График зависимости уровня дыма от времени'),
    (methane, 'Уровень метана', 'График зависимости уровня метана от времени'),
    (co2, 'Уровень CO2', 'График зависимости уровня CO2 от времени')
]

# Построение графиков
for data_list, ylabel, title in graph_data:
    plt.figure()
    plt.plot(time_data, data_list)
    plt.xlabel("Время")
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid()
    plt.show()
