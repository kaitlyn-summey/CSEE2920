#include <SoftwareSerial.h>

#define RX 10
#define TX 11
#define touchPin 2

String AP = "wifi_name";       // CHANGE ME
String PASS = "wifi_password"; // CHANGE ME

String API = "thingspeak_key";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

int motion;

SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {
  Serial.begin(9600);
  pinMode(touchPin, INPUT);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
  int count = 0;
  int last;
  int data;
  motion = digitalRead(touchPin);
  if(motion == HIGH){
    data = 10;
  }else{
    count ++;
    if(count > 5){
      data = 0;
    }
  }
  
  

 String getData = "GET /update?api_key="+ API +"&field5="+String(data);

 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
