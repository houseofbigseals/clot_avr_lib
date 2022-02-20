
#ifndef clot_slave_h
#define clot_slave_h

#define RS485Transmit HIGH  //for future using in rs485 mode
#define RS485Receive LOW

#include <Arduino.h>
#include "clot_protocol.h"
#include "base_unit.h"
//#include <SoftwareSerial>

class clot_slave
{
    // this class realizes all methods to check and verify packages
    //
    // also it contains pointers to all units
    //
    // it can create first checks of packages integrity
    // and trying to find what unit is a destination of this package
    // if it cant find desired unit or package is broken it does nothing
    // if it can, it put command to correspond unit
    // then it takes back results and creates slave message from unit and sends it to master

    public:
    void parse_package(uint8_t * package, uint16_t len);
    //void send_master_message(master_message mm);
    void send_slave_message(slave_message sm_);
    slave_message parse_master_message(master_message mm);
    slave_message create_error_message(uint8_t error_code, uint8_t unit_id);
    //void parse_slave_message(slave_message sm);
    //clot_slave(base_unit **device_pointer_, uint16_t devices_number_,
    //SoftwareSerial * s, uint16_t sspin, bool debug = false);

    clot_slave(uint8_t slave_addr, base_unit **units_pointer_, uint16_t units_number_, HardwareSerial * s_,
    bool debug = false);

    void do_main_loop();
    uint8_t slave_address;

    //protected:
    //bool D_RS485_DEBUG_OUTPUT_ALLOWED; // debug flag
    base_unit **_units_pointer; // pointer to array of pointers to devices 
    uint16_t _units_number; // number of devices
    HardwareSerial * _s; // pointer to hardvareserial object for uart communicating

    // ss and its pin will be used in rs485 version
    //SoftwareSerial * _s; // pointer to software serial to real messaging
    //uint16_t software_serial_pin; // pin to go from read mode to write mode in max485 
};


#endif