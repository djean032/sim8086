#pragma warning (disable : 4996)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 1000000

typedef uint8_t u8;

void run_program(size_t program_size, u8 *memory);
void decode_opcode(u8 *memory, size_t stack_point);

int main(int argc, char **argv) {

    char* filename = argv[1];

    FILE *file = fopen(filename, "rb");
    if (file==NULL) {
        printf("Error: File could not be opened.\n");
        exit(1);
    }


    fseek(file, 0L, SEEK_END);
    const size_t sp_max = ftell(file);
    u8 *contents = (u8*)malloc(MEM_SIZE);
    fseek(file, 0, SEEK_SET);

    fread(contents, sizeof(char), sp_max, file);

    run_program(sp_max, contents);

    return 0;
}

void run_program(const size_t program_size, u8 *memory) {
    size_t sp = 0;
    int op_decode = 1;
    while (sp < program_size) {
        if (op_decode) {
            decode_opcode(memory, sp);
            op_decode = 0;
        } else {
        }
        sp++;
    }
}

void decode_opcode(u8 *memory, size_t stack_point) {
    int op_val = (memory[stack_point] & 0xFF) >> 2;
    switch (op_val) {
        case 0b100010:
            printf("mov ");
            break;
        default:
            printf("Error: Invalid Opcode");
            exit(1);
    }
}

