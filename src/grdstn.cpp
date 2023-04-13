#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "10276";
char text[100] = "";
const int btn = 8;
int signal=0;

void listen(){
    radio.openReadingPipe(0, address);
    radio.startListening();
    if(radio.available()){
        radio.read(&text,sizeof(text));
        Serial.println(text);
    }
}

void write(int data){
    radio.openWritingPipe(address);
    radio.stopListening();
    radio.write(&data, sizeof(data));
}

void setup() {
    pinMode(btn, INPUT_PULLUP);
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
}

void loop() {
    signal = digitalRead(btn);
    if(signal){
        write(1);
    }
}