#ifndef clot_relay_unit_h
#define clot_relay_unit_h


//#include <stdint.h>
#include "base_unit.h"
#include "slave.h"
#include "clot_protocol.h"

class clot_relay_unit : public base_unit
{
    public:
    uint8_t pin_id;
    uint8_t unit_id;
    clot_relay_unit(uint8_t unit_id_, uint8_t pin_id);
    slave_message handle_commands(uint8_t command);

};

#endif