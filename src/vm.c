#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vm.h"

#define INC_PC()                 g_Regs.pc += LEN_WORD
#define INC_SP(N)                g_Regs.sp += (LEN_WORD * N)
#define DEC_SP(N)                g_Regs.sp -= (LEN_WORD * N)
#define VM_ASSERT(code, msg)     do{fprintf(stderr, msg); assert(1 == code);}while(0)

Reg_s_t g_Regs = {0};
Mem_s_t g_Mem = {0};

static void _assign_more_mem(Virtual_mem_s_t *vmem)
{
    vmem->size_byte += GRAN_MEM;
    vmem->mem = (uint8_t*)realloc(vmem->mem, vmem->size_byte);
}

static Exe_result_e_t _exe_stack_inst(Opcode_u_t opcode)
{
    uint64_t bitmap = opcode.bytes.stack_type.reg_bitmap;
    uint32_t reg_num = 0;

    while (bitmap)
    {
        if (bitmap & 1)
        {
            if (Inst_Push == opcode.instruction)
            {
                uint64_t src = g_Regs.r[reg_num];
                *(uint64_t*)g_Regs.sp = src;
                INC_SP(1);
            }
            else if (Inst_Pop == opcode.instruction)
            {
                DEC_SP(1);
                uint64_t src = *(uint64_t*)g_Regs.sp;
                g_Regs.r[reg_num] = src;
            }
            else
            {
                VM_ASSERT(3376, "unexpected instruction, must be push, pop");
            }
        }
        bitmap = bitmap >> 1;
        reg_num++;
    }
    return Exe_Done;
}

void vm_init(void)
{
    _assign_more_mem(&g_Mem.data);
    _assign_more_mem(&g_Mem.inst);
    _assign_more_mem(&g_Mem.stack);

    g_Regs.sp = (uint64_t)g_Mem.stack.mem;
    g_Regs.pc = (uint64_t)g_Mem.inst.mem;
}

uint64_t vm_fetch(void)
{
    uint64_t op_bin = *(uint64_t*)g_Regs.pc;
    INC_PC();
    return op_bin;
}

Opcode_u_t vm_decode(uint64_t op_bin)
{
    Opcode_u_t opcode = {0};
    opcode.u64 = op_bin;

    return opcode;
}

void vm_execute(Opcode_u_t opcode)
{
    Inst_e_t inst = (Inst_e_t)opcode.instruction;
    Exe_result_e_t ret;

    switch (inst)
    {
        case Inst_Nop:
            INC_PC();
        break;

        case Inst_Mov:
        break;

        case Inst_Ldr:
        case Inst_Str:
        break;

        case Inst_Jmp:
        case Inst_Brn:
        break;

        case Inst_Ret:
        break;

        case Inst_Push:
        case Inst_Pop:
        ret = _exe_stack_inst(opcode);
        break;

        case Inst_Add:
        case Inst_Sub:
        case Inst_Mul:
        case Inst_Div:
        case Inst_Mod:
        case Inst_Shr:
        case Inst_Shl:
        case Inst_And:
        case Inst_Or:
        case Inst_Xor:
        break;

        default:
            ret = Exe_Inst_Abort;
    }

    vm_abort(ret);
}

void vm_abort(Exe_result_e_t exe_ret)
{
    switch (exe_ret)
    {
        case Exe_Done:
            return;
        case Exe_Inst_Abort:
            break;
        case Exe_Data_Abort:
            break;
    }
}