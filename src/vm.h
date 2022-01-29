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

#define LEN_WORD            sizeof(int64_t)
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
    Inst_Ret,
    Inst_Push,
    Inst_Pop,
    Inst_CallFunc,      // Call C function
    Inst_Add = 0x80,
    Inst_Sub,
    Inst_Mul,
    Inst_Div,
    Inst_Mod,
    Inst_Shr,
    Inst_Shl,
    Inst_And,
    Inst_Or,
    Inst_Xor,
    // Alias
    Inst_Cmp = Inst_Sub,
} Inst_e_t;

typedef enum _condition_e_
{
    Cond_None = 0,
    Cond_Eq   = 1,
    Cond_Neq  = 2,
    Cond_gt   = 3,
    Cond_lt   = 4,
    Cond_get  = 5,
    Cond_let  = 6,
} Cond_e_t;

typedef enum _exe_result_e_
{
    Exe_Done,
    Exe_Inst_Abort,
    Exe_Data_Abort,
} Exe_result_e_t;

typedef union _opcode_u_
{
    uint64_t u64;
    uint8_t  instruction;

    union
    {
        /*
        Simple type
            * Source from register
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            |Instruction 8b | Dest Reg 8b   |0| reserved 15b                |  Src Reg 8b   |   Reserved 24b                                |

            * Source from immediate value
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            |Instruction 8b | Dest Reg 8b   |1| reserved 15b                |            Immediate Value 32b                                | 
        */
        struct
        {
            uint8_t inst;
            uint8_t dest_reg_id;
            uint16_t param_type;        // upper 15b are reserved
            union
            {
                struct
                {
                    uint32_t id : 8;
                    uint32_t rsvd : 24;
                } reg;      // param_type = 0

                uint32_t imm_val; // param_type = 1
            } param;
        } simple_type;  // mov

        /*
        Arithmetic type
            * reg = reg OP reg
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            |Instruction 8b | Dest Reg 8b   |0 0|  rsvd 6b  |    Reg 0      |      Reg 1    |          Reserved 24b                         |

            * reg = reg OP imm
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            |Instruction 8b | Dest Reg 8b   |0 1|  rsvd 6b  |    Reg 0      |           Immediate Value 32b                                 |
            
            * reg = imm OP imm
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            |Instruction 8b | Dest Reg 8b   |1 0|  rsvd 6b  |    Immediate Value 0  16b     |    Immediate Value 1   16b    | reserved 8b   |
        */
        struct
        {
            uint8_t inst;
            uint8_t dest_reg_id;
            uint8_t param_type;    // upper 6b are reserved
            union
            {
                struct
                {
                    uint8_t reg0_id;
                    uint8_t reg1_id;
                    uint8_t rsvd[3];
                } __attribute__((aligned(1),packed)) reg_reg;       // operand_type = 00b

                struct
                {
                   uint8_t reg_id;
                   uint32_t imm_val;
                } __attribute__((aligned(1),packed)) reg_imm;       // operand_type = 01b

                struct
                {    
                   uint16_t imm_val0;
                   uint16_t imm_val1;
                   uint8_t rsvd;
                } __attribute__((aligned(1),packed)) imm_imm;       // operand_type = 10b
            } param;
        } arithmetic_type;   // add, sub, mul ..... and, or, xor

       /*
       Jump type
            * Register based PC relative addressing
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|0 0| Rsvd 6b   | Cnd | Rsvd 5b | Register ID 8b|              Reserved 32b 

            * Immediate PC relative addressing
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|0 1| Rsvd 6b   | Cnd | Rsvd 5b |      Immediate value 32b                                      |  Reserved 8b

            * Register based addressing
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|1 0| Rsvd 6b   | Cnd | Rsvd 5b | Register ID 8b|              Reserved 32b 

            * Immediate addressing
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|1 1| Rsvd 6b   | Cnd | Rsvd 5b |      Immediate value 32b                                      |  Reserved 8b
       */
        struct
        {
            uint8_t inst;
            uint8_t param_type;   // upper 6b are reserved
            uint8_t condition;  // upper 6b are reserved - Refer Cond_e_t

            union
            {
                struct
                {
                    uint8_t id;
                    uint32_t rsvd;
                } __attribute__((aligned(1),packed)) reg_based;

                struct
                {
                    uint32_t value;
                    uint8_t rsvd;
                } __attribute__((aligned(1),packed)) imm;
            } param;
        } jump_type;      // jmp, brn

        /*
        Memory type
            * Store : [reg ...] = reg0 ... regN
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|0 0|   Rsvd 6b | Dest Reg ID 8b|P| Rsvd 7b     |    Src Reg ID bitmap 32b

            * Store : [imm ...] = reg0 ... regN
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|0 1|   Rsvd 6b |Pge| Rsvd 6b   |  Src Reg ID bitmap 16b        |  Dst Immediate address 16b    | Base Addr Reg

            * Store : [reg] = imm
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|1 0|   Rsvd 6b |  Dst Reg ID 8b|      Src Immediate value 32b                                  | Rsvd 8b

            * Store : [imm] = imm
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|1 1|   Rsvd 6b | Dst Immediate value 16b       |  Src Immediate value 16b      | Base Addr Reg | Reserved 8b

            * Load  : reg0 ... regN = [reg ...]
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|0 0|   Rsvd 6b | Src Reg ID 8b |P| Rsvd 7b     |    Dst Reg ID bitmap 32b

            * Load  : reg0 ... regN = [imm ...]
            +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
            | Instruction 8b|0 1|   Rsvd 6b |Pge| Rsvd 6b   |  Dst Reg ID bitmap 16b        |  src Immediate value 16b      | Base Addr Reg
        */
        struct
        {
            uint8_t inst;
            uint8_t param_type;     // upper 6b are reserved

            union
            {
                struct
                {
                    uint8_t reg_id;
                    uint8_t reg_bitmap_page_1b;     // use only 1bit[0:0~31, 1:32~63]  upper 7b are reserved
                    uint32_t reg_bitmap;
                } __attribute__((aligned(1),packed)) reg_reg_bmp; // Store : [reg ...] = reg0 ... regN  | Load  : reg0 ... regN = [reg ...]

                struct
                {
                    uint8_t reg_id;
                    uint32_t imm_val;
                    uint8_t rsvd;
                } __attribute__((aligned(1),packed)) reg_imm;  // Store : [reg] = imm

                struct
                {
                    uint8_t reg_bitmap_page_2b;     // use only 2b [00:0~15, 01:16~31, 10:32~47, 11:48~63]
                    uint16_t reg_bitmap;
                    uint16_t imm_addr;
                    uint8_t base_reg_id;
                } __attribute__((aligned(1),packed)) imm_reg_bmp;  // Store : [[base_reg_id] + imm ...] = reg0 ... regN | Load  : reg0 ... regN = [[base_reg_id] + imm ...]

                struct
                {
                    uint16_t imm_addr;
                    uint16_t imm_val;
                    uint8_t base_reg_id;
                    uint8_t rsvd;
                } __attribute__((aligned(1),packed)) imm_imm;  // Store : [imm] = imm
            } param;
        } memory_type;

        /*
        Stack type
                * Push : reg0, reg1 ... regN
                * Pop : reg0, reg1 ... regN
                +-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-*-+-+-+-+-+-+-+-+
                | Instruction 8b|               Reserved 24b                    |      Register bit map 32b 
        */
       struct
       {
            uint32_t inst : 8;
            uint32_t reserved : 24;
            uint32_t reg_bitmap;
       } stack_type;
    } bytes;
} Opcode_u_t;

