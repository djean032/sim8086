#include "opcodes.h"
#include <array>
#include <string>
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

std::array<std::string_view, 16> basic_register_table = {
    {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "ax", "cx", "dx", "bx",
     "sp", "bp", "si", "di"}};

std::unordered_map<uint8_t, OpcodeFunction> instruction_functions = {
    {0x88, &decode_mov},
    {0x89, &decode_mov},
    {0x8a, &decode_mov},
    {0x8b, &decode_mov},
};

const std::array<std::array<std::string, 8>, 3> addressing_modes = {
    {// mod 00
     {{"[BX+SI]", "[BX+DI]", "[BP+SI]", "[BP+DI]", "[SI]", "[DI]", "", "[BX]"}},
     // mod 01 {{"[BX+SI+disp8]"},
     {"[BX+DI+disp8]", "[BP+SI+disp8]", "[BP+DI+disp8]", "[SI+disp8]",
      "[DI+disp8]", "[BP+disp8]", "[BX+disp8]"},
     // mod 10
     {"[BX+SI+disp16]", "[BX+DI+disp16]", "[BP+SI+disp16]", "[BP+DI+disp16]",
      "[SI+disp16]", "[DI+disp16]", "[BP+disp16]", "[BX+disp16]"}},
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
    }
  } else {
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
