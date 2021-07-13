
#include <stdlib.h>

#include "vm.h"

#define INC_PC()    g_Regs.pc += LEN_WORD
#define INC_SP(N)   g_Regs.sp += (LEN_WORD * N)

Reg_s_t g_Regs = {0};
Mem_s_t g_Mem = {0};

static void _assign_more_mem(Virtual_mem_s_t *vmem)
{
    vmem->size_byte += GRAN_MEM;
    vmem->mem = (uint8_t*)realloc(vmem->mem, vmem->size_byte);
}

void vm_init(void)
{
    _assign_more_mem(&g_Mem.data);
    _assign_more_mem(&g_Mem.inst);
    _assign_more_mem(&g_Mem.stack);
}

uint64_t vm_fetch(void)
{
    return (g_Mem.inst.mem[g_Regs.pc]);
}

void vm_decode(uint64_t op_bin)
{
    Opcode_u_t opcode = {0};
    opcode.u64 = op_bin;

    Inst_e_t inst = (Inst_e_t)opcode.instruction;

    switch (inst)
    {
        case Inst_Nop:
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
         // Error
    }
}