#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

/********** NRF24 Macros / Variables **********/
#define CE_PIN 9
#define CSN_PIN 10
const uint64_t read_pipe = 0xe8e8f0f0d1LL; // Receive address. This will
                                      // be the same as the Transmit address
                                      // on the Pi. 
int data[2] = { 0 }; // First int indicates the relay to change, 
                     // Second int indicates whether to turn on or off.
RF24 radio(CE_PIN, CSN_PIN);


/********** Relay Macros / Variables **********/
#define RELAY_PIN_ONE   2 // Relay pin one connected to digital pin 2, etc.
#define RELAY_PIN_TWO   3
#define RELAY_PIN_THREE 4
#define RELAY_PIN_FOUR  5




void setup() {
  Serial.begin(57600);
  Serial.println("Relays on radio.");
  
  printf_begin();
  radio.begin();
  radio.setRetries(15, 15); // Even though this is going to be RX only
  radio.openReadingPipe(1, read_pipe);
  radio.printDetails();
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}
void loop() {
  if(radio.available()) { //All the following code only runs when radio data is received.
    bool done = false;
    
    while(!done){
      radio.read(data, (sizeof(int) * 2));
      done = true;
    } 
    
    switch(data[0]){  // data[0] -- relay number
                      // data[1] -- on or off (1 or 0)
      case 1:         
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(2, LOW);
        } else if (data[1] == 0){
          digitalWrite(2, HIGH);
        } else {
          Serial.print("Received signal for Relay 1: ");
          Serial.println(data[1]);
        } 
        break;
        
      case 2:
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(2, LOW);
        } else if (data[1] == 0){
          digitalWrite(2, HIGH);
        } else {
          Serial.print("Received signal for Relay 2: ");
          Serial.println(data[1]);
        } 
        break;
        
      case 3:
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(2, LOW);
        } else if (data[1] == 0){
          digitalWrite(2, HIGH);
        } else {
          Serial.print("Received signal for Relay 3: ");
          Serial.println(data[1]);
        } 
        break;
        
      case 4:
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(2, LOW);
        } else if (data[1] == 0){
          digitalWrite(2, HIGH);
        } else {
          Serial.print("Received signal for Relay 1: ");
          Serial.println(data[1]);
        } 
        break;  
    }
  }
}
