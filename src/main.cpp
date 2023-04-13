#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "10276";
int message = -1, cstart=-1, launchThresh = 10;
float cdown = 0.0;
const int pyro1 = 7, led=6, buzz=5;

void listen(){
  radio.openReadingPipe(0, address);
  radio.startListening();
  if(radio.available()){
    radio.read(&message,sizeof(message));
  }
}

void write(char text[100]){
  radio.openWritingPipe(address);
  radio.stopListening();
  radio.write(&text, sizeof(text));
}

void ignition(){
  digitalWrite(pyro1, HIGH);
  Serial.println("IGNITION!!!");
  write("Ignition Successful");
  digitalWrite(led, LOW);
  digitalWrite(buzz, LOW);
  cdown=0;
  cstart=0;
}

void abort(){
  cstart=-1;
  cdown=-1;
}

void setup() {
  pinMode(pyro1, OUTPUT);
  digitalWrite(pyro1, LOW);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz, LOW);
  radio.begin(); 
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
}

void loop() {
  listen();
  if(message==1 && cdown==-1){
    cstart = millis();
    write("Countdown Started");
    digitalWrite(led, HIGH);
    digitalWrite(buzz, HIGH);
  }
  if(message==2){
    abort();
    write("ABORTED");
    digitalWrite(led, LOW);
    digitalWrite(buzz, LOW);
  }
  if(cstart>0){
    cdown=(millis()-cstart)/1000;
  }
  if(millis()-cstart>=launchThresh){
    ignition();
  }
}