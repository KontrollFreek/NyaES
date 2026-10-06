#pragma once

#include <stdbool.h>

typedef struct Flags {
    unsigned char carry : 1;
    unsigned char zero : 1;
    unsigned char interrupt_disable : 1;
    unsigned char decimal : 1;
    unsigned char overflow : 1;
    unsigned char negative : 1;
} Flags;

typedef struct Registers {
    unsigned short program_counter;
    unsigned char a, x, y;
} Registers;

typedef struct AddressSpace {
    unsigned char ram[0x0800];
    unsigned char rom[0x8000];
} AddressSpace;

typedef struct RomData {
    unsigned char header[0x10];
    unsigned short reset_addr;
} RomData;

typedef struct CPU {
    Registers registers;
    bool cpu_halted;
    unsigned short remaining_cpu_cycles;
    Flags flags;
} CPU;

typedef struct NES {
    AddressSpace address_space;
    RomData rom_data;
    CPU cpu;
} NES;

void load_rom(NES* nes, const char* name);
void reset(NES* nes);

unsigned char* get_addr(NES* nes, unsigned short address);
unsigned char* get_program_counter(NES* nes);
unsigned char read_addr(NES* nes, unsigned short address);
unsigned char read_program_counter(NES* nes);
void write_addr(NES* nes, unsigned short address, unsigned char value);

void step_cpu(NES* nes);
