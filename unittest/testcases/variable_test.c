#include "TestMain.h"

#include "planck.h"

#include "symtab.h"

REGISTER_SUITE_AUTO(Variable_Test, "Variable Test")

TESTCASE(1, "basic test")
{
    char* codeline = "let sample = 3 + 4";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ret = symtab_get_value_from_symbol_idx(0);

    ASSERT_EQ_INT(ret.val.ival, 7);
    ASSERT_EQ_INT(error_vm_stack_empty, error);
}

TESTCASE(2, "load test")
{
    char* codeline = "let a = 3 + 4";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ret = symtab_get_value_from_symbol_idx(0);

    ASSERT_EQ_INT(ret.val.ival, 7);
    ASSERT_EQ_INT(error_vm_stack_empty, error);

    codeline = "4 + a";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(ret.val.ival, 11);

    codeline = "4 - a";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(ret.val.ival, -3);
}