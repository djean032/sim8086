#include <cstring>
#pragma warning (disable : 4996)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 1000000

typedef uint8_t u8;

char registers[][2][3] = {
    {"al", "ax"},
    {"cl", "cx"},
    {"dl", "dx"},
    {"bl", "bx"},
    {"ah", "sp"},
    {"ch", "bp"},
    {"dh", "si"},
    {"bh", "di"},
};

char opcodes[][5] = {
    "mov",
};


void run_program(size_t program_size, u8 *memory, FILE *test_file);
void decode_opcode(u8 *memory, size_t *stack_point, FILE *test_file);

int main(int argc, char **argv) {

    char* filename = argv[1];

    FILE *file = fopen(filename, "rb");
    FILE *test_file = fopen("test_file.asm", "w");
    fprintf(test_file, "bits 16\n");
    if (file==NULL) {
        printf("Error: File could not be opened.\n");
        exit(1);
    }


    fseek(file, 0L, SEEK_END);
    const size_t sp_max = ftell(file);
    u8 *contents = (u8*)malloc(MEM_SIZE);
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

void decode_opcode(u8 *memory, size_t *stack_point, FILE *test_file) {
    int op_idx;
    int op_val = (memory[*stack_point] & 0xFF) >> 2;
    int w = memory[*stack_point] & 1;
    int d = (memory[*stack_point] >> 1) & 1;
    switch (op_val) {
        case 0x22:
            op_idx = 0;
            break;
        default:
            printf("Error: Invalid Opcode");
            exit(1);
    }
    (*stack_point)++;

    // Second byte
    int mod = (memory[*stack_point] >> 6) & 0x3;
    int reg = (memory[*stack_point] >> 3) & 0x7;
    int rm = memory[*stack_point] & 0x7;

    fprintf(test_file, "%s %s, %s\n", opcodes[0], registers[d ? reg : rm][w], registers[d ? rm : reg][w]);
    (*stack_point)++;
}
