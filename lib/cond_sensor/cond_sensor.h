
#ifndef COND_SENSOR_H
#define COND_SENSOR_H

#include <Arduino.h>

void TWIInit();
void TWIStart();
void TWIStop();
void TWIWrite(uint8_t u8data);
uint8_t TWIReadACK();
uint8_t TWIReadNACK();
uint8_t TWIGetStatus();
uint16_t i2c_read_bytes(uint8_t addr);
void i2c_write_byte(uint8_t addr, uint8_t val2wr);
void loop1();
void setup1();

#endif