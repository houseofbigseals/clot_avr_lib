
#ifndef  clot_base_unit_h
#define clot_base_unit_h

//#include <stdint.h>
#include "clot_protocol.h"

class base_unit
{
    public:
    uint8_t unit_id;
    base_unit(uint8_t unit_id_);
    base_unit() {};
    virtual slave_message handle_commands(uint8_t command);
    // in this method different hildren classes can parse master commands in different way
};

#endif