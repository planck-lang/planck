#include "TestMain.h"

#include "planck.h"

REGISTER_SUITE_AUTO(Basic_Op_Test, "Basic Operation Test")

TESTCASE(1, "simple add")
{
    char* codeline = "3 + 4";

    data_t ret = planck(codeline);

    ASSERT_EQ_INT(ret.val.ival, 7);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");
}

TESTCASE(2, "simple mixed")
{
    char* codeline;

    data_t ret;

    codeline = "3 + 4 * 3";
    ret = planck(codeline);
    ASSERT_EQ_INT(3 + 4 * 3, ret.val.ival);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "3 - 4 * 3";
    ret = planck(codeline);
    ASSERT_EQ_INT(3 - 4 * 3, ret.val.ival);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "3 - 4 / 3";
    ret = planck(codeline);
    ASSERT_EQ_INT(3 - 4 / 3, ret.val.ival);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");
}

TESTCASE(3, "no space expr")
{
    char* codeline;

    data_t ret;

    codeline = "3+4*3";
    ret = planck(codeline);
    ASSERT_EQ_INT(3+4*3, ret.val.ival);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "3-4*3";
    ret = planck(codeline);
    ASSERT_EQ_INT(3-4*3, ret.val.ival);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "3.3-4.4/3.3";
    ret = planck(codeline);
    ASSERT_EQ_NUM(3.3-4.4/3.3, ret.val.dval);
    ASSERT(ret.valtype == valtype_double, "valtype should be double");

    codeline = "-3+4*3";
    ret = planck(codeline);
    ASSERT_EQ_INT(-3+4*3, ret.val.ival);

    codeline = "-3+2";
    ret = planck(codeline);
    ASSERT_EQ_INT(-3+2, ret.val.ival);
}

TESTCASE(4, "bracket")
{
    char* codeline;

    data_t ret;

    codeline = "(3+4)*3";
    ret = planck(codeline);
    ASSERT_EQ_INT((3+4)*3, ret.val.ival);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "(3-4)*3";
    ret = planck(codeline);
    ASSERT_EQ_INT((3-4)*3, ret.val.ival);
    ASSERT(ret.valtype == valtype_int, "valtype should be int");

    codeline = "(3.3-4.4)/3.3";
    ret = planck(codeline);
    ASSERT_EQ_NUM((3.3-4.4)/3.3, ret.val.dval);
    ASSERT(ret.valtype == valtype_double, "valtype should be double");
}