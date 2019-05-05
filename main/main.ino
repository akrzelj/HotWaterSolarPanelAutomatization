#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define RELAY_PIN    7

float temperatureOut = 0;
float temperatureIn  = 0;
bool  relayOn        = false;
int   errorCounter   = 0;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Addresses of 3 DS18B20s
uint8_t sensorOut[8] = { 0x28, 0xFF, 0x9A, 0xF3, 0x94, 0x16, 0x04, 0x2A }; //kesa
uint8_t sensorIn[8] = { 0x28, 0xFF, 0x6D, 0x14, 0xA0, 0x16, 0x05, 0x0C };

/**
 * Function controls relay way of work 
 * if tempOut - tempIn > 15 => pump ON
 * if tempOut - tempIn < 5  => pump OFF
 * @param temparatureOut temparature read by sensor which is deployed on solar panel
 * @param temparatureIn  temparature read by sensor which is deployed on boiler
 * @param relayOn        indication for pump operational status, relayOn = true => pump ON, relayOn == false => pump OFF
 */
void RelayController(float temperatureIn, float temperatureOut, bool* relayOn) {
  if (temperatureOut - temperatureIn > 10) {
    *relayOn = true;
    digitalWrite(RELAY_PIN, LOW);
  }
  
  if(temperatureOut - temperatureIn < 5) {
      *relayOn = false;
      digitalWrite(RELAY_PIN, HIGH);
    }
}

/**
 * Function returns temperature in Celisius from sensor with provided address and print fetched values
 * @param sensor address
 * @return temp in Celisius
 */
float getTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print((char)176);
  Serial.print("C  |  ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.print((char)176);
  Serial.println("F");  

  return tempC;
}

void setup(void)
{
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  
  Serial.print("Sensor 1 [kesa/vani]: ");
  temperatureOut = getTemperature(sensorOut);
  
  Serial.print("Sensor 2 [van kese/unutra]: ");
  temperatureIn = getTemperature(sensorIn);
 
  if (((int)temperatureIn != -127) && ((int)temperatureOut != -127) && ((int)temperatureIn != 85) && ((int)temperatureOut != 85)) { //check for some specific invalid measurements
    bool var = (int)temperatureIn != -127;
    Serial.println("");
    errorCounter = 0;
    RelayController(temperatureIn, temperatureOut, &relayOn);
  } else {
    errorCounter++;
      Serial.print("Error counter ");
      Serial.println(errorCounter);
      Serial.println();
  
    if(errorCounter >= 5) {
      Serial.print("Gasim pumpu zbog velikog broja uzastopnih krivih mjerenja. Broj mjerenja: ");
      Serial.println(errorCounter);
      relayOn = false;
      digitalWrite(RELAY_PIN, HIGH);
    }
  }
  delay(5000);
}
