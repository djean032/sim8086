#pragma once

#include <array>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

enum class OperandType : uint8_t {
  MEM_NONE,
  MEM_8,
  MEM_16,
  REG,
  IMMED_8,
  IMMED_16
};

enum class Register : uint8_t {
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
};

typedef struct {
  OperandType type;
  std::string_view value;
} Operand;

typedef struct {
  uint8_t type;
  std::string_view mnemonic;
  uint8_t d_bit;
  uint8_t w_bit;
  uint8_t s_bit;
  uint8_t v_bit;
  uint8_t z_bit;
} Opcode;

typedef std::string (*OpcodeFunction)(Opcode, uint8_t *, size_t &);

std::string instruction_decode(uint8_t opcode, uint8_t *memory,
                               size_t &stack_point);

std::string decode_mov(Opcode opcode, uint8_t *memory, size_t &stack_point);
uint16_t get_next_two_bytes(uint8_t *memory, size_t &stack_point);
uint8_t get_next_byte(uint8_t *memory, size_t &stack_point);