#define SIMPLE_TYPE_REG     (0)
#define SIMPLE_TYPE_IMM     (1)

#define MEMORY_TYPE_STR_REG_REG     (0)
#define MEMORY_TYPE_STR_IMM_REG     (1)
#define MEMORY_TYPE_STR_REG_IMM     (2)
#define MEMORY_TYPE_STR_IMM_IMM     (3)

#define MEMORY_TYPE_LDR_REG_REG     (0)
#define MEMORY_TYPE_LDR_REG_IMM     (1)

#define REG_PAGE_32_0           (0)
#define REG_PAGE_32_1           (1)
#define REG_PAGE_16_0           (0)
#define REG_PAGE_16_1           (1)
#define REG_PAGE_16_2           (2)
#define REG_PAGE_16_3           (3)

typedef union _status_reg_u_
{
    uint64_t u64;

    struct {
        struct
        {
            uint8_t negative    : 1;
            uint8_t zero        : 1;
            uint8_t carry       : 1;
            uint8_t overflow    : 1;
            uint8_t rsvc        : 4;
        } u8_0;

        uint8_t reserved[7];
    } bytes;
} Stsr_u_t;

typedef struct _reg_s_
{
    uint64_t pc;
    uint64_t sp;
    uint64_t fp;
    uint64_t ld;
    Stsr_u_t status;
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


void        vm_init(void);
uint64_t    vm_fetch(void);
Opcode_u_t  vm_decode(uint64_t op_bin);
void        vm_execute(Opcode_u_t opcode);
void        vm_abort(Exe_result_e_t exe_ret);