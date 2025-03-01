#include <MPU9250_asukiaaa.h>
#include <Adafruit_BMP280.h>
#include <iarduino_Pressure_BMP.h> 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MQ135.h>
#include <DHT.h>
#define DHTPIN 27
#define DHTTYPE DHT11
// Определяем пины подключения датчиков
#define MQ2_PIN 4
#define MQ135_PIN 0
DHT dht(DHTPIN, DHTTYPE);
File myFile;
const int CS = 5;
#define BUZZER_PIN 26





// Библиотека MPU9250
MPU9250_asukiaaa mpu;

// Библиотека BMP280
Adafruit_BMP280 bmp280;

// Библиотека GPS
TinyGPSPlus gps;



// Конфигурация SoftwareSerial для GPS
//МОЖНО ПОМЕНЯТЬ СРОЧНО
// СРОЧНО
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);

const float baseLat = 51.076554;
const float baseLon = 71.383502;

// Константы BMP280
#define SDA_PIN 21
#define SCL_PIN 22
#define BMP280_ADDRESS 0x76

// Давление на уровне моря (по умолчанию)
double seaLevelPressure = 760.0;
int sec = 1;
// Переменные для хранения данных
float aX, aY, aZ, aSqrt, gX, gY, gZ, mX, mY, mZ, mDirection;
double temp, pres, density, altitude;
float latitude, longitude;

void AppendFile(const char * path, const String& message){
  // открыть файл в режиме добавления. 
  // Заметьте, что только один файл может быть открыт в данный момент,
  // поэтому вам нужно закрыть этот перед открытием другого.
  myFile = SD.open(path, FILE_APPEND);
  // если файл успешно открыт, записать в него:
  if (myFile) {
    tone (BUZZER_PIN, 300, 100);
    Serial.printf("Writing to %s ", path);
    myFile.println(message);
    myFile.close(); // закрыть файл:
    Serial.println("completed.");
  }
  // если файл не удалось открыть, вывести ошибку:
  else {
    Serial.println("error opening file ");
    Serial.println(path);
  }
}

void setup() {
  // Инициализация Serial
  Serial.begin(9600);
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); 
  while (!Serial) { ; }  // wait for serial port to connect. Needed for native USB port only
  Serial.println("Initializing SD card...");
  if (!SD.begin(CS)) {
    Serial.println("initialization failed!");
    return;
  }
  // Инициализация Wire (I2C)
  Wire.begin(SDA_PIN, SCL_PIN);

  


  // Инициализация MPU9250
  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  mpu.beginMag();

  // Инициализация BMP280
  bmp280.begin(BMP280_ADDRESS);

  // Инициализация SoftwareSerial для GPS
  ss.begin(9600);

  // Задержка для стабилизации датчиков
  delay(1000);
}

void loop() {
  // tone (BUZZER_PIN, 500); 
  // Чтение данных MPU9250
  if (mpu.accelUpdate() == 0) {
    aX = mpu.accelX();
    aY = mpu.accelY();
    aZ = mpu.accelZ();
    aSqrt = mpu.accelSqrt();
  }
  if (mpu.gyroUpdate() == 0) {
    gX = mpu.gyroX();
    gY = mpu.gyroY();
    gZ = mpu.gyroZ();
  }
  if (mpu.magUpdate() == 0) {
    mX = mpu.magX();
    mY = mpu.magY();
    mZ = mpu.magZ();
    mDirection = mpu.magHorizDirection();
  }
  //mq4 and methan
  int mq2_value = analogRead(MQ2_PIN);
  int mq135_value = analogRead(MQ135_PIN);
  float mq2_concentration = map(mq2_value, 0, 1023, 0, 100); // Примерное преобразование
  float mq135_concentration = map(mq135_value, 0, 1023, 0, 100); // Примерное преобразование
  float humidity = dht.readHumidity();



  // Чтение данных BMP280
  temp = bmp280.readTemperature();
  pres = bmp280.readPressure();

  // Расчет высоты и плотности
  density = (29.0 * pres) / ((temp + 273.15) * 287.058);
  altitude = 44330.0 * (1.0 - pow(pres / 101325.0, 0.1903));

  // Чтение данных GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // Вывод данных, только если GPS-фиксация валидна
  if (gps.location.isValid()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();

  } else {
    latitude = baseLat;
    longitude = baseLon;
  }
    Serial.println(altitude);
    if(altitude > 310){
      tone (BUZZER_PIN, 1000);
    }

    String dataString =String(sec) + "," + String(altitude) + "," +  String(temp) + "," + String(humidity) + "," + String(pres) + "," + String(mq135_concentration) + "," + String(mq2_concentration) + "," + String(density)  + "," + String(latitude) + "," + String(longitude)  ;
    AppendFile("/data.txt", dataString);
    // Вывод объединенных данных
    // Serial.print("*Данные датчиков:*");
    // Serial.println();
    // Serial.print("*Температура:* ");
    // Serial.print(temp);
    // Serial.print(" *C");
    // Serial.print("\t*Давление:* ");
    // Serial.print(pres);
    // Serial.println(" Pa");
    // Serial.print("*Высота:* ");
    // Serial.print(altitude);
    // Serial.println(" м");
    // Serial.println();
    // Serial.print("*Данные GPS:*");
    // Serial.println();
    // Serial.print("*Широта:* ");
    // Serial.println(latitude);
    // Serial.print("*Долгота:* ");
    // Serial.println(longitude);
  // Задержка
  sec++;
  delay(1000);
}