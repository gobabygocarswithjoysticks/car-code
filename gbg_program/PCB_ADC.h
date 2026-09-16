#ifdef IS_PCB
#include <Wire.h>

const uint8_t sensorAddress = 0x48;
boolean pcbSensorsAvailable = true;
uint32_t lastSensorMillis = 0;
boolean newI2CData = false;

float batVoltage = -1;
float leftCurrent = -1;
float rightCurrent = -1;

const float batVoltMultiplier = 13.92;
const float leftCurrentMultiplier = 6150.0 / 604;
const float rightCurrentMultiplier = 6150.0 / 604;

uint8_t i2cWriteBuffer[4];
uint8_t i2cReadBuffer[4];

int8_t adcCounter = -1;

const int8_t NUM_I2C_ADC_CHANNELS = 3;
const int NUM_I2C_ADC_STEPS = 3;

#define ADC_CONF_RESERVED 0x03
#define ADC_CONF_DATA_RATE 0x03
#define ADC_CONF_MODE 0b1 // single shot
#define OS_POSN 15

const int8_t ADC_CONF_I2C_REGISTER = 0x01;
const int8_t ADC_CONV_I2C_REGISTER = 0x00;

uint16_t configs[NUM_I2C_ADC_CHANNELS] = {
  ADC_CONF_RESERVED | ADC_CONF_DATA_RATE << 5 | ADC_CONF_MODE << 8 | 0x02 << 9 | 0b101 << 12 | (0b1 << OS_POSN), //ain1
  ADC_CONF_RESERVED | ADC_CONF_DATA_RATE << 5 | ADC_CONF_MODE << 8 | 0x02 << 9 | 0b110 << 12 | (0b1 << OS_POSN), //ain2
  ADC_CONF_RESERVED | ADC_CONF_DATA_RATE << 5 | ADC_CONF_MODE << 8 | 0x02 << 9 | 0b111 << 12 | (0b1 << OS_POSN), //ain3
  //  0x03 | 0x04 << 5 | 0x01 << 8 | 0x02 << 9 | 0b100 << 12 | 0x01 << 15, // Ain0, spare channel
};

void onI2CFinish() {
  //  Serial.print("i2cDone ");
  //  Serial.println(adcCounter);
  if ((adcCounter % NUM_I2C_ADC_STEPS) == 2) { // 3rd step of 3, after requesting conv data
    //    Serial.println("got data");
    int16_t newData = i2cReadBuffer[1] | (i2cReadBuffer[0] << 8);
    //    Serial.println(newData);
    if (newData & 0x8000) {
      newData >>= 4;
      newData |= 0x8000;
    } else {
      newData >>= 4;
    }
    switch (adcCounter / NUM_I2C_ADC_STEPS) {
      case 0: // right motor
        rightCurrent = (newData) * rightCurrentMultiplier / 1000;
        lastSensorMillis = millis();
        newI2CData = true;
        break;
      case 1: // battery
        batVoltage = (newData) * batVoltMultiplier / 1000;
        lastSensorMillis = millis();
        newI2CData = true;
        break;
      case 2: // left motor
        leftCurrent =  (newData) * leftCurrentMultiplier / 1000;
        lastSensorMillis = millis();
        newI2CData = true;
        break;
    }
  }
}

void setupPCBSensors() {
  Wire1.setSDA(14);
  Wire1.setSCL(15);
  Wire1.onFinishedAsync(onI2CFinish);
  Wire1.begin();
  Wire1.setTimeout(25);
}

void runPCBSensors() {
  if (pcbSensorsAvailable == true && millis() - lastSensorMillis > 100) {
    pcbSensorsAvailable = false;
  }
  if (Wire1.finishedAsync()) {
    adcCounter++;
    if (adcCounter >= NUM_I2C_ADC_STEPS * NUM_I2C_ADC_CHANNELS) {
      adcCounter = 0;
      // did full cycle, if the adc was responsive say that pcb sensor data is now available
      if (pcbSensorsAvailable == false && millis() - lastSensorMillis <= 100) {
        pcbSensorsAvailable = true;
      }
    }
    //    Serial.println(adcCounter);

    if ((adcCounter % NUM_I2C_ADC_STEPS) == 0) { // send config
      i2cWriteBuffer[0] = ADC_CONF_I2C_REGISTER;
      i2cWriteBuffer[1] = highByte(configs[adcCounter / NUM_I2C_ADC_STEPS]);
      i2cWriteBuffer[2] = lowByte(configs[adcCounter / NUM_I2C_ADC_STEPS]);
      Wire1.writeAsync(sensorAddress, i2cWriteBuffer, 3, true);
    }
    else if ((adcCounter % NUM_I2C_ADC_STEPS) == 1) { // begin poll for done bit
      //      Serial.println("start of reading os");
      i2cWriteBuffer[0] = ADC_CONF_I2C_REGISTER;
      Wire1.writeReadAsync(sensorAddress, i2cWriteBuffer, 1, i2cReadBuffer, 2, true);
    }
    else if ((adcCounter % NUM_I2C_ADC_STEPS) == 2) { // 3rd step of 4, polling for done flag
      uint16_t newData = i2cReadBuffer[1] | (i2cReadBuffer[0] << 8);
      //      Serial.println(newData, BIN);
      if ((newData >> OS_POSN) & 1) { //OS BIT == 1 means done
        // read data from CONV
        i2cWriteBuffer[0] = ADC_CONV_I2C_REGISTER;
        //        Serial.print("starting read from conv, data was: ");
        //        Serial.println(newData, BIN);
        Wire1.writeReadAsync(sensorAddress, i2cWriteBuffer, 1, i2cReadBuffer, 2, true);
      } else {
        // poll OS again
        adcCounter--;
        //        Serial.print("polling os, data was: ");
        //        Serial.println(newData, BIN);
        i2cWriteBuffer[0] = ADC_CONF_I2C_REGISTER;
        Wire1.writeReadAsync(sensorAddress, i2cWriteBuffer, 1, i2cReadBuffer, 2, true);
      }
    }
  }
}

#endif
