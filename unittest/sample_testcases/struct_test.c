#include "TestMain.h"

#include "planck.h"

#include "typetab.h"

REGISTER_SUITE_AUTO(Struct_Test, "Struct Test")

TESTCASE(1, "basic test")
{
    typetab_init();

    char* codeline = "struct basic_t {\n" 
                     "  let a as int\n" 
                     "  let b as int\n" 
                     "  let c as num\n"
                     "}";
    
    data_t ret = {0};
    error_code_e error = planck(codeline, &ret);

    ASSERT_EQ_INT(error_vm_stack_empty, error);
}