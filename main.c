#include <stdio.h>

enum Op { LOAD, ADD, PRINT, HALT };
struct CPU {
    int registers[8];
    int memory[256];
    int pc;           // program counter — tracks which instruction we're on
};

//print all registers and some memory
void print_cpu_state(struct CPU cpu) {
    
    // print registers
    printf("=== REGISTERS ===\n");
    for (int i = 0; i < 8; i++) {
        printf("R%d: %d\n", i, cpu.registers[i]);
    }

    // print first 16 slots of memory
    printf("\n=== MEMORY (first 16) ===\n");
    for (int i = 0; i < 16; i++) {
        printf("[%d]: %d\n", i, cpu.memory[i]);
    }
    
    printf("\nPC: %d\n", cpu.pc);

    
}

//load a value into the given register 
void load(struct CPU *cpu, int register_num, int value) {
    //either one is correct. the -> is more industry standard (accessing field of what struct you are pointing to)
    //(*cpu).registers[register_num] = value;
    cpu -> registers[register_num] = value;
}

//load a value into the given register 
void add(struct CPU *cpu, int reg_destination ,int register_num1, int register_num2) {
    cpu -> registers[reg_destination] = cpu -> registers[register_num1] + cpu -> registers[register_num2];
}
//load a value into the given register 
void print_reg_value(struct CPU *cpu, int register_num) {
    int value = cpu -> registers[register_num];
    printf("the value of register number %d is %d \n", register_num, value);
}


    // --- SYNTAX REFERENCE ---
    // Variables:      int x = 5;   char c = 'a';   float f = 3.14;
    // Arrays:         int arr[8] = {0};             // all zeros
    //                 arr[0] = 99;                  // set element
    //                 arr[0];                       // get element
    // Printf:         %d = int,  %c = char,  %f = float,  %s = string,  %p = pointer/address,  %x = hex
    //                 printf("val: %d\n", x);
    // For loop:       for (int i = 0; i < 8; i++) { }
    // If/else:        if (x == 5) { } else { }
    // Function:       int add(int a, int b) { return a + b; }
    // Struct:         struct CPU { int regs[8]; int memory[256]; };
    // Enum:           enum Op { LOAD, ADD, PRINT, HALT };
    //
    // Pointers:       int x = 5;         // x lives at some address, value is 5
    //                 int *p = &x;       // p stores the ADDRESS of x
    //                 x                  // value: 5
    //                 &x                 // address of x: e.g. 0x1000
    //                 p                  // same address: 0x1000
    //                 *p                 // follow the address, get value: 5
    //                 *p = 99;           // follow the address, SET value — x is now 99
    //                 int *p             // declare a pointer to int
    //                 struct CPU *p      // declare a pointer to a CPU struct
    //                 p->field           // access struct field through a pointer (same as (*p).field)
    //                 &cpu               // pass address of cpu to a function
    // -------------------------


int main() {
    //create a cpu struct and initialize fill everything inside to 0
    struct CPU cpu = {0};

    //hard coded program
    int program[] = { LOAD, 0, 5,  LOAD, 1, 10,  ADD, 2, 1, 0,  PRINT, 2,  HALT };
    int len_program = sizeof(program) / sizeof(program[0]);

    //copy the program into memory
    for (int i = 0; i < len_program; i++) {
        cpu.memory[i] = program[i];
    }

    //while loop that goes over whole program in memory
    int running = 1;
    while (running) {
        //get the opcode that the pointer is pointing to in memory
        int opcode = cpu.memory[cpu.pc];

        switch (opcode) {
            case LOAD: {
                load(&cpu, cpu.memory[cpu.pc + 1], cpu.memory[cpu.pc + 2]);
                printf("calling LOAD \n");
                cpu.pc += 3;
                break;
            }
            case ADD: {
                add(&cpu, cpu.memory[cpu.pc + 1], cpu.memory[cpu.pc + 2],cpu.memory[cpu.pc + 3]);
                printf("calling ADD \n");
                cpu.pc += 4;
                break;

            }
             case PRINT: {
                printf("calling PRINT\n");
                print_reg_value(&cpu,cpu.memory[cpu.pc+1]);
                cpu.pc += 2;
                break;

            }
             case HALT: {
                printf("call HALT\n");
                running = 0;
                cpu.pc += 1;
                break;

            }
            default: {
            printf("some other command\n");
            cpu.pc += 1;
            
            }
        }
    
    }
    
    
    //print stuff from the cpu
    print_cpu_state(cpu);
    

    return 0;
}
