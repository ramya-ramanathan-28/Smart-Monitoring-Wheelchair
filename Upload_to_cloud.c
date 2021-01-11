

#include <SoftwareSerial.h>
#include <stdlib.h>

// LED
int ledPin = 13;

int output_Pin = A0;                                // initializing the sensor output pin

double alpha = 0.75;

int period = 200;

double change = 0.0;

String apiKey = "T2RJXWQAVXG4ZV39";


SoftwareSerial ser(10, 11); // RX, TX


void setup() {                
  
  pinMode(ledPin, OUTPUT);    

  
  Serial.begin(9600);
  
  ser.begin(9600);

 
  ser.println("AT+RST");
}



void loop() {

  
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);
  

    

    static double oldValue = 0;

    static double oldChange = 0;

    int rawValue = analogRead (output_Pin);                                         // Reading the sensors values

    double value = alpha * oldValue + (1 - alpha) * rawValue;         // calculating values using the formula




   

    oldValue = value;

    delay (period);

  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  char buf[16];

  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(dtostrf(value,4,1,buf));
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

  // thingspeak needs 15 sec delay between updates
  delay(16000);  
}