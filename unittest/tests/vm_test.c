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
}

TESTCASE(2, "pop test")
{
    Opcode_u_t pop_op = {.instruction = Inst_Pop};

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

    ASSERT_EQ_UINT(g_Mem.stack.mem, g_Regs.sp); // all stack vaules are poped out

    ASSERT_EQ_UINT(0x32423, g_Regs.r[5]);
    ASSERT_EQ_UINT(0x986523, g_Regs.r[10]);
    ASSERT_EQ_UINT(0x88928452, g_Regs.r[15]);
    ASSERT_EQ_UINT(0x882994892004, g_Regs.r[17]);
    ASSERT_EQ_UINT(0x724, g_Regs.r[20]);
    ASSERT_EQ_UINT(0x2324242, g_Regs.r[28]);
    ASSERT_EQ_UINT(0x888234, g_Regs.r[31]);
}

TESTCASE(3, "mov test")
{
    Opcode_u_t mov_op = {.instruction = Inst_Mov};
    
    // Reg = Reg
    mov_op.bytes.simple_type.param_type = 0;

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

    mov_op.bytes.simple_type.param_type = 1;

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