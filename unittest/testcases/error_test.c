#include "TestMain.h"

#include "planck.h"

REGISTER_SUITE_AUTO(Error_Test, "Error Test")

TESTCASE(1, "type mismatch error")
{
    char* codeline = "3 + 4.0";

    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_INT(error_vm_type_mismatch, error);
}