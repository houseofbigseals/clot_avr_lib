
#ifndef clot_protocol_h
#define clot_protocol_h

#include <stdint.h>


// ========================= pseudo documentation =============================

/*
    package from master to device - 6 bytes
    
    | MASTER_START_BYTE | SLAVE_ADDR | UNIT_ADDR | COMMAND | CRC16_LOW | CRC16_HIGH |
    MASTER_START_BYTE - 1 byte - everytime is 0xCA
    SLAVE_ADDR - 1 byte - to what slave device
    UNIT_ADDR - byte - to ehat unit on slave device
    COMMAND - 1 byte - what to do - different for all units
    CRC16 - 2 bytes - crc16 sum of package

    -----------------------------------------------------------------------------------

    package from slave to master - 10 bytes
    | SLAVE_START_BYTE | SLAVE_ADDR | UNIT_ADDR | STATUS | RESULT1 | RESULT2 | RESULT3 | RESULT4 |
     CRC16_LOW | CRC16_HIGH |

    SLAVE_START_BYTE - 1 byte - everytime is 0xAC
    SLAVE_ADDR - 1 byte - to what slave device
    UNIT_ADDR - byte - from what unit on slave device
    STATUS - success or error code
    RESULT - 4 bytes - result of command execution 
    CRC16 - 2 bytes - crc16 sum of package


    -----------------------------------------------------------------------------------

    statuses:
    0x00 : 0x0F - success execution codes
    0xAA - absolutely win

    0x30 - incorrect CRC sum
    0x31 - incorrect format of package, but we dont know where is error
    0x32 - package is too long
    0x33 - package is too short
    0x34 - incorrect command
    0x35 - 

    0x40 : 0x4F - unknown error codes
    0x40 - something went wrong

    other error codes:
    0xBB - we are in big unknown trouble (use only as flag of real big problem, like fatal error)
    0xFF - reserved
    */


// ========================= constants declaration =============================
// constants

// constants

const uint16_t clot_system_version = 0x0001;

//success codes
const uint8_t clot_success = 0xAA;

//error codes

const uint8_t clot_crc_incorrect = 0x30;
const uint8_t clot_incorrect_package = 0x31;
const uint8_t clot_incorrect_command = 0x34;
const uint8_t clot_unknown_error = 0x40;

// some default variables to construct messages
const uint8_t clot_stub_value = 0x00;
const uint8_t clot_master_start_byte = 0xCA;
const uint8_t clot_slave_start_byte = 0xAC;
const uint8_t clot_master_message_len = 6;
const uint8_t clot_slave_message_len = 10;



// ========================= useful methods declaration =============================
// some useful methods and structs
struct two_bytes  //disgusting_s->write(sm.STATUS_1);
{
    uint8_t first;
    uint8_t last;
};

two_bytes clot_split2 (uint16_t value);
uint32_t clot_unite4 (uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
uint16_t clot_unite2 (uint8_t first, uint8_t last);
uint16_t clot_crc16 (uint8_t * pcBlock, uint16_t len);



// ============================== classes for messages ==================================


// | MASTER_START_BYTE | SLAVE_ADDR | UNIT_ADDR | COMMAND | CRC16_LOW | CRC16_HIGH |
class master_message
{
    // class that simply keep all needed fields for master message and constructor
    public:
    master_message(
        // we have to fill all values in every construction of message
        // because there is no any default behavior
        // except start_byte - its always 0xCA
        // crc16 fields we have to not fill
        // they will we calculated inside of that object by internal method
        // from this library (uint16_t d_crc16 (uint8_t* start_ptr, int len))
        uint8_t SLAVE_ADDR_,
        uint8_t UNIT_ADDR_,
        uint8_t COMMAND_
        );

    uint8_t START_BYTE;
    uint8_t SLAVE_ADDR;
    uint8_t UNIT_ADDR;
    uint8_t COMMAND;
    uint8_t CRC_16_1;
    uint8_t CRC_16_2;
};
//    package from slave to master - 10 bytes
//    | SLAVE_START_BYTE | SLAVE_ADDR | UNIT_ADDR | STATUS | RESULT1 | RESULT2 | RESULT3 | RESULT4 |
//     CRC16_LOW | CRC16_HIGH |
class slave_message
{
    // class that simply keeps all needed fields for slave message and constructor
    public:
    slave_message(
        // we have to fill all values in every construction of message
        // because there is no any default behavior
        // except start_byte - its always 0xAC for slave device
        // crc16 fields we have to not fill
        // they will be calculated inside of that object by some internal method

        uint8_t SLAVE_ADDR_,
        uint8_t UNIT_ADDR_,
        uint8_t STATUS_,
        uint8_t RESULT1_,
        uint8_t RESULT2_,
        uint8_t RESULT3_,
        uint8_t RESULT4_
        );
    
    uint8_t START_BYTE;
    uint8_t SLAVE_ADDR;
    uint8_t UNIT_ADDR;
    uint8_t STATUS;
    uint8_t RESULT1;
    uint8_t RESULT2;
    uint8_t RESULT3;
    uint8_t RESULT4;
    uint8_t CRC_16_1;
    uint8_t CRC_16_2;
    void update_crc();

};


#endif