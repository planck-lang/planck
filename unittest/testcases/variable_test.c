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

    ASSERT_EQ_INT(7, ret.val);
    ASSERT_EQ_INT(error_vm_stack_empty, error);
}

TESTCASE(2, "load test")
{
    char* codeline = "let a = 3 + 4";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ret = symtab_get_value_from_symbol_idx(1);

    ASSERT_EQ_INT(7, ret.val);
    ASSERT_EQ_INT(error_vm_stack_empty, error);

    codeline = "4 + a";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(11, ret.val);

    codeline = "4 - a";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(-3, ret.val);

    codeline = "-a * 3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(-21, ret.val);
}

TESTCASE(3, "find symbol index")
{
    char* codeline = "let b = 3 + 4";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_vm_stack_empty, error);

    uint32_t idx = symtab_get_idx_by_name("b");
    ASSERT_EQ_UINT(idx, 2);

    idx = symtab_get_idx_by_name("be");
    ASSERT_EQ_UINT(idx, SYMTAB_NO_IDX);
}

TESTCASE(4, "overwrite")
{
    char* codeline = "a";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(7, ret.val);

    codeline = "a = 23 + 99";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_vm_stack_empty, error);

    codeline = "a";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT((23+99), ret.val);
}
