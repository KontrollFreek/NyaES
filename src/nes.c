#include "nes.h"

#include <stdio.h>

#define __len_bitmask(x) ~((unsigned long long)~0 << x)

void load_rom(NES* nes, const char* name) {
    FILE* rom_file;
    fopen_s(&rom_file, name, "rb");

    fgets((char*)nes->rom_data.header, 0x10, rom_file);
    fseek(rom_file, 1, SEEK_CUR); // for some reason the pointer is set one character behind
    fgets((char*)nes->address_space.rom, 0x8000, rom_file);

    nes->rom_data.reset_addr = ((unsigned short)read_addr(nes, 0xFFFD) << 8) | (read_addr(nes, 0xFFFC));

    fclose(rom_file);
}

void reset(NES* nes) {
    nes->cpu.flags.interrupt_disable = true;
    nes->cpu.registers.program_counter = nes->rom_data.reset_addr;
}



unsigned char* get_addr(NES* nes, unsigned short address) {
    // RAM space
    if (address < 0x8000) {
        // Mask the lower 11 bits to mirror the 4 sections of RAM
        address &= __len_bitmask(11);
        return nes->address_space.ram + address;
    }
    // ROM space
    else {
        // ROM starts at 0x8000 but reads from our buffer starting from 0
        address -= 0x8000;
        return nes->address_space.rom + address;
    }
}

unsigned char* get_program_counter(NES* nes) {
    return get_addr(nes, nes->cpu.registers.program_counter++);
}

unsigned char read_addr(NES* nes, unsigned short address) {
    return *get_addr(nes, address);
}

unsigned char read_program_counter(NES* nes) {
    return *get_program_counter(nes);
}

void write_addr(NES* nes, unsigned short address, unsigned char value) {
    if (address < 0x8000)
        *get_addr(nes, address) = value;
}