#include "TestMain.h"

#include "planck.h"

#include "codegen.h"

REGISTER_SUITE_AUTO(Error_Test, "Error Test")

TESTCASE(1, "type mismatch error")
{
    char* codeline = "3 + 4.0";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_UINT(error_vm_type_mismatch, error);
}

TESTCASE(2, "unknown opcode")
{
    codegen_add_opcode(opcode_nop);
    codegen_add_opcode(opcode_nop);
    codegen_add_opcode(opcode_nop);
    codegen_add_opcode((opcode_e)328947);

    char* codeline = "3 + 4";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_UINT(error_vm_unknown_opcode, error);
}

TESTCASE(3, "syntax error")
{
    char* codeline = "dkl gills a * 0;";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_UINT(error_parser_syntax, error);
}