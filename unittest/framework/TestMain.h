#ifndef _TEST_MAIN_
#define _TEST_MAIN_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
 
#define PRE_TEST_FUNC()         \
    static void preTest(void);  \
    __attribute__ ((__constructor__)) static void regPreTest ## suiteName(void) \
    {suite.preTestFunc = preTest;} \
    static void preTest(void)

#define POST_TEST_FUNC()        \
    static void postTest(void); \
    __attribute__ ((__constructor__)) static void regPostTest ## suiteName(void) \
    {suite.postTestFunc = postTest;} \
    static void postTest(void)

#define TESTCASE(idx, desc) \
    static void testauto##idx(TestCase_t* tc); \
    __attribute__ ((__constructor__(10 ## idx))) static void regTest ## idx(void) \
    {AddTestCase(&suite, testauto##idx, desc);} \
    static void testauto##idx(TestCase_t* tc)

#define REGISTER_SUITE_AUTO(suiteName, suiteDesc) \
    static TestSuite_t suite = {0};\
    static char* suiteNameStr = suiteDesc;\
    static void Init(TestSuite_t* suite) {printf("Auto Initialization\n");} \
    __attribute__ ((__constructor__)) static void RegisterSuite ## suiteName(void) \
    { suite.name = suiteNameStr; suite.initFunc = Init; AddTestSuite(&suite);}

#define TESTCASEINDENT  "  "
#define TESTCASEMSGINDENT "    "

#define ASSERTMSG_NUM_EQ_FAIL(expect, actual)  "FAIL! expect %f actual %f (at Line %d)\n", (double)expect, (double)actual, __LINE__
#define ASSERTMSG_NUM_NEQ_FAIL(expect, actual)  "FAIL! expect %f should NOT same as %f (at Line %d)\n", (double)expect, (double)actual, __LINE__
#define ASSERTMSG_UINT_EQ_FAIL(expect, actual)  "FAIL! expect 0x%lx actual 0x%lx (at Line %d)\n", (uint64_t)expect, (uint64_t)actual, __LINE__
#define ASSERTMSG_UINT_NEQ_FAIL(expect, actual)  "FAIL! expect 0x%lx should NOT same as 0x%lx (at Line %d)\n", (uint64_t)expect, (uint64_t)actual, __LINE__
#define ASSERTMSG_INT_EQ_FAIL(expect, actual)  "FAIL! expect 0x%lx actual 0x%lx (at Line %d)\n", (int64_t)expect, (int64_t)actual, __LINE__
#define ASSERTMSG_INT_NEQ_FAIL(expect, actual)  "FAIL! expect 0x%lx should NOT same as 0x%lx (at Line %d)\n", (int64_t)expect, (int64_t)actual, __LINE__
#define ASSERTMSG_STR_EQ_FAIL(expect, actual)  "FAIL! expect [\"%s\"] actual [\"%s\"] (at Line %d)\n", expect, actual, __LINE__
#define ASSERTMSG_STR_NEQ_FAIL(expect, actual)  "FAIL! expect [\"%s\"] should NOT same as [\"%s\"] (at Line %d)\n", expect, actual, __LINE__

#define ASSERT_CMPSTR(expect, actual) ((strncmp(expect, actual, strlen(actual)) == 0) && (strlen(expect) == strlen(actual)))
#define ASSERT(cond, msgfmt) if(!(cond)){printf("\n" TESTCASEMSGINDENT msgfmt); tc->result = false; return;} else {tc->result = true;}

#define ASSERT_EQ_NUM(expect, actual) ASSERT(((double)expect == (double)actual), ASSERTMSG_NUM_EQ_FAIL(expect, actual))
#define ASEERT_NEQ_NUM(expect, actual) ASSERT(((double)expect != (double)actual), ASSERTMSG_NUM_NEQ_FAIL(expect, actual))
#define ASSERT_EQ_UINT(expect, actual) ASSERT(((uint64_t)expect == (uint64_t)actual), ASSERTMSG_UINT_EQ_FAIL(expect, actual))
#define ASEERT_NEQ_UINT(expect, actual) ASSERT(((uint64_t)expect != (uint64_t)actual), ASSERTMSG_UINT_NEQ_FAIL(expect, actual))
#define ASSERT_EQ_INT(expect, actual) ASSERT(((int64_t)expect == (int64_t)actual), ASSERTMSG_INT_EQ_FAIL(expect, actual))
#define ASEERT_NEQ_INT(expect, actual) ASSERT(((int64_t)expect != (int64_t)actual), ASSERTMSG_INT_NEQ_FAIL(expect, actual))
#define ASSERT_EQ_STR(expect, actual) ASSERT(ASSERT_CMPSTR(expect,actual), ASSERTMSG_STR_EQ_FAIL(expect, actual))
#define ASEERT_NEQ_STR(expect, actual) ASSERT((strncmp(expect, actual, strlen(actual)) != 0), ASSERTMSG_STR_NEQ_FAIL(expect, actual))

struct _test_suite_t_;
struct _test_case_t_;
typedef void (*InitSuite_t)(struct _test_suite_t_ *);
typedef void (*TestCaseFunc_t)(struct _test_case_t_ *);
typedef void (*PrePostFunc_t)(void);

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
    PrePostFunc_t   preTestFunc;
    PrePostFunc_t   postTestFunc;
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

