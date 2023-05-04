
#include <stdint.h>
#include <stdio.h>
#include "assignment1.h"



typedef struct Registers{
  uint16_t af, bc, de, hl, sp;
} Registers;


uint16_t stepHomeworkInstruction(void* mem, struct Registers* regs, uint16_t* pc,
    uint8_t(*readMemory)(void*, uint16_t), void(*writeMemory)(void*, uint16_t, uint8_t)){

  uint8_t opcode = readMemory(mem, *pc);

  // return value 16 bit unsigned int
  uint16_t ret = 0;

  switch (opcode) {
    
  case 0xc3: 
    {
      uint16_t immd;
      
      uint8_t immd_LSB = readMemory(mem, ++(*pc));
      uint16_t immd_MSB = readMemory(mem, ++(*pc));

      // Places MSB in the correct place in the 16 bit uint
      immd_MSB = immd_MSB << 8;

      // Combines the MSB and LSB together to create the full 16 bit immd.
      immd = immd_MSB | immd_LSB;

      // Sets the pc to the immd
      *pc = immd;

      // sets the return value
      ret = 4;

      break;
      
    }

  case 0x3e: 
    {
      // grabs immd from memory
      uint8_t immd = readMemory(mem, ++(*pc));

      // sets the a register to the immd
      setHigh(&(regs -> af), immd);

      ++(*pc);

      ret = 2;

      break;
    }

  case 0xea:
    {
      
      uint16_t immd;
      
      uint8_t immd_LSB = readMemory(mem, ++(*pc));
      uint16_t immd_MSB = readMemory(mem, ++(*pc));

      immd_MSB = immd_MSB << 8;

      immd = immd_MSB | immd_LSB;

      uint8_t a = getHigh(regs -> af);

      // Set the value of a into the memory address specified by the immd.
      writeMemory(mem, immd, a);

      ++(*pc);

      ret = 4;
      
      break;
    }

  case 0x3c: 
    {
      uint8_t a = getHigh(regs -> af);

      // copy of the a register before the increment.
      uint8_t a_copy = getHigh(regs -> af);
      
      a += 1;

      // sets a register to new value of a.
      setHigh(&(regs -> af), a);

      // grabs the last 4 bits of a_copy
      uint8_t copy_last4Bits = a_copy & 0x0f;

      // Sets the HF flag if bit 3 carries over to bit 4
      if (copy_last4Bits == 0x0f)
      {
	setBit(&(regs -> af), 5);
      }

      else
      {
	clearBit(&(regs -> af), 5);
      }

      

      // Sets the Z flag if a == 0
      if (a == 0)
      {
	setBit(&(regs -> af), 7);
      }

      else
      {
	clearBit(&(regs -> af), 7);
      }

      // sets the SF flag to zero
      clearBit(&(regs -> af), 6);

      ++(*pc);

      ret = 1;

      break;
    }

  case 0x2a: 
    {
      // grabs the data stored at memory and increment hl
      int8_t hl_data = readMemory(mem, (regs -> hl)++);

      // sets a to hl_data
      setHigh(&(regs -> af), hl_data);

      ++(*pc);

      ret = 2;

      break;
    }

  case 0x3a: 
    {
      // grabs the data stored at memory and decrement hl
      int8_t hl_data = readMemory(mem, (regs -> hl)--);

      // ses a to hl_data
      setHigh(&(regs -> af), hl_data);

      ++(*pc);

      ret = 2;

      break;
    }

  case 0x22: 
    {
      // grabs the hl register
      int16_t hl = (regs -> hl);

      int8_t a = getHigh(regs -> af);

      // store a in memory specified by hl
      writeMemory(mem, hl, a);

      // increment hl
      (regs -> hl)++;
      
      ++(*pc);

      ret = 2;

      break;
    }

  case 0x32: 
    {
      // grabs the hl register
      int16_t hl = (regs -> hl);

      int8_t a = getHigh(regs -> af);

      // store a in memory specified by hl
      writeMemory(mem, hl, a);

      // decrement hl
      (regs -> hl)--;
      
      ++(*pc);

      ret = 2;

      break;
    }

  case 0xc6: 
    {
      // grabs the immd value
      uint8_t immd = readMemory(mem, ++(*pc));

      
      uint8_t a = getHigh(regs -> af);
      uint8_t a_copy = getHigh(regs -> af);

      a += immd;

      setHigh(&(regs -> af), a);

      uint8_t a_copy_last4Bits = a_copy & 0x0f;
      uint8_t immd_last4Bits = immd & 0x0f;

      //Set/Clear the HF Flag
      if ((a_copy_last4Bits + immd_last4Bits) > 0x0f)
      {
	setBit(&(regs -> af), 5);
      }

      else
      {
	clearBit(&(regs -> af), 5);
      }

      //Set/Clear the CF Flag
      if (a <= a_copy)
      {
	setBit(&(regs -> af), 4);
      }

      else
      {
	clearBit(&(regs -> af), 4);
      }
      
      //Set/Clear the Z flag
      if (a == 0)
      {
	setBit(&(regs -> af), 7);
      }

      else
      {
	clearBit(&(regs -> af), 7);
      }

      //Clear the SF flag
      clearBit(&(regs -> af), 6);

      ++(*pc);

      ret = 2;

      break;
    }

  case 0xd6: 
    {
      uint8_t immd = readMemory(mem, ++(*pc));

      uint8_t a = getHigh(regs -> af);
      uint8_t a_copy = getHigh(regs -> af);

      a -= immd;

      setHigh(&(regs -> af), a);

      uint8_t a_copy_last4Bits = a_copy & 0x0f;
      uint8_t immd_last4Bits = immd & 0x0f;

      //Set/Clear the HF Flag
      if ((a_copy_last4Bits + immd_last4Bits) > 0x0f)
      {
	setBit(&(regs -> af), 5);
      }

      else
      {
	clearBit(&(regs -> af), 5);
      }

      //Set/Clear the CF Flag
      if (a > a_copy)
      {
	setBit(&(regs -> af), 4);
      }

      else
      {
	clearBit(&(regs -> af), 4);
      }
      
      //Set/Clear the Z flag
      if (a == 0)
      {
	setBit(&(regs -> af), 7);
      }

      else
      {
	clearBit(&(regs -> af), 7);
      }

      //Set the SF flag
      setBit(&(regs -> af), 6);

      ++(*pc);

      ret = 2;

      break;
    }

  case 0x1f: 
    {
      uint8_t a = getHigh(regs -> af);
      uint8_t f = getLow(regs -> af);

      uint8_t oldCF = f >> 4;
      oldCF = oldCF & 0x1;
      oldCF = oldCF << 7;
      
      uint8_t newCF = a & 0x01;

      a = a >> 1;
      a = a | oldCF;

      setHigh(&(regs -> af), a);

      //Clear ZF Flag
      clearBit(&(regs -> af), 7);

      //Clear SF Flag
      clearBit(&(regs -> af), 6);

      //Clear HF Flag
      clearBit(&(regs -> af), 5);
      
      //Set/Clear CF Flag
      if (newCF == 1)
      {
	setBit(&(regs -> af), 4);
      }

      else
      {
	clearBit(&(regs -> af), 4);
      }

      ++(*pc);

      ret = 1;

      break;   
    }

  case 0xfe: 
    {
      uint8_t immd = readMemory(mem, ++(*pc));

      uint8_t a = getHigh(regs -> af);
      uint8_t a_copy = getHigh(regs -> af);

      a -= immd;

      uint8_t a_copy_last4Bits = a_copy & 0x0f;
      uint8_t immd_last4Bits = immd & 0x0f;

      //Set/Clear the HF Flag
      if ((a_copy_last4Bits + immd_last4Bits) > 0x0f)
      {
	setBit(&(regs -> af), 5);
      }

      else
      {
	clearBit(&(regs -> af), 5);
      }

      //Set/Clear the CF Flag
      if (a > a_copy)
      {
	setBit(&(regs -> af), 4);
      }

      else
      {
	clearBit(&(regs -> af), 4);
      }
      
      //Set/Clear the Z flag
      if (a == 0)
      {
	setBit(&(regs -> af), 7);
      }

      else
      {
	clearBit(&(regs -> af), 7);
      }

      //Set the SF flag
      setBit(&(regs -> af), 6);

      ++(*pc);

      ret = 2;

      break;
    }

  case 0xca: 
    {
      uint16_t immd;

      uint8_t immd_LSB = readMemory(mem, ++(*pc));
      uint16_t immd_MSB = readMemory(mem, ++(*pc));

      immd_MSB = immd_MSB << 8;

      immd = immd_MSB | immd_LSB;
      
      uint8_t f = getLow(regs -> af);
      uint8_t zeroFlag = f >> 7;
      

      if (zeroFlag == 0x1)
      {
	*pc = immd;

	ret = 4;

	break;
      }

      else
      {
	++(*pc);

	ret = 3;

	break;
      }

      break;
    }

  case 0xda: 
    {
      uint16_t immd;

      uint8_t immd_LSB = readMemory(mem, ++(*pc));
      uint16_t immd_MSB = readMemory(mem, ++(*pc));

      immd_MSB = immd_MSB << 8;

      immd = immd_MSB | immd_LSB;
      
      uint8_t f = getLow(regs -> af);
      uint8_t carryFlag = f >> 4;
      carryFlag = carryFlag & 0x1;

      if (carryFlag == 0x1)
      {
	*pc = immd;

	ret = 4;

	break;
      }

      else
      {
	++(*pc);

	ret = 3;

	break;
      }

      break;
    }

  case 0x18: 
    {
      // Pointing at the opcode
      ++(*pc);
      // pc points at immediate value
      
      int8_t immd = readMemory(mem, (*pc));

      // pc is past the immediate
      *pc += immd + 1;
      
      ret = 3;

      break;
    }

  case 0xc5: 
    {
      uint8_t bc_LSB = getLow(regs -> bc);
      uint8_t bc_MSB = getHigh(regs -> bc);

      //Grows the sp by 2 bytes
      (regs -> sp) -= 2;

      writeMemory(mem, (regs -> sp), bc_LSB);
      writeMemory(mem, ((regs -> sp) + 1), bc_MSB);

      ++(*pc);

      ret = 4;

      break;
    }

  case 0xd1: 
    {
      // Grabs the LSB of the stack value and shrinks the sp by 1 byte
      uint8_t stack_val_LSB = readMemory(mem, (regs -> sp)++);

      // Grabs the MSB of the stack value and shrinks the sp by 1 byte.
      uint8_t stack_val_MSB = readMemory(mem, (regs -> sp)++);

      
      setHigh(&(regs -> de), stack_val_MSB);
      setLow(&(regs -> de), stack_val_LSB);

      ++(*pc);

      ret = 3;

      break;
      
    }

  case 0xcd: 
    {
      (regs -> sp) -= 2;
      
      writeMemory(mem, (regs -> sp), ++(*pc));
      uint16_t immd = readMemory(mem, (*pc));

      *pc = immd;

      ret = 6;

      break;
    }

  case 0xc9: 
    {
      uint16_t retaddr;
      uint8_t retaddr_LSB = readMemory(mem, (regs -> sp)++);
      uint16_t retaddr_MSB = readMemory(mem, (regs -> sp)++);

      retaddr_MSB = retaddr_MSB << 8;

      retaddr = retaddr_MSB | retaddr_LSB;

      *pc = retaddr;

      ret = 4;

      break;
    }

  default:
    {
      printf("pc has gone too far");
    }
  
  }
  
  return ret;
}

//return the LSB of the 2-byte value
uint8_t getLow(uint16_t reg){

  uint8_t LSB = reg;

  return LSB;
}


//set the LSB of the 2-byte value pointed to by reg_p to val
void setLow(uint16_t* reg_p, uint8_t val){

  *reg_p = *reg_p & 0xff00;

  *reg_p = *reg_p | val;
  
}

//return the MSB of the 2-byte value
uint8_t getHigh(uint16_t reg){

  uint8_t MSB = reg >> 8;

  return MSB;
}

//set the MSB of the 2-byte value pointed to by reg_p to val
void setHigh(uint16_t* reg_p, uint8_t val){

  *reg_p = *reg_p & 0x00ff;

  uint16_t val16 = val << 8;

  *reg_p = *reg_p | val16;
}

//set the i'th bit of the value pointed to by reg_p to 0
void clearBit(uint16_t* reg_p, uint8_t i){

  uint16_t i_pos = 1 << i;

  *reg_p = *reg_p & (~i_pos);
  
}

//set the i'th bit of the value pointed to by reg_p to 1
void setBit(uint16_t* reg_p, uint8_t i){

  uint16_t i_pos = 1 << i;

  *reg_p = *reg_p | i_pos;
  
}


