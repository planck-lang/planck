#ifndef _TEST_MAIN_
#define _TEST_MAIN_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
 
#define REGISTER_SUITE_FUNC(suiteName, init_func) \
    __attribute__ ((__constructor__)) \
    void RegisterSuite ## suiteName(void) \
    { TestSuite_t* suite = (TestSuite_t*)malloc(sizeof(TestSuite_t)); memset(suite, 0, sizeof(TestSuite_t)); suite->initFunc = init_func; AddTestSuite(suite); }

#define PRE_TEST_BEGIN() static bool preTest(void){
#define PRE_TEST_END return true;} \
    __attribute__ ((__constructor__)) static void regPreTest ## suiteName(void) \
    {suite.preTestFunc = preTest;}

#define POST_TEST_BEGIN() static bool postTest(void){
#define POST_TEST_END return true;} \
    __attribute__ ((__constructor__)) static void regPostTest ## suiteName(void) \
    {suite.postTestFunc = postTest;}

#define TESTCASE(idx, desc) \
    static bool testauto##idx(void); \
    __attribute__ ((__constructor__)) static void regTest ## idx(void) \
    {AddTestCase(&suite, testauto##idx, desc);} \
    static bool testauto##idx(void)

#define REGISTER_SUITE_AUTO(suiteName, suiteDesc) \
    static TestSuite_t suite = {0};\
    static char* suiteNameStr = suiteDesc;\
    static void Init(TestSuite_t* suite) {printf("Auto Initialization\n");} \
    __attribute__ ((__constructor__)) static void RegisterSuite ## suiteName(void) \
    { suite.name = suiteNameStr; suite.initFunc = Init; AddTestSuite(&suite);}

#define TESTCASEINDENT  "  "
#define TESTCASEMSGINDENT "    "

#define ASSERTMSG_INT_FAIL(expect, actual)  "FAIL! expect 0x%lx actual 0x%lx\n", (uint64_t)expect, (uint64_t)actual
#define ASSERTMSG_STR_FAIL(expect, actual)  "FAIL! expect [\"%s\"] actual [\"%s\"]\n", expect, actual

#define ASSERT_CMPSTR(expect, actual) ((strncmp(expect, actual, strlen(actual)) == 0) && (strlen(expect) == strlen(actual)))

#define ASSERT(cond, msgfmt) if(!(cond)){printf("\n" TESTCASEMSGINDENT msgfmt);} return (cond);

struct _test_suite_t_;
typedef void (*InitSuite_t)(struct _test_suite_t_ *);
typedef bool (*TestCaseFunc_t)(void);

typedef struct _test_case_t_
{
    char*           name;
    TestCaseFunc_t  testFunc;
    bool            result;
    struct _test_case_t_ *next;
} TestCase_t;

typedef struct _test_suite_t_
{
    InitSuite_t     initFunc;
    TestCaseFunc_t  preTestFunc;
    TestCaseFunc_t  postTestFunc;
    TestCase_t*     testCaseList;
    char*           name;
    uint32_t        totalTestCaseNum;
    struct _test_suite_t_ *next;
} TestSuite_t;

typedef struct _test_unit_t_
{
    TestSuite_t* testSuitsList;
    uint32_t     totalSuites;
    uint32_t     totalCases;
    uint32_t     totalPass;
    uint32_t     totalFail;
} TestUnit_t;

void AddTestSuite(TestSuite_t* testSuite);
TestCase_t* MakeTestCase(TestCaseFunc_t testFunc, char* name);
TestCase_t* AddTestCaseObj(TestSuite_t* testSuite, TestCase_t* testCase);
TestCase_t* AddTestCase(TestSuite_t* testSuite, TestCaseFunc_t testFunc, char* name);

//fonts color
#define FBLACK      "\033[30;"
#define FRED        "\033[31;"
#define FGREEN      "\033[32;"
#define FYELLOW     "\033[33;"
#define FBLUE       "\033[34;"
#define FPURPLE     "\033[35;"
#define D_FGREEN    "\033[6;"
#define FWHITE      "\033[7;"
#define FCYAN       "\x1b[36m"

//background color
#define BBLACK      "40m"
#define BRED        "41m"
#define BGREEN      "42m"
#define BYELLOW     "43m"
#define BBLUE       "44m"
#define BPURPLE     "45m"
#define D_BGREEN    "46m"
#define BWHITE      "47m"

//end color
#define NONE        "\033[0m"

#endif

