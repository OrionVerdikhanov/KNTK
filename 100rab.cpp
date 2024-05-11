#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SoftwareSerial.h>

Adafruit_BME280 bme;
SoftwareSerial sim900(7, 8);

void setup() {
  Serial.begin(9600);
  sim900.begin(9600);

  if (!bme.begin(0x76)) {
    Serial.println("Ошибка инициализации BME280!");
    // Вместо бесконечного цикла, сообщаем об ошибке
    return; // Выходим из setup()
  }
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  // Уменьшаем задержку для демонстрации, но в реальном приложении может потребоваться другая логика
  delay(1000); // Сокращаем задержку

  sim900.println("AT");
  delay(1000); // Сокращаем задержку для примера
  sim900.println("AT+CGATT=1");
  delay(1000);
  sim900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);
  sim900.println("AT+SAPBR=3,1,\"APN\",\"internet.tele2.ru\"");
  delay(1000);
  sim900.println("AT+SAPBR=1,1");
  delay(1000);
  sim900.println("AT+SAPBR=2,1");
  delay(1000);
  
  String queryParams = "?temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&pressure=" + String(pressure);
  String fullURL = "http://verdikhanov.ru/upload.php" + queryParams;
  
  sim900.println("AT+HTTPINIT");
  delay(1000);
  sim900.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  sim900.println("AT+HTTPPARA=\"URL\",\"" + fullURL + "\"");
  delay(1000);
  sim900.println("AT+HTTPACTION=0");
  delay(5000); // Даем больше времени на выполнение HTTP запроса
  sim900.println("AT+HTTPTERM");
  delay(1000);

  while (sim900.available()) {
    Serial.write(sim900.read());
  }

  delay(55555); // Сокращаем общую задержку между циклами отправки данных
}
