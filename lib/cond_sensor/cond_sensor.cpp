
#include "cond_sensor.h"


#define F_CPU 8000000UL
#define MAX_ITER 1
#define DELAY_TIME 150
#define TW_START 0x08
#define TW_REP_START 0x10
#define TW_ARB_LOST 0x38
#define TW_SLA_R_ACK 0x40
#define TW_SLA_R_NACK 0x48
#define TW_MR_DATA_ACK 0x50
#define TW_MR_DATA_NACK 0x58
#define TW_SLA_W_ACK 0x18
#define TW_SLA_W_NACK 0x20
#define TW_MT_DATA_ACK 0x28
#define TW_MT_DATA_NACK 0x30
#define DEV_ADDR 0x4C // Address of Slave-Device


void setup()
{
    Serial.begin(9600);
    _delay_ms(5000);  
    TWIInit();
    _delay_ms(1000);
    //TODO
}

void loop()
{
    //TODO
    //wait for user command, then measure and send data back 
    /*
    read_val = i2c_read_bytes(DEV_ADDR);
    adc_val = (float)read_val*5/1023;
    
    if( (read_val&0x8000) == 0)
        Serial.print(adc_val);
    else
        Serial.print(0.0);  
    Serial.println ();  
  */

}

void sendState() 
{

    /*
  String outs="R";
  if (state[0])
    outs=outs+"1";
  else
    outs=outs+"0";
  if (state[1])
    outs=outs+"1";
  else
    outs=outs+"0";
  outs=outs+";";
  
  Serial.print (outs);
  read_val = i2c_read_bytes(DEV_ADDR);
  adc_val = (float)read_val*5/1023;
  
  if( (read_val&0x8000) == 0)
    Serial.print(adc_val);
  else
    Serial.print(0.0);  
  Serial.println ();  
  */
}

void i2c_write_byte(uint8_t addr, uint8_t val2wr){
  begin:
    TWIStart();
    switch (TWSR&0xF8){
      case TW_REP_START: // OK, but should not happen
      case TW_START:
        break;
      case TW_ARB_LOST:
        goto begin;
      default:
        return; // error: not in start condition NB: do /not/ send stop condition
    }
/* send SLA+W */
    TWIWrite(addr);
    switch (TWSR&0xF8){
      case TW_SLA_W_ACK:
        TWDR = val2wr;
        break;
      case TW_SLA_W_NACK: // nack during select: device busy writing
        goto quit;
      case TW_ARB_LOST: // re-arbitrate
        goto begin;
      default:
        goto quit; // must send stop condition
    }
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // send ACK
    while (!(TWCR & (1<<TWINT))); // wait for transmission
    switch (TWSR&0xF8) {
      case TW_MT_DATA_NACK:
      case TW_MT_DATA_ACK:
      default:
      goto quit;
    }
    quit:
      TWIStop();
  return;
}

uint16_t i2c_read_bytes(uint8_t addr)
{
    uint16_t result;
    begin:
    TWIStart();
    switch (TWSR&0xF8)
    {
        case TW_REP_START: // OK, but should not happen
        //      Serial.println ("TW_REP_START");
        case TW_START:
        //      Serial.println ("TW_START");
        break;
        case TW_ARB_LOST:
        //      Serial.println ("TW_ARB_LOST");
        goto begin;
        default:
        //      Serial.println ("not in start condition NB");
        return -1; // error: not in start condition NB: do /not/ send stop condition
    }
    /* send SLA+R */
    TWIWrite(addr|0x01);
    switch (TWSR&0xF8)
    {
        case TW_SLA_R_ACK:
        //      Serial.println ("TW_SLA_R_ACK");
        break;
        case TW_SLA_R_NACK: // nack during select: device busy writing
        //      Serial.println ("TW_SLA_R_NACK");
        goto quit;
        case TW_ARB_LOST: // re-arbitrate
        //      Serial.println ("TW_ARB_LOST");
        goto begin;
        default:
        //      Serial.println ("must send stop condition");
        goto error; // must send stop condition
    }

    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // send ACK
    while (!(TWCR & (1<<TWINT))); // wait for transmission
    switch (TWSR&0xF8) 
    {
        case TW_MR_DATA_NACK:
            //      Serial.println ("TW_MR_DATA_NACK");
        case TW_MR_DATA_ACK:
            //      Serial.println ("TW_MR_DATA_ACK");
            result = TWDR;
            break;
        default:
            //      Serial.println ("error 1");
            goto error;
    }
    TWCR = (1<<TWINT)|(1<<TWEN); // send NACK
    while (!(TWCR & (1<<TWINT))); // wait for transmission
    switch (TWSR&0xF8) 
    {
        case TW_MR_DATA_NACK:
        //      Serial.println ("TW_MR_DATA_NACK");
        result = (result<<8)|TWDR;
        break;
        case TW_MR_DATA_ACK:
        //      Serial.println ("TW_MR_DATA_ACK");
        default:
            //      Serial.println ("error 2");
            goto error;
    }
    quit:
        TWIStop();
        return result;
    error:
        result = -1;
        goto quit;
}

void TWIInit() { // OscFreq / (16+2*TWBR*4^TWSR); TWSR = 0..3
    //  Serial.println ("TWIInit begin");  
    TWBR = 0xB0;
    TWSR = (TWSR & 0xFC)|0x02;
    TWCR = (1<<TWEN); //enable TWI
    //  Serial.println ("TWIInit end");    
}

void TWIStart() {
    //  Serial.println ("TWIStart begin");    
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    //  Serial.println ("TWIStart end");      
}

void TWIStop() 
{
    //  Serial.println ("TWIStop begin");      
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    //  Serial.println ("TWIStop end");        
}

void TWIWrite(uint8_t u8data) 
{
    //  Serial.println ("TWIWrite begin");        
    TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    //  Serial.println ("TWIWrite end");          
}

uint8_t TWIReadACK() 
{
    //  Serial.println ("TWIReadACK begin");          
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    //  Serial.println ("TWIReadACK end");            
    return TWDR;
}

uint8_t TWIReadNACK() 
{
    //  Serial.println ("TWIReadNACK begin");            
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    //  Serial.println ("TWIReadNACK end");              
    return TWDR;
}