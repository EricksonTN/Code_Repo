#include <stdint.h>
//return the LSB of the 2-byte value
uint8_t getLow(uint16_t reg);


//set the LSB of the 2-byte value pointed to by reg_p to val
void setLow(uint16_t* reg_p, uint8_t val);

//return the MSB of the 2-byte value
uint8_t getHigh(uint16_t reg);

//set the MSB of the 2-byte value pointed to by reg_p to val
void setHigh(uint16_t* reg_p, uint8_t val);

//set the i'th bit of the value pointed to by reg_p to 0
void clearBit(uint16_t* reg_p, uint8_t i);

//set the i'th bit of the value pointed to by reg_p to 1
void setBit(uint16_t* reg_p, uint8_t i);
