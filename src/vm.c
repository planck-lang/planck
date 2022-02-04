#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vm.h"

#define SUPPORT_STOP_VM_IMMEDIATE       (1)

#define INC_PC()                 g_Regs.pc += LEN_WORD
#define INC_SP(N)                g_Regs.sp += (LEN_WORD * N)
#define DEC_SP(N)                g_Regs.sp -= (LEN_WORD * N)

#if (1 == SUPPORT_STOP_VM_IMMEDIATE)
    #define VM_ASSERT(code, msg)     do{fprintf(stderr, msg); assert(1 == code);}while(0)
#else
    #define VM_ASSERT(code, msg)     // do nothing
#endif

Reg_s_t g_Regs = {0};
Mem_s_t g_Mem = {0};

static void _assign_more_mem(Virtual_mem_s_t *vmem)
{
    vmem->size_byte += GRAN_MEM;
    vmem->mem = (uint8_t*)realloc(vmem->mem, vmem->size_byte);
}

static uint32_t _get_lsb_bitmap_and_clear(uint32_t *bitmap)
{
    uint32_t max_bitmap = 32;

    for (uint32_t cnt = 0; cnt < max_bitmap ; cnt++)
    {
        if ((*bitmap >> cnt) & 1)
        {
            *bitmap &= ~(1<<cnt);   // clear bit
            return cnt;             // lsb offset (zero base)
        }
    }
    VM_ASSERT(0x220128, "Bitmap should not be zero");
    return max_bitmap;
}

static uint32_t _get_msb_bitmap_and_clear(uint32_t *bitmap)
{
    for (uint32_t cnt = 31; cnt >= 0 ; cnt--)
    {
        if ((*bitmap >> cnt) & 1)
        {
            *bitmap &= ~(1<<cnt);   // clear bit
            return cnt;             // msb offset (zero base)
        }
    }
    VM_ASSERT(0x22012802, "Bitmap should not be zero");
    return 32;
}

#define REG_ID_REG_BMP_LSB      0
#define REG_ID_REG_BMP_MSB      1
static uint32_t _get_reg_id_from_reg_bitmap(uint32_t *bitmap, uint32_t reg_page, uint32_t reg_page_granularity, uint32_t from)
{
    uint32_t reg_idx = 0;
    
    if (REG_ID_REG_BMP_LSB == from)
    {
        reg_idx = _get_lsb_bitmap_and_clear(bitmap);
    }
    else if (REG_ID_REG_BMP_MSB == from)
    {
        reg_idx = _get_msb_bitmap_and_clear(bitmap);
    }
    else
    {
        VM_ASSERT(0x22012802, "Invalid LSB, MSB flag");
    }

    return (reg_idx + (reg_page * reg_page_granularity));
}

static Exe_result_e_t _exe_stack_inst(Opcode_u_t opcode)
{
    uint32_t bitmap = opcode.bytes.stack_type.reg_bitmap;
    uint8_t reg_page = opcode.bytes.stack_type.reg_bitmap_page_1b;

    while (0 != bitmap)
    {
        if (Inst_Push == opcode.instruction)
        {
            uint32_t reg_idx = _get_reg_id_from_reg_bitmap(&bitmap, reg_page, 32, REG_ID_REG_BMP_LSB);
            uint64_t src = g_Regs.r[reg_idx];
            *(uint64_t*)g_Regs.sp = src;
            INC_SP(1);
        }
        else if (Inst_Pop == opcode.instruction)
        {
            DEC_SP(1);
            uint64_t src = *(uint64_t*)g_Regs.sp;
            uint32_t reg_idx = _get_reg_id_from_reg_bitmap(&bitmap, reg_page, 32, REG_ID_REG_BMP_MSB);
            g_Regs.r[reg_idx] = src;
        }
        else
        {
            VM_ASSERT(0x3376, "unexpected instruction, must be push, pop");
            return Exe_Inst_Abort;
        }
    }
    return Exe_Done;
}

static Exe_result_e_t _exe_mov_inst(Opcode_u_t opcode)
{
    uint32_t dest_reg_id = opcode.bytes.simple_type.dest_reg_id;

    if (0 == opcode.bytes.simple_type.param_type)
    {
        // Reg = Reg
        uint32_t src_reg_id = opcode.bytes.simple_type.param.reg.id;
        g_Regs.r[dest_reg_id] = g_Regs.r[src_reg_id];
    }
    else if (1 == opcode.bytes.simple_type.param_type)
    {
        // Reg = Imm
        uint32_t imm_val = opcode.bytes.simple_type.param.imm_val;
        g_Regs.r[dest_reg_id] = imm_val;
    }
    else
    {
        VM_ASSERT(0x0817, "unexpected param type, must be 0 or 1");
        return Exe_Inst_Abort;
    }
    
    return Exe_Done;
}

