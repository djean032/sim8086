#include "opcodes.h"
#include <unordered_map>

std::unordered_map<uint8_t, Opcode> instruction_table = {
    {0x88, {0x88, "mov", 0, 0}}, {0x89, {0x89, "mov", 0, 1}},
    {0x8a, {0x8a, "mov", 1, 0}}, {0x8b, {0x8b, "mov", 1, 1}},
    {0xb0, {0xb0, "mov", 0, 0}}, {0xb1, {0xb1, "mov", 0, 0}},
    {0xb2, {0xb2, "mov", 0, 0}}, {0xb3, {0xb3, "mov", 0, 0}},
    {0xb4, {0xb4, "mov", 0, 0}}, {0xb5, {0xb5, "mov", 0, 0}},
    {0xb6, {0xb6, "mov", 0, 0}}, {0xb7, {0xb7, "mov", 0, 0}},
    {0xb8, {0xb8, "mov", 0, 1}}, {0xb9, {0xb9, "mov", 0, 1}},
    {0xba, {0xba, "mov", 0, 1}}, {0xbb, {0xbb, "mov", 0, 1}},
    {0xbc, {0xbc, "mov", 0, 1}}, {0xbd, {0xbd, "mov", 0, 1}},
    {0xbe, {0xbe, "mov", 0, 1}}, {0xbf, {0xbf, "mov", 0, 1}},
};

std::unordered_map<uint8_t, std::string> basic_register_table = {
    {al, "al"}, {cl, "cl"}, {dl, "dl"}, {bl, "bl"}, {ah, "ah"}, {ch, "ch"},
    {dh, "dh"}, {bh, "bh"}, {ax, "ax"}, {cx, "cx"}, {dx, "dx"}, {bx, "bx"},
    {sp, "sp"}, {bp, "bp"}, {si, "si"}, {di, "di"},

};

std::unordered_map<uint8_t, InstructionFunction> instruction_functions = {
    {0x88, &decode_mov},
    {0x89, &decode_mov},
    {0x8a, &decode_mov},
    {0x8b, &decode_mov},
};

std::unordered_map<uint8_t, std::string> effective_address = {
    {0, "[BX + SI]"}, {1, "[BX + DI]"}, {2, "[BP + SI]"}, {3, "[BP + DI]"},
    {4, "[SI]"},      {5, "[DI]"},      {6, "["},         {7, "[BX]"},
    {8, "[BX + SI"},  {9, "[BX + DI"},  {10, "[BP + SI"}, {11, "[BP + DI"},
    {12, "[SI"},      {13, "[DI"},      {14, ""},         {15, "[BX"},
    {13, "[BX + SI"}, {14, "[BX + DI"}, {15, "[BP + SI"}, {16, "[BP + DI"},
    {17, "[SI"},      {18, "[DI"},      {19, "[BP"},      {20, "[BX"},
};

std::string instruction_decode(uint8_t opcode, uint8_t *memory,
                               size_t &stack_point) {
  Opcode instruction = instruction_table.at(opcode);
  std::string output_string =
      instruction_functions[opcode](instruction, memory, stack_point);
  return output_string;
};

std::string decode_mov(Opcode opcode, uint8_t *memory, size_t &stack_point) {
  std::string output_string;
  output_string += opcode.mnemonic;
  output_string += " ";
  // get next byte by incrementing stack pointer
  stack_point++;
  uint8_t next_byte = get_next_byte(memory, stack_point);
  // get first two bits
  uint8_t mod = next_byte >> 6;
  if (mod == 0b11) {
    // get second three bits
    uint8_t reg = (next_byte >> 3) & 0b111;
    // get last three bits
    uint8_t rm = next_byte & 0b111;
    if (opcode.w_bit == 1) {
      rm += 8;
      reg += 8;
    }
    if (opcode.d_bit == 0) {
      output_string += basic_register_table[rm];
      output_string += ", ";
      output_string += basic_register_table[reg];
    } else if (opcode.d_bit == 1) {
      output_string += basic_register_table[reg];
      output_string += ", ";
      output_string += basic_register_table[rm];
    } else if (mod) {
    }
  } else if (mod == 0b10) {
  } else if (mod == 0b01) {
  } else if (mod == 0b00) {
  }
  return output_string;
}

uint16_t get_next_two_bytes(uint8_t *memory, size_t &stack_point) {
  stack_point++;
  uint16_t output = static_cast<uint16_t>(
      memory[stack_point] |
      (static_cast<uint16_t>(memory[stack_point + 1]) << 8));
  stack_point++;
  return output;
}

uint8_t get_next_byte(uint8_t *memory, size_t &stack_point) {
  stack_point++;
  uint8_t output = memory[stack_point];
  return output;
}

/*
typedef struct {
  OperandType type;
  std::string value;
  uint8_t additional_bytes;
} Operand;

typedef struct {
  uint8_t type;
  std::string mnemonic;
  uint8_t d_bit;
  uint8_t w_bit;
} Opcode;
*/
