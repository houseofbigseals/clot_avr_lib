

#include "slave.h"


clot_slave::clot_slave(uint8_t slave_addr, base_unit **units_pointer_, uint16_t units_number_, 
HardwareSerial * s_, bool debug = false)
{
    this->slave_address = slave_addr;
    this->_units_pointer = units_pointer_;
    this->_units_number = units_number_;
    this->_s= s_;

}


void clot_slave::do_main_loop()
{
    // that function must be used as fast as we can
    // so no any sleeping
    //SoftwareSerial()
    // at first lets check if we have incoming messages through uart
    if(_s->available())
    {
        // if we have any lets get them
        uint8_t message[11]; // we dont want read more than 7 characters //TODO fix
        uint16_t len = 0;
        delay(1);  // strategical delay - awaiting until all bytes will be sent
        while(_s->available())
        {
            message[len] = _s->read();
            len++;
            delayMicroseconds(5);
        }
        // then lets parse it
        //_s->println("hahahaha");
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


void clot_slave::send_slave_message(slave_message sm_)
{
    //    package from slave to master - 10 bytes
    //    | SLAVE_START_BYTE | SLAVE_ADDR | UNIT_ADDR | STATUS | RESULT1 | 
    //      RESULT2 | RESULT3 | RESULT4 | CRC16_1 | CRC16_2 |
    // disgusting
    // go to send mode
    //digitalWrite(software_serial_pin, RS485Transmit);
    // then send bytes
    _s->write(sm_.START_BYTE);
    delayMicroseconds(5);
    _s->write(sm_.SLAVE_ADDR);
    delayMicroseconds(5);
    _s->write(sm_.UNIT_ADDR);
    delayMicroseconds(5);
    _s->write(sm_.STATUS);
    delayMicroseconds(5);
    _s->write(sm_.RESULT1);
    delayMicroseconds(5);
    _s->write(sm_.RESULT2);
    delayMicroseconds(5);
    _s->write(sm_.RESULT3);
    delayMicroseconds(5);
    _s->write(sm_.RESULT4);
    delayMicroseconds(5);
    _s->write(sm_.CRC_16_1);
    delayMicroseconds(5);
    _s->write(sm_.CRC_16_2);
    delayMicroseconds(5);

    // go back to read mode
    //digitalWrite(software_serial_pin, RS485Receive);
    // clean
    _s->flush();

}


void clot_slave::parse_package(uint8_t * package, uint8_t len)
{
    _s->println("we got: \n");
    for(uint8_t i =0; i<len; i++)
    {
        _s->print(package[i]);
    }
    _s->println("\nour reaction: ");
    // at first lets check if it for us
    if(package[1] != this->slave_address)
    {
        //this message is not for us
        // do nothing
        _s->println("not my msg");

        return;
    }
    else
    {

        if(len != clot_master_message_len)
        {
            // its incorrect but for us
            slave_message error_message = create_error_message(clot_incorrect_command,
            package[2]);
            _s->println("wrong len");
            send_slave_message(error_message);
            return;
        }
    
        else
        {
            // then check start byte
            if(package[0] != clot_master_start_byte)
            {
                // its incorrect
                slave_message error_message = create_error_message(clot_incorrect_command,
                package[2]);
                _s->println("wrong start byte");
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
                    slave_message error_message = create_error_message(
                        clot_incorrect_command, package[2]);
                    _s->println("wrong crc");
                    send_slave_message(error_message);
                    return;
                }
                else
                {
                    _s->println("good msg");
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
                            // now we have to add slave addr to this msg
                            sm.SLAVE_ADDR = this->slave_address;
                            // and then recalculate crc sum
                            sm.update_crc();
                            // then we have to send it back to master pc
                            send_slave_message(sm);
                        }
                    }
                }
            }   
        }
    }
}


slave_message clot_slave::create_error_message(uint8_t error_code, uint8_t unit_id_)
{
            return slave_message(
                this->slave_address,
                unit_id_,
                error_code,
                0x00,
                0x00,
                0x00,
                0x00);
}