static Exe_result_e_t _exe_memory_inst(Opcode_u_t opcode)
{
    if (Inst_Str == opcode.instruction)
    {
        // [reg] = reg bitmap
        if (MEMORY_TYPE_STR_REG_REG == opcode.bytes.memory_type.param_type)
        {
            uint8_t reg_page = opcode.bytes.memory_type.param.reg_reg_bmp.reg_bitmap_page_1b;
            if (REG_PAGE_32_0 == reg_page || REG_PAGE_32_1 == reg_page)
            {
                uint32_t bitmap = opcode.bytes.memory_type.param.reg_reg_bmp.reg_bitmap;
                uint32_t dst_reg_idx = opcode.bytes.memory_type.param.reg_reg_bmp.reg_id;
                uint64_t *dst_mem = (uint64_t*)g_Regs.r[dst_reg_idx];

                while (0 != bitmap)
                {
                    uint32_t reg_idx = _get_reg_id_from_reg_bitmap(&bitmap, reg_page, 32, REG_ID_REG_BMP_LSB);
                    *dst_mem = g_Regs.r[reg_idx];
                    dst_mem++;
                }
            }
            else
            {
                VM_ASSERT(0x1120, "wrong REG page, must be 0 or 1 (1bit)");
                return Exe_Inst_Abort;
            }
        }
        // [imm] = reg bitmap
        if (MEMORY_TYPE_STR_IMM_REG == opcode.bytes.memory_type.param_type)
        {
            uint8_t reg_page = opcode.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b;
            if (REG_PAGE_16_0 == reg_page || REG_PAGE_16_1 == reg_page || REG_PAGE_16_2 == reg_page || REG_PAGE_16_3 == reg_page)
            {
                uint32_t bitmap = opcode.bytes.memory_type.param.imm_reg_bmp.reg_bitmap;
                uint32_t base_reg_idx = opcode.bytes.memory_type.param.imm_reg_bmp.base_reg_id;
                uint32_t imm_addr = opcode.bytes.memory_type.param.imm_reg_bmp.imm_addr;
                uint64_t* dst_mem = (uint64_t*)(g_Regs.r[base_reg_idx] + imm_addr);

                while (0 != bitmap)
                {
                    uint32_t reg_idx = _get_reg_id_from_reg_bitmap(&bitmap, reg_page, 16, REG_ID_REG_BMP_LSB);
                    *dst_mem = g_Regs.r[reg_idx];
                    dst_mem++;
                }
            }
            else
            {
                VM_ASSERT(0x1130, "wrong REG page, must be one of 0,1,2,3 (2bits)");
                return Exe_Inst_Abort;
            }
        }

        // [reg] = imm
        if (MEMORY_TYPE_STR_REG_IMM == opcode.bytes.memory_type.param_type)
        {
            uint32_t dst_reg_idx = opcode.bytes.memory_type.param.reg_imm.reg_id;
            uint32_t src_value = opcode.bytes.memory_type.param.reg_imm.imm_val;

            uint64_t* dst_mem = (uint64_t*)g_Regs.r[dst_reg_idx];
            *dst_mem = src_value;
        }

        // [imm] = imm
        if (MEMORY_TYPE_STR_IMM_IMM == opcode.bytes.memory_type.param_type)
        {
            uint32_t base_reg_idx = opcode.bytes.memory_type.param.imm_imm.base_reg_id;
            uint32_t imm_addr = opcode.bytes.memory_type.param.imm_imm.imm_addr;
            uint64_t* dst_mem = (uint64_t*)(g_Regs.r[base_reg_idx] + imm_addr);

            uint32_t src_value = opcode.bytes.memory_type.param.imm_imm.imm_val;
            *dst_mem = src_value;
        }
    }
    else if (Inst_Ldr == opcode.instruction)
    {
        // reg .... reg = [reg...]
        if (MEMORY_TYPE_LDR_REG_REG == opcode.bytes.memory_type.param_type)
        {
            uint8_t reg_page = opcode.bytes.memory_type.param.reg_reg_bmp.reg_bitmap_page_1b;
            if (REG_PAGE_32_0 == reg_page || REG_PAGE_32_1 == reg_page)
            {
                uint32_t bitmap = opcode.bytes.memory_type.param.reg_reg_bmp.reg_bitmap;
                uint32_t src_reg_idx = opcode.bytes.memory_type.param.reg_reg_bmp.reg_id;
                uint64_t *src_mem = (uint64_t*)g_Regs.r[src_reg_idx];

                while (0 != bitmap)
                {
                    uint32_t reg_idx = _get_reg_id_from_reg_bitmap(&bitmap, reg_page, 32, REG_ID_REG_BMP_LSB);
                    g_Regs.r[reg_idx] = *src_mem;
                    src_mem++;
                }
            }
            else
            {
                VM_ASSERT(0x22020300, "wrong REG page, must be 0 or 1 (1bit)");
                return Exe_Inst_Abort;
            }
        }
        // reg .... reg = [imm...]
        if (MEMORY_TYPE_LDR_REG_IMM == opcode.bytes.memory_type.param_type)
        {
            uint8_t reg_page = opcode.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b;
            if (REG_PAGE_16_0 == reg_page || REG_PAGE_16_1 == reg_page || REG_PAGE_16_2 == reg_page || REG_PAGE_16_3 == reg_page)
            {
                uint32_t bitmap = opcode.bytes.memory_type.param.imm_reg_bmp.reg_bitmap;
                uint32_t base_reg_idx = opcode.bytes.memory_type.param.imm_reg_bmp.base_reg_id;
                uint32_t imm_addr = opcode.bytes.memory_type.param.imm_reg_bmp.imm_addr;
                uint64_t* src_mem = (uint64_t*)(g_Regs.r[base_reg_idx] + imm_addr);

                while (0 != bitmap)
                {
                    uint32_t reg_idx = _get_reg_id_from_reg_bitmap(&bitmap, reg_page, 16, REG_ID_REG_BMP_LSB);
                    g_Regs.r[reg_idx] = *src_mem;
                    src_mem++;
                }
            }
            else
            {
                VM_ASSERT(0x22020301, "wrong REG page, must be one of 0,1,2,3 (2bits)");
                return Exe_Inst_Abort;
            }
        }
    }
    else
    {
        VM_ASSERT(0x1119, "unexpected instruction type, must be STR or LDR");
        return Exe_Inst_Abort;
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
    Exe_result_e_t ret = Exe_Inst_Abort;

    switch (inst)
    {
        case Inst_Nop:
            INC_PC();
        break;

        case Inst_Mov:
        ret = _exe_mov_inst(opcode);
        break;

        case Inst_Ldr:
        case Inst_Str:
        ret = _exe_memory_inst(opcode);
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