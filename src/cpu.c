#include "nes.h"

#include <stdio.h>


#define __inc_cycles(x) (nes->cpu.remaining_cpu_cycles = x)

unsigned char read_zero_page(NES* nes) {
    unsigned char temp = read_program_counter(nes);
    return read_addr(nes, temp);
}

unsigned short read_absolute(NES* nes) {
    unsigned char* temp = get_addr(nes, nes->cpu.registers.program_counter);
    nes->cpu.registers.program_counter += 2;

    return ((unsigned short)read_addr(nes, temp[1]) << 8) | (read_addr(nes, temp[0]));
}

void step_cpu(NES* nes) {
    if (nes->cpu.remaining_cpu_cycles > 0) {
        --nes->cpu.remaining_cpu_cycles;
        return;
    }

    unsigned char opcode = read_program_counter(nes);
    switch (opcode) {
        case 0x02: // HLT
            nes->cpu.cpu_halted = true;
            break;


    #define __load_immediate(reg) \
        do { \
            unsigned char val = read_program_counter(nes); \
            reg = val; \
            nes->cpu.flags.zero = val == 0; \
            nes->cpu.flags.negative = val & 0x80; \
            __inc_cycles(2); \
        } while (0)

        case 0xA9: // LDA Immediate
            __load_immediate(nes->cpu.registers.a);
            break;
        case 0xA2: // LDX Immediate
            __load_immediate(nes->cpu.registers.x);
            break;
        case 0xA0: // LDY Immediate
            __load_immediate(nes->cpu.registers.y);
            break;


    #define __load_zero_page(reg) \
        do { \
            unsigned char val = read_zero_page(nes); \
            reg = val; \
            nes->cpu.flags.zero = val == 0; \
            nes->cpu.flags.negative = val & 0x80; \
            __inc_cycles(3); \
        } while (0)

        case 0xA5: // LDA Zero Page
            __load_zero_page(nes->cpu.registers.a);
            break;
        case 0xA6: // LDX Zero Page
            __load_zero_page(nes->cpu.registers.x);
            break;
        case 0xA4: // LDY Zero Page
            __load_zero_page(nes->cpu.registers.y);
            break;


    #define __store_zero_page(reg) \
        do { \
            unsigned char addr = read_program_counter(nes); \
            write_addr(nes, addr, reg); \
            __inc_cycles(3); \
        } while (0)

        case 0x85: // STA Zero Page
            __store_zero_page(nes->cpu.registers.a);
            break;
        case 0x86: // STX Zero Page
            __store_zero_page(nes->cpu.registers.x);
            break;
        case 0x84: // STY Zero Page
            __store_zero_page(nes->cpu.registers.y);
            break;


    #define __store_absolute(reg) \
        do { \
            unsigned short addr = read_absolute(nes); \
            write_addr(nes, addr, reg); \
            __inc_cycles(4); \
        } while (0)

        case 0x8D: // STA Absolute
            __store_absolute(nes->cpu.registers.a);
            break;
        case 0x8E: // STX Absolute
            __store_absolute(nes->cpu.registers.x);
            break;
        case 0x8C: // STX Absolute
            __store_absolute(nes->cpu.registers.y);
            break;


    #define __branch(x) \
        do { \
            unsigned char temp = read_program_counter(nes); \
            if (x) { \
                unsigned short old_pc = nes->cpu.registers.program_counter; \
                nes->cpu.registers.program_counter += (char)temp; \
                if ((old_pc & 0xFF) == (nes->cpu.registers.program_counter & 0xFF)) \
                    __inc_cycles(3); \
                else \
                    __inc_cycles(4); \
            } \
            else { \
                __inc_cycles(2); \
            } \
        } while (0)

        case 0x10: // BPL
            __branch(!nes->cpu.flags.negative);
            break;
        case 0x30: // BMI
            __branch(nes->cpu.flags.negative);
            break;
        case 0x50: // BVC
            __branch(!nes->cpu.flags.overflow);
            break;
        case 0x70: // BVS
            __branch(nes->cpu.flags.overflow);
            break;
        case 0x90: // BCC
            __branch(!nes->cpu.flags.carry);
            break;
        case 0xB0: // BCS
            __branch(nes->cpu.flags.carry);
            break;
        case 0xD0: // BNE
            __branch(!nes->cpu.flags.zero);
            break;
        case 0xF0: // BEQ
            __branch(nes->cpu.flags.zero);
            break;
        

        case 0xEA: // NOP
            __inc_cycles(2);
            break;

        default:
            fprintf(stderr, "unknown opcode: 0x%x\n", opcode);
            __inc_cycles(2);
            break;
    }
    
    --nes->cpu.remaining_cpu_cycles;
}