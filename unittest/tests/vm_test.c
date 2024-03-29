#include "TestMain.h"

#include "vm.h"

extern Mem_s_t g_Mem;
extern Reg_s_t g_Regs;

REGISTER_SUITE_AUTO(vm_test, "01_vm_tests")

PRE_TEST_FUNC()
{
    vm_init();
}

TESTCASE(1, "push test")
{
    Opcode_u_t push_op = {.instruction = Inst_Push};

    // add testing value to the registers
    g_Regs.r[8] = 0x32423;
    g_Regs.r[18] = 0x888234;
    g_Regs.r[11] = 0x88928452;
    g_Regs.r[12] = 0x882994892004;
    g_Regs.r[10] = 0x986523;
    g_Regs.r[13] = 0x724;
    g_Regs.r[15] = 0x2324242;

    // Register page 0 (Reg0 - Reg31)
    push_op.bytes.stack_type.reg_bitmap_page_1b = REG_PAGE_32_0;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 8;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 18;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 11;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 12;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 10;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 13;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 15;

    ASSERT_EQ_UINT(0x4bd00, push_op.bytes.stack_type.reg_bitmap);
    ASSERT_EQ_UINT(0x4bd0000000007, push_op.u64);

    *(uint64_t*)g_Regs.pc = push_op.u64;

    uint64_t op_bin = vm_fetch();
    ASSERT_EQ_UINT(0x4bd0000000007, op_bin);

    Opcode_u_t opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT((LEN_WORD * 7) + g_Mem.stack.mem, g_Regs.sp);  // test case writes 7 registers

    ASSERT_EQ_UINT(0x32423, *(uint64_t*)g_Mem.stack.mem); // g_Regs.r[8] = 0x32423;
    ASSERT_EQ_UINT(0x986523, *(uint64_t*)(g_Mem.stack.mem + LEN_WORD)); // g_Regs.r[10] = 0x986523;
    ASSERT_EQ_UINT(0x88928452, *(uint64_t*)(g_Mem.stack.mem + (LEN_WORD * 2))); // g_Regs.r[11] = 0x88928452;
    ASSERT_EQ_UINT(0x882994892004, *(uint64_t*)(g_Mem.stack.mem + (LEN_WORD * 3))); // g_Regs.r[12] = 0x882994892004;
    ASSERT_EQ_UINT(0x724, *(uint64_t*)(g_Mem.stack.mem + (LEN_WORD * 4))); // g_Regs.r[13] = 0x724;
    ASSERT_EQ_UINT(0x2324242, *(uint64_t*)(g_Mem.stack.mem + (LEN_WORD * 5))); // g_Regs.r[15] = 0x2324242;
    ASSERT_EQ_UINT(0x888234, *(uint64_t*)(g_Mem.stack.mem + (LEN_WORD * 6))); // g_Regs.r[18] = 0x888234;

    // Register page 1 (Reg32 - Reg63)
    g_Regs.r[40] = 0xF32423;
    g_Regs.r[50] = 0xF888234;
    g_Regs.r[43] = 0xF88928452;
    g_Regs.r[44] = 0xF882994892004;
    g_Regs.r[42] = 0xF986523;
    g_Regs.r[45] = 0xF724;
    g_Regs.r[47] = 0xF2324242;

    push_op.bytes.stack_type.reg_bitmap_page_1b = REG_PAGE_32_1;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 8;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 18;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 11;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 12;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 10;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 13;
    push_op.bytes.stack_type.reg_bitmap |= 1 << 15;

    *(uint64_t*)g_Regs.pc = push_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    uint8_t* test_base = (uint8_t*)(g_Regs.sp - (LEN_WORD * 7));        // 7 values are pushed into stack
    ASSERT_EQ_UINT(0xF32423, *(uint64_t*)test_base); // g_Regs.r[8] = 0x32423;
    ASSERT_EQ_UINT(0xF986523, *(uint64_t*)(test_base + LEN_WORD)); // g_Regs.r[10] = 0x986523;
    ASSERT_EQ_UINT(0xF88928452, *(uint64_t*)(test_base + (LEN_WORD * 2))); // g_Regs.r[11] = 0x88928452;
    ASSERT_EQ_UINT(0xF882994892004, *(uint64_t*)(test_base + (LEN_WORD * 3))); // g_Regs.r[12] = 0x882994892004;
    ASSERT_EQ_UINT(0xF724, *(uint64_t*)(test_base + (LEN_WORD * 4))); // g_Regs.r[13] = 0x724;
    ASSERT_EQ_UINT(0xF2324242, *(uint64_t*)(test_base + (LEN_WORD * 5))); // g_Regs.r[15] = 0x2324242;
    ASSERT_EQ_UINT(0xF888234, *(uint64_t*)(test_base + (LEN_WORD * 6))); // g_Regs.r[18] = 0x888234;
}

