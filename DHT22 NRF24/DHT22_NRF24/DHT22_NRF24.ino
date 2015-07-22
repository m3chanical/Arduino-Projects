
#include "DHT.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// DHT22 Macros
#define DHTPIN 3     
#define DHTTYPE DHT22

// NRF24 Macros
#define CE_PIN 9
#define CSN_PIN 10
#define DATA_SIZE 5

// TMP36:
#define TMP_PIN 3


DHT dht(DHTPIN, DHTTYPE);
RF24 radio(CE_PIN, CSN_PIN);

const uint64_t pipe = 0xe8e8f0f0e1LL; // Transmist pipe address
float data[DATA_SIZE] = { 0 }; // 5 element float array for temperature/humidity data
                       // (4 bytes * 5 = 20 bytes, radio can transmit 32 bytes at one time)

void setup() {
  Serial.begin(57600);
  Serial.println("DHT22 with Radio!");
  printf_begin();

  dht.begin();
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(pipe);
  
  radio.printDetails();
}

void loop() {
  // Wait a few seconds between measurements.
  int i;
  
  /* For some reason delaying the Arduino without powering the radio off, then on
      will mess with the transmit. The delay is needed to wait to read from the 
      DHT22 sensor. Will research...
  */
  radio.powerDown();
  delay(2000);
  radio.powerUp();
  
  // TMP36 read:
  int reading = analogRead(TMP_PIN);
  Serial.println(reading);
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  
  float temperature_Celsius = (voltage - 0.5) * 100;
  float temperature_Fahrenheit = (temperature_Celsius * (9.0 / 5.0)) + 32.0;
  

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  data[0] = dht.readHumidity();
  // Read temperature as Celsius (the default)
  data[1] = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  data[2] = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(data[0]) || isnan(data[1]) || isnan(data[2])) {
    Serial.println("Failed to read from DHT sensor!");
    data[0] = data[1] = data[2] = -1;
  }

  // Compute heat index in Fahrenheit (the default)
  data[3] = temperature_Fahrenheit;
  // Compute heat index in Celsius (isFahreheit = false)
  data[4] = temperature_Celsius;

  Serial.println("***********************************");
  Serial.println("");
  Serial.print("Humidity: ");
  Serial.print(data[0]);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(data[1]);
  Serial.print(" *C ");
  Serial.print(data[2]);
  Serial.print(" *F\t");
  Serial.print("TMP 36: ");
  Serial.print(data[3]);
  Serial.print(" Volts ");
  Serial.print(data[4]);
  Serial.println(" *C");
  
  printf("Now sending data:\n");
  for(i = 0; i < DATA_SIZE; i++){
    Serial.print(data[i]);
    if(i == (DATA_SIZE - 1)){
      printf("\n");
    } else {
      printf("\t");
    }
  }
  
  bool ok = radio.write(&data, (sizeof(float) * DATA_SIZE));
  
  if(ok) {
    printf("Data Sent!\n");
  } else {
    printf("Data failed to send!\n");
  }
  
}
