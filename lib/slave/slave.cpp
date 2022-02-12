

#include "slave.h"

void clot_slave::do_main_loop()
{
    // that function must be used as fast as we can
    // so no any sleeping
    //SoftwareSerial()
    // at first lets check if we have incoming messages through uart
    if(_s->available())
    {
        // if we have any lets get them
        uint8_t message[7]; // we dont want read more than 7 characters //TODO fix
        uint16_t len = 0;
        delay(1);  // strategical delay - awaiting until all bytes will be sent
        while(_s->available())
        {
            message[len] = _s->read();
            len++;
            delayMicroseconds(5);
        }
        // then lets parse it
        parse_package(message, len);
    }
    // if we got message or not we have to do routine for all devices
    /*
    for(uint16_t d = 0; d < _units_number; d++)
    {
        // we cant control the time of every device work
        // so we have to wait or use timer interruptions
        // or use rtos and tasks abstraction
        _device_pointer[d]->do_your_duty();
    }
    */

}

void clot_slave::parse_package(uint8_t * package, uint16_t len)
{
    // at first lets check if it for us
    if(package[1] != this->slave_address)
    {
        //this message is not for us
        // do nothing
        return;
    }
    else
    {

    if(len != clot_master_message_len)
    {
        // its incorrect but for us
        slave_message error_message = create_error_message(clot_incorrect_command);
        send_slave_message(error_message);
        return;
    }
    
    else
    {
        // then check start byte
        if(package[0] != clot_master_start_byte)
        {
            // its incorrect
            slave_message error_message = create_error_message(clot_incorrect_command);
            send_slave_message(error_message);
            return;
        }
        else
        {
            // check crc sum 
            // just put pointer of array and len -2 to clot_crc16 function
            uint16_t crc16_check = clot_crc16(package, len-2);
            // split it to half
            uint8_t crc1 = (uint8_t)((crc16_check & 0xFF00) >> 8);
            uint8_t crc2 = (uint8_t)(crc16_check & 0x00FF);
            if (crc1 != package[len-2] || crc2 != package[len-1])
            {
                // its incorrect
                slave_message error_message = create_error_message(clot_incorrect_command);
                send_slave_message(error_message);
                return;
            }
            else
            {
                // check if it is for one of our devices
                for (uint16_t d = 0; d < _units_number; d++)
                {
                    // go through all devices and compare to their _uid
                    uint8_t did = _units_pointer[d]->unit_id;
                    // unite uid from message to uint16 for simplicity
                    if (did == package[2])
                    {
                        uint8_t command = package[3];
                        slave_message sm = _units_pointer[d]->handle_commands(command);
                        // then we have to send it back to master pc
                        send_slave_message(sm);
                    }
                }
            }
        }   
    }
}