TESTCASE(2, "pop test")
{
    Opcode_u_t pop_op = {.instruction = Inst_Pop};

    // Register page 0 (Reg0 - Reg31)
    pop_op.bytes.stack_type.reg_bitmap_page_1b = REG_PAGE_32_0;
    pop_op.bytes.stack_type.reg_bitmap |= 1 << 31;
    pop_op.bytes.stack_type.reg_bitmap |= 1 << 28;
    pop_op.bytes.stack_type.reg_bitmap |= 1 << 20;
    pop_op.bytes.stack_type.reg_bitmap |= 1 << 17;
    pop_op.bytes.stack_type.reg_bitmap |= 1 << 15;
    pop_op.bytes.stack_type.reg_bitmap |= 1 << 10;
    pop_op.bytes.stack_type.reg_bitmap |= 1 << 5;

    ASSERT_EQ_UINT(0x90128420, pop_op.bytes.stack_type.reg_bitmap);
    ASSERT_EQ_UINT(0x9012842000000008, pop_op.u64);

    *(uint64_t*)g_Regs.pc = pop_op.u64;

    uint64_t op_bin = vm_fetch();
    ASSERT_EQ_UINT(0x9012842000000008, op_bin);

    Opcode_u_t opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0xF32423, g_Regs.r[5]);
    ASSERT_EQ_UINT(0xF986523, g_Regs.r[10]);
    ASSERT_EQ_UINT(0xF88928452, g_Regs.r[15]);
    ASSERT_EQ_UINT(0xF882994892004, g_Regs.r[17]);
    ASSERT_EQ_UINT(0xF724, g_Regs.r[20]);
    ASSERT_EQ_UINT(0xF2324242, g_Regs.r[28]);
    ASSERT_EQ_UINT(0xF888234, g_Regs.r[31]);

    // Register page 1 (Reg32 - Reg63)
    pop_op.bytes.stack_type.reg_bitmap_page_1b = REG_PAGE_32_1;

    *(uint64_t*)g_Regs.pc = pop_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x32423, g_Regs.r[5 + 32]);
    ASSERT_EQ_UINT(0x986523, g_Regs.r[10 + 32]);
    ASSERT_EQ_UINT(0x88928452, g_Regs.r[15 + 32]);
    ASSERT_EQ_UINT(0x882994892004, g_Regs.r[17 + 32]);
    ASSERT_EQ_UINT(0x724, g_Regs.r[20 + 32]);
    ASSERT_EQ_UINT(0x2324242, g_Regs.r[28 + 32]);
    ASSERT_EQ_UINT(0x888234, g_Regs.r[31 + 32]);

    ASSERT_EQ_UINT(g_Mem.stack.mem, g_Regs.sp); // all stack vaules are poped out
}

TESTCASE(3, "mov test")
{
    Opcode_u_t mov_op = {.instruction = Inst_Mov};
    
    // Reg = Reg
    mov_op.bytes.simple_type.param_type = SIMPLE_TYPE_REG;

    g_Regs.r[34] = 0x20210817;
    g_Regs.r[43] = 0;

    mov_op.bytes.simple_type.dest_reg_id = 43;
    mov_op.bytes.simple_type.param.reg.id = 34;

    *(uint64_t*)g_Regs.pc = mov_op.u64;

    uint64_t op_bin = vm_fetch();
    ASSERT_EQ_UINT(0x0000002200002b01, op_bin);

    Opcode_u_t opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x20210817, g_Regs.r[43]);

    // Reg = Imm
    mov_op.u64 = 0; // reset
    mov_op.instruction = Inst_Mov;

    mov_op.bytes.simple_type.param_type = SIMPLE_TYPE_IMM;

    g_Regs.r[53] = 0x99939820; // dummy

    mov_op.bytes.simple_type.dest_reg_id = 53;
    mov_op.bytes.simple_type.param.imm_val = 0x20210818;

    *(uint64_t*)g_Regs.pc = mov_op.u64;

    op_bin = vm_fetch();
    ASSERT_EQ_UINT(0x2021081800013501, op_bin);

    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x20210818, g_Regs.r[53]);
}

