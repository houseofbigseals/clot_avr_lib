#include <Arduino.h>
#include "test_relay_protocol.h"

const int BUFFER_SIZE = 100;
char buf[BUFFER_SIZE];
const uint8_t ch1_pin = 2;
const uint8_t ch2_pin = 3;
const uint8_t ch3_pin = 4;
const uint8_t ch4_pin = 5;
const uint8_t ch5_pin = 6;
const uint8_t ch6_pin = 7;

const uint8_t ch1_led = 8;
const uint8_t ch2_led = 9;
const uint8_t ch3_led = 10;
const uint8_t ch4_led = 11;
const uint8_t ch5_led = 12;
const uint8_t ch6_led = 13;

uint8_t leds[6] = {ch1_led, ch2_led, ch3_led, ch4_led, ch5_led, ch6_led};
uint8_t pins[6] = {ch1_pin, ch2_pin, ch3_pin, ch4_pin, ch5_pin, ch6_pin};

String error_string = "error";
String not_my_work = "wrong_slave_addr";
String job_done = "job_done";
//String wrong_start_byte = "1";
//String wrong_relay_num = "2";
//String wrong_relay_state = "3";


char slave_addr = '1';
char start_byte = 'a';
char end_byte = 'g';


String parse_command(String com)
{
    if (com[1] != slave_addr)
    {
        return not_my_work;
    }
    else
    {
        int ch_num = (int)com[2] - 48; // stupid converting char digit to int
        int ch_state = (int)com[3] - 48;
        digitalWrite(pins[ch_num -1], ch_state);
        digitalWrite(leds[ch_num -1], !ch_state);
        return job_done;
    }
}

void setup1()
{
    for(int i; i < 6; i++)
    {
        pinMode(leds[i], OUTPUT);
        pinMode(pins[i], OUTPUT);
        digitalWrite(leds[i], LOW);
        digitalWrite(pins[i], HIGH);
    }
    Serial.begin(9600);

}

void loop1()
{
    while(true)
    {
        if (Serial.available() > 0)
        {
            String s = Serial.readStringUntil('\n');
            /*
            This is very simple example 
            Only for stupid relay
            We are awaiting string like: "agbcd\n" where:
            'a' is a start byte, forever
            'g' is addr for slave device 
            'd' is end byte forever
            'b' is number of relay channel 1-6
            and 'c' is relay status - 0 or 1
            0 is 0 on relays, 1 is high on relays

            so for example a130d\n is command for slave 1
            to set relay 3 to state 0

            answer:
            there is two answers - echo and reaction
            echo is simply received command
            reaction is 
            String error_string = "error";
            String not_my_work = "wrong_slave_addr";
            String job_done = "job_done";

            */


            //Serial.println("ffffuuuu");
            Serial.println(s);
            String answer = parse_command(s);
            Serial.println(answer);
        }

    }
}