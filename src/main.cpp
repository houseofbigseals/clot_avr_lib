#include <Arduino.h>
#include "test_relay_protocol.h"

/*
#include "slave.h"
#include "clot_protocol.h"
#include "relay_unit.h"


clot_relay_unit relay_ch1 = clot_relay_unit(0x01, 2);
clot_relay_unit relay_ch2 = clot_relay_unit(0x02, 3);
clot_relay_unit relay_ch3 = clot_relay_unit(0x03, 4);
clot_relay_unit relay_ch4 = clot_relay_unit(0x04, 5);
clot_relay_unit relay_ch5 = clot_relay_unit(0x05, 6);
clot_relay_unit relay_ch6 = clot_relay_unit(0x06, 7);

const uint8_t device_len = 6;
base_unit* devarr[device_len] = {&relay_ch1, &relay_ch2, &relay_ch3, &relay_ch4,
&relay_ch5, &relay_ch6};

clot_slave main_slave = clot_slave(0x01, devarr, device_len, &Serial);

void setup() 
{	
	Serial.begin(9600);
	//Serial.println("hah");
}

void loop() 
{
	main_slave.do_main_loop();
}
*/

void setup()
{
	setup1();
}

void loop()
{
	loop1();
}