TESTCASE(4, "nop test")
{
    Opcode_u_t nop_op = {.instruction = Inst_Nop};

    uint64_t last_pc_addr = g_Regs.pc;

    *(uint64_t*)g_Regs.pc = nop_op.u64;

    uint64_t op_bin = vm_fetch();
    Opcode_u_t opcode = vm_decode(op_bin);
    vm_execute(opcode);

    // PC was increased when run the vm_fetch()
    // and PC was increased one more time when executes the NOP instruction
    // so, expected PC is increased as (LEN_WORD * 2)
    ASSERT_EQ_UINT(last_pc_addr + (LEN_WORD * 2), g_Regs.pc);
}

TESTCASE(5, "str test")
{
    Opcode_u_t str_op = {.instruction = Inst_Str};

    // [reg] = reg bitmap, page 0 (reg0 ~ reg31)
    g_Regs.r[5] = 0x55;
    g_Regs.r[9] = 0x99;
    g_Regs.r[13] = 0xdd;
    g_Regs.r[22] = 0x16;
    g_Regs.r[30] = 0x1e;

    g_Regs.r[48] = (uint64_t)(g_Mem.data.mem + 0x8);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_REG_REG;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_id = 48;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap_page_1b = REG_PAGE_32_0;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap = 0;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 5);
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 9);
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 13);
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 22);
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 30);

    *(uint64_t*)g_Regs.pc = str_op.u64;

    uint64_t op_bin = vm_fetch();
    Opcode_u_t opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x55, *(uint64_t*)(g_Mem.data.mem + 0x8));
    ASSERT_EQ_UINT(0x99, *(uint64_t*)(g_Mem.data.mem + 0x8 + (LEN_WORD * 1)));
    ASSERT_EQ_UINT(0xdd, *(uint64_t*)(g_Mem.data.mem + 0x8 + (LEN_WORD * 2)));
    ASSERT_EQ_UINT(0x16, *(uint64_t*)(g_Mem.data.mem + 0x8 + (LEN_WORD * 3)));
    ASSERT_EQ_UINT(0x1e, *(uint64_t*)(g_Mem.data.mem + 0x8 + (LEN_WORD * 4)));

    // reset
    str_op.u64 = 0;
    str_op.instruction = Inst_Str;

    // [reg] = reg bitmap, page 1 (reg32 ~ reg63)
    g_Regs.r[32] = 0x3322;
    g_Regs.r[43] = 0x4433;
    g_Regs.r[57] = 0x5577;
    g_Regs.r[61] = 0x66ee;
    g_Regs.r[63] = 0x33dd;

    g_Regs.r[30] = (uint64_t)(g_Mem.data.mem + 0x30);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_REG_REG;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_id = 30;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap_page_1b = REG_PAGE_32_1;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap = 0;
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (32 - 32));
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (43 - 32));
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (57 - 32));
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (61 - 32));
    str_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (63 - 32));

    *(uint64_t*)g_Regs.pc = str_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x3322, *(uint64_t*)(g_Mem.data.mem + 0x30));
    ASSERT_EQ_UINT(0x4433, *(uint64_t*)(g_Mem.data.mem + 0x30 + (LEN_WORD * 1)));
    ASSERT_EQ_UINT(0x5577, *(uint64_t*)(g_Mem.data.mem + 0x30 + (LEN_WORD * 2)));
    ASSERT_EQ_UINT(0x66ee, *(uint64_t*)(g_Mem.data.mem + 0x30 + (LEN_WORD * 3)));
    ASSERT_EQ_UINT(0x33dd, *(uint64_t*)(g_Mem.data.mem + 0x30 + (LEN_WORD * 4)));

    // reset
    str_op.u64 = 0;
    str_op.instruction = Inst_Str;

    // [imm] = reg bitmap, page 0 (reg0 ~ reg15)
    g_Regs.r[5] = 0x55aa;
    g_Regs.r[9] = 0x99cd;
    g_Regs.r[13] = 0xdd8498;

    g_Regs.r[17] = (uint64_t)(g_Mem.data.mem + 0x60);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_IMM_REG;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_0;
    str_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 17;
    str_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x230;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << 5);
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << 9);
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << 13);

    *(uint64_t*)g_Regs.pc = str_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x55aa, *(uint64_t*)(g_Mem.data.mem + 0x60 + 0x230));
    ASSERT_EQ_UINT(0x99cd, *(uint64_t*)(g_Mem.data.mem + 0x60 + 0x230 + (LEN_WORD * 1)));
    ASSERT_EQ_UINT(0xdd8498, *(uint64_t*)(g_Mem.data.mem + 0x60 + 0x230 + (LEN_WORD * 2)));

    // reset
    str_op.u64 = 0;
    str_op.instruction = Inst_Str;
    
    // [imm] = reg bitmap, page 1 (reg16 ~ reg31)
    g_Regs.r[17] = 0xaabb;
    g_Regs.r[28] = 0xcdef;
    g_Regs.r[30] = 0x8498dd;

    g_Regs.r[12] = (uint64_t)(g_Mem.data.mem + 0x70);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_IMM_REG;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_1;
    str_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 12;
    str_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x130;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (17 - 16));
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (28 - 16));
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (30 - 16));

    *(uint64_t*)g_Regs.pc = str_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0xaabb, *(uint64_t*)(g_Mem.data.mem + 0x70 + 0x130));
    ASSERT_EQ_UINT(0xcdef, *(uint64_t*)(g_Mem.data.mem + 0x70 + 0x130 + (LEN_WORD * 1)));
    ASSERT_EQ_UINT(0x8498dd, *(uint64_t*)(g_Mem.data.mem + 0x70 + 0x130 + (LEN_WORD * 2)));

    // reset
    str_op.u64 = 0;
    str_op.instruction = Inst_Str;

    // [imm] = reg bitmap, page 2 (reg32 ~ reg47)
    g_Regs.r[33] = 0xddaabb;
    g_Regs.r[42] = 0x32cdef;
    g_Regs.r[47] = 0x98dd03;

    g_Regs.r[29] = (uint64_t)(g_Mem.data.mem + 0x80);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_IMM_REG;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_2;
    str_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 29;
    str_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x90;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (33 - 32));
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (42 - 32));
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (47 - 32));

    *(uint64_t*)g_Regs.pc = str_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0xddaabb, *(uint64_t*)(g_Mem.data.mem + 0x80 + 0x90));
    ASSERT_EQ_UINT(0x32cdef, *(uint64_t*)(g_Mem.data.mem + 0x80 + 0x90 + (LEN_WORD * 1)));
    ASSERT_EQ_UINT(0x98dd03, *(uint64_t*)(g_Mem.data.mem + 0x80 + 0x90 + (LEN_WORD * 2)));

    // reset
    str_op.u64 = 0;
    str_op.instruction = Inst_Str;

    // [imm] = reg bitmap, page 3 (reg48 ~ reg63)
    g_Regs.r[48] = 0x32ddaa;
    g_Regs.r[50] = 0xcd93ef;
    g_Regs.r[60] = 0x98cd03;

    g_Regs.r[45] = (uint64_t)(g_Mem.data.mem + 0x10);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_IMM_REG;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_3;
    str_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 45;
    str_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x490;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (48 - 48));
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (50 - 48));
    str_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (60 - 48));

    *(uint64_t*)g_Regs.pc = str_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x32ddaa, *(uint64_t*)(g_Mem.data.mem + 0x10 + 0x490));
    ASSERT_EQ_UINT(0xcd93ef, *(uint64_t*)(g_Mem.data.mem + 0x10 + 0x490 + (LEN_WORD * 1)));
    ASSERT_EQ_UINT(0x98cd03, *(uint64_t*)(g_Mem.data.mem + 0x10 + 0x490 + (LEN_WORD * 2)));

    // reset
    str_op.u64 = 0;
    str_op.instruction = Inst_Str;

    // [reg] = imm
    g_Regs.r[29] = (uint64_t)(g_Mem.data.mem + 0x410);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_REG_IMM;
    str_op.bytes.memory_type.param.reg_imm.reg_id = 29;
    str_op.bytes.memory_type.param.reg_imm.imm_val = 0xc0ffee;

    *(uint64_t*)g_Regs.pc = str_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0xc0ffee, *(uint64_t*)(g_Mem.data.mem + 0x410));

    // reset
    str_op.u64 = 0;
    str_op.instruction = Inst_Str;

    // [imm] = imm
    g_Regs.r[4] = (uint64_t)(g_Mem.data.mem + 0x400);

    str_op.bytes.memory_type.param_type = MEMORY_TYPE_STR_IMM_IMM;
    str_op.bytes.memory_type.param.imm_imm.base_reg_id = 4;
    str_op.bytes.memory_type.param.imm_imm.imm_addr = 0x24;
    str_op.bytes.memory_type.param.imm_imm.imm_val = 0x0516;

    *(uint64_t*)g_Regs.pc = str_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x0516, *(uint64_t*)(g_Mem.data.mem + 0x424));
}

