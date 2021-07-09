/*
 * MIT License
 *
 * Copyright (c) 2021 Manwoo Yi
 *
 MIT License

Copyright (c) 2019 planck-lang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 
 */

#include <stdint.h>

#pragma once

#define NUM_REGS            64          // number of general registers
#define GRAN_MEM            4096        // garanularity of the memory size as byte

typedef enum _inst_e_
{
    Inst_Nop = 0x0,
    Inst_Mov,
    Inst_Ldr,
    Inst_Str,
    Inst_Jmp,
    Inst_Brn,
    Inst_Push,
    Inst_Pop,
    Inst_Add = 0x100,
    Inst_Sub,
    Inst_Mul,
    Inst_Div,
    Inst_Mod,
    Inst_Shr,
    Inst_Shl,
} Inst_e_t;

typedef struct _reg_s_
{
    uint64_t pc;
    uint64_t sp;
    uint64_t fp;
    uint64_t status;
    uint64_t r[NUM_REGS];
} Reg_s_t;

typedef struct _vir_mem_s_
{
    uint8_t *mem;
    uint64_t size_byte;
    uint64_t current_use;
} Virtual_mem_s_t;

typedef struct _mem_s_
{
    Virtual_mem_s_t stack;
    Virtual_mem_s_t data;
    Virtual_mem_s_t inst;
} Mem_s_t;


void vm_init(void);