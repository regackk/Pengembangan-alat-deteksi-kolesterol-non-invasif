#include <stdio.h>
#include <stdlib.h>
#include <SoftwareSerial.h>
#include <Wire.h> //library I2C
#include <LiquidCrystal_I2C.h> //library LCD
#include "MAX30100_PulseOximeter.h"
LiquidCrystal_I2C lcd(0x27, 16, 2); // set address I2C dan besar karakter untuk lcd 16×2
SoftwareSerial mySerial(9, 8); // RX, TX
char data = 0;
boolean mode = true;
boolean mode2;
boolean mode3;

const int relay1 = 2;
const int relay2 = 3;
const int relay3 = 4;
const int relay4 = 5;

#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

void onBeatDetected() {
    Serial.println("Beat!");
}

void setup(){
  Serial.begin(9600);
  Serial.print("Initializing pulse oximeter..");

  mySerial.begin(9600);

  // Initialize sensor
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop(){

// Read from the sensor
    pox.update();

    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }


if(mySerial.available() > 0)  
    {
      data = mySerial.read(); //baca data yang dikirim   
      if(mode == false){   
        if(data == 'A'){
          Serial.println("REGA");
          digitalWrite(relay1, LOW); //nyalakan relay
        }
        else if(data == 'a'){
          Serial.println("REGA GANTENG");      
          digitalWrite(relay1, HIGH); //matikan relay
        } 
        if(data == 'B'){
          digitalWrite(relay2, LOW); 
        }
        else if(data == 'b'){      
          digitalWrite(relay2, HIGH); 
        } 
        if(data == 'C'){
          digitalWrite(relay3, LOW); 
        }
        else if(data == 'c'){      
          digitalWrite(relay3, HIGH); 
          lcd.clear();
        }
    }
    if(data == 'D'){
//    digitalWrite(relay4, LOW); 
      mode = false;
    }
    else if(data == 'd'){    
      mode = true;  
//    digitalWrite(relay4, HIGH); 
    }                          
  }

  
}
