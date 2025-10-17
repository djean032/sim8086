#include <cstring>
#pragma warning(disable : 4996)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 1000000

typedef uint8_t u8;

typedef enum op_type {
  OP_MOV,
  OP_PUSH,
  OP_POP,
} op_type;

typedef enum usage {
  BITS_END,
  OPCODE,
  MOD,
  REG,
  RM,
  SR,
  W,
  S,
  D,
  V,
  Z,
  DATA8,
  DATALO,
  DATAHI,
  DISPLO,
  DISPHI,
  IPLO,
  IPHI,
  CSLO,
  CSHI,
  IPINC8,
  IPINCLO,
  IPINCHI,
  ADDRLO,
  ADDRHI,
  XXX,
  YYY,
  REG8,
  REG16,
  MEM8,
  MEM16,
  IMMED8,
  IMMED16,
  SEGREG,
  DESTSTR8,
  SRCSTR8,
  DESTSTR16,
  SRCSTR16,
  SHORTLBL,
  NEARPROC,
  FARPROC,
  NEARLBL,
  FARLBL,
  SRCLBL,
  ESCOP,
  SRC,
} usage;

typedef struct bits {
  usage bit_use;
  u8 bit_count;
  u8 shift;
  u8 value;
} bits;

typedef struct opcode {
  op_type name;
  bits bits[16];
} opcode;

char registers[][2][3] = {
    {"al", "ax"}, {"cl", "cx"}, {"dl", "dx"}, {"bl", "bx"},
    {"ah", "sp"}, {"ch", "bp"}, {"dh", "si"}, {"bh", "di"},
};

opcode op_table[7] = {
    {OP_MOV, {{OPCODE, 6, 0, 0b100010}}},
    {OP_MOV, {{OPCODE, 7, 0, 0b1100011}}},
    {OP_MOV, {{OPCODE, 4, 0, 0b1011}}},
    {OP_MOV, {{OPCODE, 7, 0, 0b1010000}}},
    {OP_MOV, {{OPCODE, 7, 0, 0b1010001}}},
    {OP_MOV, {{OPCODE, 8, 0, 0b10001110}}},
    {OP_MOV, {{OPCODE, 8, 0, 0b10001100}}},
};

void run_program(size_t program_size, u8 *memory, FILE *test_file);
void decode_opcode(u8 *memory, size_t *stack_point, FILE *test_file);
size_t lookup_op(u8 value);

int main(int argc, char **argv) {

  char *filename = argv[1];

  FILE *file = fopen(filename, "rb");
  FILE *test_file = fopen("test_file.asm", "w");
  fprintf(test_file, "bits 16\n");
  if (file == NULL) {
    printf("Error: File could not be opened.\n");
    exit(1);
  }

  fseek(file, 0L, SEEK_END);
  const size_t sp_max = ftell(file);
  u8 *contents = (u8 *)malloc(MEM_SIZE);
  fseek(file, 0, SEEK_SET);

  fread(contents, sizeof(char), sp_max, file);
  fclose(file);

  run_program(sp_max, contents, test_file);

  free(contents);
  fclose(test_file);
  return 0;
}

void run_program(const size_t program_size, u8 *memory, FILE *test_file) {
  size_t sp = 0;
  while (sp < program_size) {
    decode_opcode(memory, &sp, test_file);
  }
}

size_t lookup_op(u8 value) {
  size_t found_idx;
  for (int i = 0; i < (sizeof(op_table) / sizeof(op_table[0])); i++) {
    if (value == op_table[i].bits[0].value) {
      found_idx = i;
      return found_idx;
    }
  }
  return 18;
}

void decode_opcode(u8 *memory, size_t *stack_point, FILE *test_file) {
  for (int i = 0; i < 8; i++) {
    u8 tmp_op;
    u8 mask = 0xFF;
    u8 tmp_val = memory[*stack_point] & mask;
    tmp_op = tmp_val >> i;
    printf("Current Value: %d\n", tmp_op);
    size_t found = lookup_op(tmp_op);
    if (found != 18) {
      printf("%d\n", op_table[found].bits[0].bit_use);
    } else {
      printf("Not Found!\n");
    }
  }
  (*stack_point)++;
}
