#include <stdio.h>
#include <stdlib.h>

enum Op { LOAD, ADD, PRINT, HALT };
struct CPU {
    int registers[8];
    int memory[256];
    int pc;           // program counter — tracks which instruction we're on
    int running;
};

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

    printf("calling load\n");

    //get which register to load the number into    
    int register_num = cpu->memory[cpu->pc + 1];
    //number to load into register
    int value =   cpu->memory[cpu->pc + 2];
 


    //either one is correct. the -> is more industry standard (accessing field of what struct you are pointing to)
    //(*cpu).registers[register_num] = value;
    //store the value in the register
    cpu -> registers[register_num] = value;

    //update counter 
    cpu->pc += 3;
}

//load a value into the given register 
void add(struct CPU *cpu) {

    printf("calling add\n");

    //get the register you want the sum to be stored in
    int reg_destination =  cpu->memory[cpu->pc + 1];

    //registers to add
    int register_num1 =  cpu->memory[cpu->pc + 2];
    int register_num2 = cpu->memory[cpu->pc + 3];

    //add the registers and store it in register destination
    cpu -> registers[reg_destination] = cpu -> registers[register_num1] + cpu -> registers[register_num2];
    cpu->pc += 4;
}
//load a value into the given register 
void print_reg_value(struct CPU *cpu) {
    printf("calling PRINT\n");
    
    //register to print
    int register_num = cpu->memory[cpu->pc+1];
    //value of the register
    int value = cpu -> registers[register_num];

    cpu->pc += 2;

    printf("the value of register number %d is %d \n", register_num, value);
}

//load a value into the given register 
void halt(struct CPU *cpu) {
    //set running to 0 so infinite loop stops running
    printf("calling halt\n");
    cpu->running = 0;
    cpu->pc += 1;
}

    // --- SYNTAX REFERENCE ----
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
    // ------------------------


int main() {
    //hard coded program
    int program[] = { LOAD, 0, 5,  LOAD, 1, 10,  ADD, 2, 1, 0,  PRINT, 2,  HALT };
    int len_program = sizeof(program) / sizeof(program[0]);


    //create a cpu struct pointer. calloc initialized everything to 0
    // cpu          // 0x5000  (the address)
    // *cpu         // the entire struct at 0x5000
    // cpu->pc      // the pc field inside the struct at 0x5000
    // (*cpu).pc    // exact same thing, just uglier
    struct CPU *cpu =  calloc(1, sizeof(struct CPU));

    //copy the program into memory
    for (int i = 0; i < len_program; i++) {
        cpu->memory[i] = program[i];
    }



    //create an array of pointers to functions
    //syntax: return value is void (array of 4 pointers) (input parameter for each function pointer)
    void (*cpu_functions[4])(struct CPU *) = {load, add, print_reg_value, halt};






    //while loop that goes over whole program in memory
    cpu->running = 1;
    while (cpu->running) {
        //get the opcode that the pointer is pointing to in  memory
        int opcode = cpu->memory[cpu->pc];
        
        //call the correct function given the opcode (look up the opcode in the array of function pointers)
        cpu_functions[opcode](cpu);
       
    
    }
    
    
    //print stuff from the cpu.
    print_cpu_state(cpu);
    
    free(cpu);
    return 0;
}
