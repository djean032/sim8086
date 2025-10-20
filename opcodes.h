#pragma once

#include <array>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

typedef enum {
  MEM_NONE,
  MEM_8,
  MEM_16,
  REG,
} OperandType;

typedef enum {
  al,
  cl,
  dl,
  bl,
  ah,
  ch,
  dh,
  bh,
  ax,
  cx,
  dx,
  bx,
  sp,
  bp,
  si,
  di,
} RegisterType;

typedef struct {
  OperandType type;
  std::string value;
} Operand;

typedef struct {
  uint8_t type;
  std::string mnemonic;
  uint8_t d_bit;
  uint8_t w_bit;
} Opcode;

typedef std::string (*InstructionFunction)(Opcode, uint8_t *, size_t &);

std::string instruction_decode(uint8_t opcode, uint8_t *memory,
                               size_t &stack_point);

std::string decode_mov(Opcode opcode, uint8_t *memory, size_t &stack_point);
uint16_t get_next_two_bytes(uint8_t *memory, size_t &stack_point);
uint8_t get_next_byte(uint8_t *memory, size_t &stack_point);
