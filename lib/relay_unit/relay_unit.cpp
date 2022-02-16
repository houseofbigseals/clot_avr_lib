
#include "relay_unit.h"
#include "Arduino.h"

clot_relay_unit::clot_relay_unit(uint8_t unit_id_, uint8_t pin_id_)
{

    this->unit_id = unit_id_;
    this->pin_id = pin_id_;
    pinMode(this->pin_id, OUTPUT);  
    digitalWrite(this->pin_id, HIGH);

}

slave_message  clot_relay_unit::handle_commands(uint8_t command)
{
    // command is one-byte uint8  0-255
    // if command is 0x00 - set pin low
    // if command is 0x01 - set pin high
    if(command == 0x00)
    {
        digitalWrite(this->pin_id, LOW);

        return slave_message(
            0x00,
            this->unit_id,
            clot_success,
            0x00,
            0x00,
            0x00,
            0x00);

    }
    else if(command == 0x01)
    {
        digitalWrite(this->pin_id, HIGH);

        return slave_message(
            0x00,
            this->unit_id,
            clot_success,
            0x00,
            0x00,
            0x00,
            0x00);
    }
    else
    {
        return slave_message(
            0x00,
            this->unit_id,
            clot_incorrect_command,
            0x00,
            0x00,
            0x00,
            0x00);
    }


}