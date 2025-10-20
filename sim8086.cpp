#include <cstring>
#pragma warning(disable : 4996)
#include "opcodes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 1000000

void run_program(size_t program_size, uint8_t *memory, FILE *test_file);
void decode_opcode(uint8_t *memory, size_t *stack_point, FILE *test_file);

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
  uint8_t *contents = (uint8_t *)malloc(MEM_SIZE);
  fseek(file, 0, SEEK_SET);

  fread(contents, sizeof(char), sp_max, file);
  fclose(file);
  printf("Program size: %zu\n", sp_max);

  run_program(sp_max, contents, test_file);

  free(contents);
  fclose(test_file);
  return 0;
}

void run_program(const size_t program_size, uint8_t *memory, FILE *test_file) {
  size_t sp = 0;
  while (sp < program_size) {
    uint8_t byte = memory[sp];
    std::string output_string = instruction_decode(byte, memory, sp);
    printf("%s\n", output_string.c_str());
    sp++;
  }
}