TESTCASE(6, "ldr test")
{
    Opcode_u_t ldr_op = {.instruction = Inst_Ldr};

    // reg .... reg = [reg...], page 0 (reg0 ~ reg31)
    g_Regs.r[41] = (uint64_t)(g_Mem.data.mem + 0x30);       // This address are used by the privious STR unittest

    ldr_op.bytes.memory_type.param_type = MEMORY_TYPE_LDR_REG_REG;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_id = 41;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap_page_1b = REG_PAGE_32_0;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap = 0;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 3);
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 17);
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 23);
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 28);
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << 30);

    *(uint64_t*)g_Regs.pc = ldr_op.u64;

    uint64_t op_bin = vm_fetch();
    Opcode_u_t opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x3322, g_Regs.r[3]);
    ASSERT_EQ_UINT(0x4433, g_Regs.r[17]);
    ASSERT_EQ_UINT(0x5577, g_Regs.r[23]);
    ASSERT_EQ_UINT(0x66ee, g_Regs.r[28]);
    ASSERT_EQ_UINT(0x33dd, g_Regs.r[30]);

    // reset
    ldr_op.u64 = 0;
    ldr_op.instruction = Inst_Ldr;

    // reg .... reg = [reg...], page 1 (reg32 ~ reg63)
    g_Regs.r[18] = (uint64_t)(g_Mem.data.mem + 0x8);       // This address are used by the privious STR unittest

    ldr_op.bytes.memory_type.param_type = MEMORY_TYPE_LDR_REG_REG;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_id = 18;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap_page_1b = REG_PAGE_32_1;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap = 0;
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (32 - 32));
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (40 - 32));
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (45 - 32));
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (49 - 32));
    ldr_op.bytes.memory_type.param.reg_reg_bmp.reg_bitmap |= (1 << (58 - 32));

    *(uint64_t*)g_Regs.pc = ldr_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x55, g_Regs.r[32]);
    ASSERT_EQ_UINT(0x99, g_Regs.r[40]);
    ASSERT_EQ_UINT(0xdd, g_Regs.r[45]);
    ASSERT_EQ_UINT(0x16, g_Regs.r[49]);
    ASSERT_EQ_UINT(0x1e, g_Regs.r[58]);

    // reset
    ldr_op.u64 = 0;
    ldr_op.instruction = Inst_Ldr;

    // reg .... reg = [imm...], page 0 (reg0 ~ reg15)
    g_Regs.r[45] = (uint64_t)(g_Mem.data.mem + 0x10);       // This address are used by the privious STR unittest

    ldr_op.bytes.memory_type.param_type = MEMORY_TYPE_LDR_REG_IMM;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_0;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 45;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x490;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << 6);
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << 10);
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << 14);

    *(uint64_t*)g_Regs.pc = ldr_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x32ddaa, g_Regs.r[6]);
    ASSERT_EQ_UINT(0xcd93ef, g_Regs.r[10]);
    ASSERT_EQ_UINT(0x98cd03, g_Regs.r[14]);

    // reset
    ldr_op.u64 = 0;
    ldr_op.instruction = Inst_Ldr;

    // reg .... reg = [imm...], page 1 (reg16 ~ reg31)
    g_Regs.r[37] = (uint64_t)(g_Mem.data.mem + 0x60);       // This address are used by the privious STR unittest

    ldr_op.bytes.memory_type.param_type = MEMORY_TYPE_LDR_REG_IMM;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_1;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 37;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x230;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (25 - 16));
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (19 - 16));
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (23 - 16));

    *(uint64_t*)g_Regs.pc = ldr_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0x55aa, g_Regs.r[19]);
    ASSERT_EQ_UINT(0x99cd, g_Regs.r[23]);
    ASSERT_EQ_UINT(0xdd8498, g_Regs.r[25]);

    // reset
    ldr_op.u64 = 0;
    ldr_op.instruction = Inst_Ldr;

    // reg .... reg = [imm...], page 2 (reg32 ~ reg47)
    g_Regs.r[27] = (uint64_t)(g_Mem.data.mem + 0x70);       // This address are used by the privious STR unittest

    ldr_op.bytes.memory_type.param_type = MEMORY_TYPE_LDR_REG_IMM;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_2;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 27;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x130;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (37 - 32));
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (33 - 32));
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (40 - 32));

    *(uint64_t*)g_Regs.pc = ldr_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0xaabb, g_Regs.r[33]);
    ASSERT_EQ_UINT(0xcdef, g_Regs.r[37]);
    ASSERT_EQ_UINT(0x8498dd, g_Regs.r[40]);

    // reset
    ldr_op.u64 = 0;
    ldr_op.instruction = Inst_Ldr;

    // reg .... reg = [imm...], page 4 (reg48 ~ reg63)
    g_Regs.r[29] = (uint64_t)(g_Mem.data.mem + 0x80);       // This address are used by the privious STR unittest

    ldr_op.bytes.memory_type.param_type = MEMORY_TYPE_LDR_REG_IMM;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap_page_2b = REG_PAGE_16_3;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.base_reg_id = 29;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.imm_addr = 0x90;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap = 0;
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (53 - 48));
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (62 - 48));
    ldr_op.bytes.memory_type.param.imm_reg_bmp.reg_bitmap |= (1 << (57 - 48));

    *(uint64_t*)g_Regs.pc = ldr_op.u64;

    op_bin = vm_fetch();
    opcode = vm_decode(op_bin);
    vm_execute(opcode);

    ASSERT_EQ_UINT(0xddaabb, g_Regs.r[53]);
    ASSERT_EQ_UINT(0x32cdef, g_Regs.r[57]);
    ASSERT_EQ_UINT(0x98dd03, g_Regs.r[62]);
}

