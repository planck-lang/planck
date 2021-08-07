#include "TestMain.h"

#include "planck.h"

REGISTER_SUITE_AUTO(Basic_Op_Test, "Basic Operation Test")

TESTCASE(1, "simple add")
{
    char* codeline = "3 + 4";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(ret.val, 7);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");
}

TESTCASE(2, "simple mixed")
{
    char* codeline;

    data_t ret;
    error_code_e error;

    codeline = "3 + 4 * 3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(3 + 4 * 3, ret.val);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "3 - 4 * 3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(3 - 4 * 3, ret.val);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "3 - 4 / 3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(3 - 4 / 3, ret.val);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");
}

TESTCASE(3, "no space expr")
{
    char* codeline;

    data_t ret;
    error_code_e error;

    codeline = "3+4*3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(3+4*3, ret.val);

    codeline = "3-4*3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(3-4*3, ret.val);

    codeline = "3.3-4.4/3.3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_NUM(3.3-4.4/3.3, ret.val);

    codeline = "-3+4*3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(-3+4*3, ret.val);

    codeline = "-3+2";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT(-3+2, ret.val);

    codeline = "-3.7+2.8";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_NUM(-3.7+2.8, ret.val);
}

TESTCASE(4, "bracket")
{
    char* codeline;

    data_t ret;
    error_code_e error;

    codeline = "(3+4)*3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT((3+4)*3, ret.val);

    codeline = "(3-4)*3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_INT((3-4)*3, ret.val);

    codeline = "(3.3-4.4)/3.3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_NUM((3.3-4.4)/3.3, ret.val);

    codeline = "-(3.3+4.4)/3.3";
    error = planck(codeline, &ret);
    ASSERT_EQ_INT(error_none, error);
    ASSERT_EQ_NUM(-(3.3+4.4)/3.3, ret.val);
}