#include "TestMain.h"

#include "planck.h"

#include "codegen.h"
#include "vm.h"

REGISTER_SUITE_AUTO(Error_Test, "Error Test")

TESTCASE(1, "no error")
{
    char* codeline = "3 + 4.0";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_UINT(error_none, error);
}

TESTCASE(2, "unknown opcode")
{
    codegen_init();
    
    codegen_add_opcode(opcode_nop);
    codegen_add_opcode(opcode_nop);
    codegen_add_opcode(opcode_nop);
    codegen_add_opcode((opcode_e)328947);

    vm_init(
        codegen_get_objcode(),
        codegen_get_objcode_lines()
    );
    vm_run();

    ASSERT_EQ_UINT(error_vm_unknown_opcode, errors_get());
}

TESTCASE(3, "syntax error")
{
    char* codeline = "dkl gills a * 0;";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_UINT(error_parser_syntax, error);
}