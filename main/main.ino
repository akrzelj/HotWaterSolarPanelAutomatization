#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Addresses of 3 DS18B20s
uint8_t sensor1[8] = { 0x28, 0xFF, 0x9A, 0xF3, 0x94, 0x16, 0x04, 0x2A }; //kesa
uint8_t sensor2[8] = { 0x28, 0xFF, 0x6D, 0x14, 0xA0, 0x16, 0x05, 0x0C };

/*
 * Function for rurning on and off relay
 * relayPin - pin where relay is connected
 * powerFlag - indication for turning on or off relay
 *             1 - turn on
 *             2 - turn off
 */
void relayPower(int relayPin, int powerFlag) {
  if (powerFlag == 1)
    digitalWrite(relayPin, 1);
  if (powerFlag == 2)
    digitalWrite(relayPin, 0);
}

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  
  Serial.print("Sensor 1 [kesa]: ");
  printTemperature(sensor1);
  
  Serial.print("Sensor 2 [van kese]: ");
  printTemperature(sensor2);
  
  Serial.println();
  delay(1000);
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print((char)176);
  Serial.print("C  |  ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.print((char)176);
  Serial.println("F");
}
