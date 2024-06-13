#include <Arduino.h>
#include <Wire.h> 

#include <Arduino_LSM6DS3.h>

#define ADC_ADDRESS 0x6C // address given by datasheet
#define MESSAGE_SIZE 2 // in bytes



#define CONFIGURATION_BYTE 0x03 //// this is: 00000011


#define SETUP_BYTE 0x82 //this is: 10000010 
// you can change these, just look at table 1 "setup byte format"




void writeRegister(uint8_t value) {
    Wire2.beginTransmission(ADC_ADDRESS);
    Wire2.write(value);
    Wire2.endTransmission();
}


void readMultiRegister(uint8_t* data, size_t length) {
    // Serial.print("Requesting: ");
    Wire2.requestFrom(ADC_ADDRESS, length);
    // size_t requested = Wire.requestFrom((uint8_t) LSM6DS3_ADDRESS, (size_t) length, true);
    // Serial.println(requested);
    for (size_t i = 0; i < length; i++) {
        data[i] = Wire.read();
    }
}

void setup() {
  Serial.begin(115200);


  Wire2.begin();

  // configure the registers:
  writeRegister(CONFIGURATION_BYTE);

  writeRegister(SETUP_BYTE);

  // no we've finished configuring, I think i've written the config register to deliver both readings

}

void loop() {
  // read 

  uint8_t reading[4]; // this will be 4 byte transaction: first 2 bytes should be AIN0, next 2 bytes should be AIN1


// read output
  readMultiRegister(reading, 4);

  // now, we will convert what we just read into the two separate readings

  
  // no reading[4] is going to store your numbers, and I think 

  // in i2c we do MSB first

  uint16_t AIN0 = (reading[0] << 8) + reading[1];

  uint16_t AIN1 = (reading[2] << 8) + reading[3];

  Serial.print(AIN0);
  Serial.print(",");
  Serial.println(AIN1);

  double tempTime = micros();

  while(micros()-tempTime < 500000); // half second delay for printing

  //SLIGHT CONCERN: the datasheet says the teensy has to send an acknowledgement in order for the ADC to keep sending the data, i think this should happen internally though, and i wouldn't know how to take care of htat anyway. So this is a "last resort to look at" kind of bug, chances are that if there are issues, it's something else

}