TESTCASE(7, "jmp test")
{
    Opcode_u_t jmp_op = {.instruction = Inst_Jmp};
    uint64_t prior_pc = 0;
    uint64_t op_bin = 0;
    Opcode_u_t opcode = {0};

    // Register based PC relative
    jmp_op.bytes.jump_type.param_type = JUMP_TYPE_REG_BASED_PC_REL_F;
        // forward
            // Cond_None = 0,
            prior_pc = g_Regs.pc;
            jmp_op.bytes.jump_type.condition = (uint8_t)Cond_None;
            g_Regs.r[22] = 0x7;
            jmp_op.bytes.jump_type.param.reg_based.id = 22;

            *(uint64_t*)g_Regs.pc = jmp_op.u64;

            op_bin = vm_fetch();
            opcode = vm_decode(op_bin);
            vm_execute(opcode);

            ASSERT_EQ_UINT((prior_pc + LEN_WORD * 0x7), g_Regs.pc);
            
            // Cond_Eq   = 1,
            // Cond_Neq  = 2,
            // Cond_gt   = 3,
            // Cond_lt   = 4,
            // Cond_get  = 5,
            // Cond_let  = 6,

        // backward

    jmp_op.u64 = 0;
    jmp_op.instruction = Inst_Jmp;

    // Immediated PC relative
        // forward
        // backward
    
    jmp_op.u64 = 0;
    jmp_op.instruction = Inst_Jmp;

    // Register based
        // forward
        // backward

    jmp_op.u64 = 0;
    jmp_op.instruction = Inst_Jmp;

    // Immediated
        // forward
        // backward
}

TESTCASE(300, "Opcode size")
{
    ASSERT_EQ_UINT(8, sizeof(Opcode_u_t));
}