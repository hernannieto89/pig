#include "DHT.h"
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
    // parsed.type == "dht22"
    //Serial.print("TRY");
    //Serial.print(parsed.type);
    //Serial.print(parsed.pin);
    if (parsed.type == "dht22" or parsed.type == "dht11") {
      readDHT(parsed.pin, parsed.type);
    }
    if (parsed.type == "soil") {
      readSoilHumidity(parsed.pin);
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
  String response = "{'pin': " + pin + ", 'type': " + type + ", 'response': " + value + "}";
  Serial.print(response);
  }

void readDHT(String pin, String type) {
  int intPin = pin.toInt();
  uint8_t intType = 0;
  String value = "";
  if (type == "dht22"){
    intType == DHT22;
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
      value = "{ 'h': " + String(h) + ", 't': " + String(t) + "}";
  }
  
  makeAndSendResponse(pin, type, value);
  return;
  }

void readSoilHumidity(String pin) {
  return;
  }
