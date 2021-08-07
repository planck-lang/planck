#include "TestMain.h"

#include "vm.h"

extern Mem_s_t g_Mem;
extern Reg_s_t g_Regs;

REGISTER_SUITE_AUTO(vm_test, "01_vm_tests")

TESTCASE(1, "push test")
{
    vm_init();

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
    ASSERT_EQ_UINT(0x4bd0007, push_op.u64);

    *(uint64_t*)g_Regs.pc = push_op.u64;

    uint64_t op_bin = vm_fetch();

    ASSERT_EQ_UINT(0x4bd0007, op_bin);
}