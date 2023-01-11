#include "DHT.h"
#include "OneWire.h"
#include "DallasTemperature.h"

const int AirValue = 620;
const int WaterValue = 310;

String nom = "Arduino";
String msg;


struct parsedMsg {
    String type;
    String pin;
  };

void setup() {
  Serial.begin(9600);
}

void loop() {
  readSerialPort();

  if (msg != "") {
    struct parsedMsg parsed;
    parsed = message_to_struct(msg);
    //parsed.type == "dht22"
    //Serial.print("TRY");
    //Serial.print(parsed.type);
    //Serial.print(parsed.pin);
    if (parsed.type == "dht22" or parsed.type == "dht11") {
      readDHT(parsed.pin, parsed.type);
    }
    if (parsed.type == "soil") {
      readSoilHumidity(parsed.pin);
    }
    if (parsed.type == "waterTemp"){
      readWaterTemp(parsed.pin);
      } 
    //sendData();
  }
  delay(500);
}

void readSerialPort() {
  msg = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      msg += (char)Serial.read();
    }
    Serial.flush();
  }
}

void sendData() {
  //write data
  Serial.print("Arduino");
  Serial.print(msg);
}

struct parsedMsg message_to_struct(String msg) {
  struct parsedMsg parsed;
  uint8_t i=0, j=0;
  String Ary[2];
  Ary[0] = "";
  Ary[1] = "";
  while (j<msg.length()){
    if (msg.charAt(j) == '_'){
      i++;
    } else {
        Ary[i] = Ary[i] + msg.charAt(j);
    }
    j++;
  }
  parsed.type = Ary[0];
  parsed.pin = Ary[1];
  return parsed;
  }

void makeAndSendResponse(String pin, String type, String value) {
  String response = pin + "_" + type + "_" + value;
  Serial.print(response);
  }

void readDHT(String pin, String type) {
  int intPin = pin.toInt();
  uint8_t intType = 0;
  String value = "";
  if (type == "dht22"){
    intType =  DHT22;
  } else {
    intType = DHT11;
  }
  DHT dht(intPin, intType);
  dht.begin();
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(isnan(t) or isnan(h)){
      value = "error";
  }else {
      value = "value#h:" + String(h) + ";t:" + String(t);
  }
  
  makeAndSendResponse(pin, type, value);
  return;
  }

bool AnalogPin(String pin) {
  if (pin.startsWith("0") or pin.startsWith("1") or pin.startsWith("2") or pin.startsWith("3") or pin.startsWith("4")){
    return true;
  } else {
    return false;
  }
}

int stringToAnalogPin(String pin) {
  if (pin.startsWith("0")) {
    return A0;
  }
  if (pin.startsWith("1")) {
    return A1;
  }
  if (pin.startsWith("2")) {
    return A2;
  }
  if (pin.startsWith("3")) {
    return A3;
  }
  if (pin.startsWith("4")) {
    return A4;
  }
  return  A5;
 }

void readSoilHumidity(String pin) {
  int soilMoisture = 0;
  int soilMoisturePercent = 0;
  bool analogPin = AnalogPin(pin);
  if (analogPin) {
    soilMoisture = analogRead(stringToAnalogPin(pin));
    soilMoisturePercent = map(soilMoisture, AirValue, WaterValue, 0, 100);
    makeAndSendResponse(pin, "soil", "value#" + String(soilMoisturePercent));
  } else {
    makeAndSendResponse(pin, "soil", "error");
    }
  return;
  }

void readWaterTemp(String pin){
  OneWire oneWireObj(pin.toInt());
  DallasTemperature sensorDS18B20(&oneWireObj);
  String value = "";
  double temperature = 0;
  String sTemperature = "";
  sensorDS18B20.begin();
  sensorDS18B20.requestTemperatures();

  temperature = sensorDS18B20.getTempCByIndex(0);
  if (temperature < 0 or temperature > 100){
    value = "error";
  } else {
    sTemperature = String(temperature, 2);
    value = "value#" + sTemperature;
  }

  makeAndSendResponse(pin, "waterTemp", value);
  return;
}
