/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios. 
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two 
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting 
 * with the serial monitor and sending a 'T'.  The ping node sends the current 
 * time to the pong node, which responds by sending the value back.  The ping 
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipe = 0xe8e8f0f0e1LL;


void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  //radio.setRetries(15,15);

  radio.openReadingPipe(1, pipe);

  radio.startListening();

  radio.printDetails();
}

void loop(void)
{
    float data[5] = { 0 };
    // if there is data ready
    if ( radio.available() )
    {
     
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( data, (sizeof(float) * 5) );

        // Spew it
        printf("Look I got data:");
        Serial.println(data[0]);
        Serial.println(data[1]);
        Serial.println(data[2]);
        Serial.println(data[3]);
        Serial.println(data[4]);

      }
    }

}
// vim:cin:ai:sts=2 sw=2 ft=cpp
