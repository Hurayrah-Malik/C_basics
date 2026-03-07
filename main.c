#include <stdio.h>
#include <stdlib.h>

enum Op { LOAD, ADD, PRINT, HALT };

struct CPU {
    int registers[8];
    int memory[256];
    int pc;          // program counter — tracks which instruction we're on
    int running;
    int instruction; // the current instruction that you are executing
};

// Instruction format (32-bit packed integer):
//   bits 31-24: opcode
//   bits 23-16: operand 1 (destination register)
//   bits 15-8:  operand 2 (value or source register 1)
//   bits 7-0:   operand 3 (source register 2, ADD only)

//print all registers and some memory
void print_cpu_state(struct CPU *cpu) {

    // print registers
    printf("=== REGISTERS ===\n");
    for (int i = 0; i < 8; i++) {
        printf("R%d: %d\n", i, cpu->registers[i]);
    }

    // print first 16 slots of memory
    printf("\n=== MEMORY (first 16) ===\n");
    for (int i = 0; i < 16; i++) {
        printf("[%d]: %d\n", i, cpu->memory[i]);
    }

    printf("\nPC: %d\n", cpu->pc);
}

//load a value into the given register
void load(struct CPU *cpu) {
    //decode the register number from the instruction.
    int register_num = (cpu->instruction >> 16) & 0xFF;

    //number to load into register.
    int value = (cpu->instruction >> 8) & 0xFF;

    //store the value in the register
    cpu->registers[register_num] = value;

    //update counter
    cpu->pc += 1;
}

//add 2 register values and store that value in a specified register
void add(struct CPU *cpu) {
    //decode the register number from the instruction
    int reg_destination = (cpu->instruction >> 16) & 0xFF;

    //registers to add
    int register_num1 = (cpu->instruction >> 8) & 0xFF;
    int register_num2 = (cpu->instruction)       & 0xFF;

    //add the registers and store it in register destination
    cpu->registers[reg_destination] = cpu->registers[register_num1] + cpu->registers[register_num2];
    cpu->pc += 1;
}

//print a register value
void print_reg_value(struct CPU *cpu) {
    //register to print
    int register_num = (cpu->instruction >> 16) & 0xFF;
    //value of the register
    int value = cpu->registers[register_num];

    cpu->pc += 1;

    printf("the value of register number %d is %d\n", register_num, value);
}

//load a value into the given register
void halt(struct CPU *cpu) {
    //set running to 0 so infinite loop stops running
    cpu->running = 0;
    cpu->pc += 1;
}

//write the harcoded program into a file called program.bin
void write_program(int *program, int len) {
    FILE *f = fopen("program.bin", "wb");
    fwrite(program, sizeof(int), len, f);
    fclose(f);
}

//read the program.bin file
void read_program(struct CPU *cpu, int len) {
    FILE *f = fopen("program.bin", "rb");
    fread(cpu->memory, sizeof(int), len, f);
    fclose(f);
}

int main() {
    //hard coded program
    int program[] = {
        (LOAD << 24) | (0 << 16) | (5  << 8),       // LOAD R0, 5
        (LOAD << 24) | (1 << 16) | (10 << 8),       // LOAD R1, 10
        (ADD  << 24) | (2 << 16) | (1  << 8) | (0), // ADD R2, R1, R0
        (PRINT << 24) | (2 << 16),                   // PRINT R2
        (HALT << 24),                                // HALT
    };

    int len_program = sizeof(program) / sizeof(program[0]);

    //write the program to a program.bin file
    write_program(program, len_program);

    //create a cpu struct pointer. calloc initializes everything to 0
    struct CPU *cpu = calloc(1, sizeof(struct CPU));

    //copy the program from program.bin to cpu->memory
    read_program(cpu, len_program);

    //create an array of pointers to functions
    void (*cpu_functions[4])(struct CPU *) = {load, add, print_reg_value, halt};

    //while loop that goes over whole program in memory
    cpu->running = 1;
    while (cpu->running) {
        //store the instruction in the cpu struct
        cpu->instruction = cpu->memory[cpu->pc];

        //extract the opcode from that instruction
        int opcode = (cpu->instruction >> 24) & 0xFF;

        //call the correct function given the opcode (look up the opcode in the array of function pointers)
        cpu_functions[opcode](cpu);
    }

    //print stuff from the cpu
    print_cpu_state(cpu);

    free(cpu);
    return 0;
}
