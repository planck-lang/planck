#include "TestMain.h"

TestUnit_t testUnit = {0};


static void InitTestUnit(void);void SortTestSuite(void);
static void RunTest(void);
static void ReportTestResult(void);
void SortTestSuite(void);

static uint32_t BeautiPrint_GetLongestNameLen(TestCase_t* testCasePtr);
static void BeautiPrint_GetCurrentPadLen(char* name, int maxLen);

int main(int argc, char* argv[])
{
    InitTestUnit();
    SortTestSuite();
    RunTest();
    ReportTestResult();

    return 0;
}

void AddTestSuite(TestSuite_t* testSuite)
{
    // Add to head
    testSuite->next = testUnit.testSuitsList;
    testUnit.testSuitsList = testSuite;
}

/*
Swap only data; It should keep the next link for each of params
 */
static void swap(TestSuite_t* a, TestSuite_t* b) 
{ 
    TestSuite_t* temp_a_next = a->next; 
    TestSuite_t* temp_b_next = b->next;

    TestSuite_t temp = {0};
    temp = *a;
    *a = *b;
    *b = temp;

    a->next = temp_a_next;
    b->next = temp_b_next;
} 

void SortTestSuite(void)
{
    bool swapped; 
    TestSuite_t* ptr1; 
    TestSuite_t* lptr = NULL; 
  
    /* Checking for empty list */
    if (testUnit.testSuitsList->next == NULL) 
        return; 

    do
    { 
        swapped = false; 
        ptr1 = testUnit.testSuitsList; 
  
        while (ptr1->next != lptr) 
        { 
            if (strncmp(ptr1->name, ptr1->next->name, 2) > 0) 
            {  
                swap(ptr1, ptr1->next); 
                swapped = true; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
}

TestCase_t* MakeTestCase(TestCaseFunc_t testFunc, char* name)
{
    TestCase_t* testCase;
    testCase = (TestCase_t*)malloc(sizeof(TestCase_t));
    memset(testCase, 0, sizeof(TestCase_t));

    testCase->name = (char*)malloc(strlen(name) + 1);
    memset(testCase->name, 0, strlen(name) + 1);
    strncpy(testCase->name, name, strlen(name));

    testCase->testFunc = testFunc;

    return testCase;
}

TestCase_t* AddTestCaseObj(TestSuite_t* testSuite, TestCase_t* testCase)
{
	testSuite->totalTestCaseNum++;

    // Add to tail
    if(testSuite->testCaseList == NULL)
    {
        testSuite->testCaseList = testCase;
        testCase->next = NULL;
        return testCase;
    }

    TestCase_t* testCasePtr;
    for(testCasePtr = testSuite->testCaseList ; testCasePtr->next != NULL ; testCasePtr = testCasePtr->next);
    testCasePtr->next = testCase;
    testCase->next = NULL;

    return testCase;
}

TestCase_t* AddTestCase(TestSuite_t* testSuite, TestCaseFunc_t testFunc, char* name)
{
    TestCase_t* testCase = MakeTestCase(testFunc, name);
    return AddTestCaseObj(testSuite, testCase);
}

static void InitTestUnit(void)
{
    TestSuite_t* testSuitePtr;
    int testSuiteCount = 0;

    for(testSuitePtr = testUnit.testSuitsList; testSuitePtr != NULL; testSuitePtr = testSuitePtr->next)
    {
        if(testSuitePtr->initFunc != NULL)
        {
            printf("====== Initialize Test Suite.. %d\n", ++testSuiteCount);
            testSuitePtr->initFunc(testSuitePtr);
            printf("Test suite name: %s\n", testSuitePtr->name);
            printf("This test suite has %d test cases. Initialize done.\n", testSuitePtr->totalTestCaseNum);
        }
    }
}

static void RunTest(void)
{
    TestSuite_t* testSuitePtr;
    for(testSuitePtr = testUnit.testSuitsList; testSuitePtr != NULL; testSuitePtr = testSuitePtr->next)
    {
        uint32_t caseCount = 0;
        bool anyFail = false;
        TestCase_t* testCasePtr;

        testUnit.totalSuites++;

        printf("\n==== Test suite [%s] Start ====\n", testSuitePtr->name);
        if(testSuitePtr->preTestFunc != NULL)
        {
            printf("===== PreTest Start =====\n");
            testSuitePtr->preTestFunc();
            printf("===== PreTest Done =====\n");
        }

        uint32_t beautiprintMaxLen = BeautiPrint_GetLongestNameLen(testSuitePtr->testCaseList);

        for(testCasePtr = testSuitePtr->testCaseList ; testCasePtr != NULL ; testCasePtr = testCasePtr->next)
        {
            printf(TESTCASEINDENT "Case %03d [%s] ", (caseCount++) + 1, testCasePtr->name);
            BeautiPrint_GetCurrentPadLen(testCasePtr->name, beautiprintMaxLen);

            if(testCasePtr->testFunc != NULL)
            {
                testUnit.totalCases++;

                testCasePtr->result = true;
                testCasePtr->testFunc(testCasePtr);
                if(testCasePtr->result)
                {
                    printf(" PASS\n");
                    testUnit.totalPass++;
                }
                else
                {
                    printf(TESTCASEINDENT FRED BYELLOW"FAIL"NONE"\n");
                    anyFail = true;
                    testUnit.totalFail++;
                }
            }
        }

        if(testSuitePtr->postTestFunc != NULL)
        {
            printf("===== PostTest Start =====\n");
            testSuitePtr->postTestFunc();
            printf("===== PostTest Done =====\n");
        }
        printf("==== Test suite [%s] %s ====\n", testSuitePtr->name, (anyFail?"Fail":"Pass!"));
    }
}

static void ReportTestResult(void)
{
	printf("\n------------------------------\n");
	if(testUnit.totalFail)
	{
		printf(FYELLOW BRED);
	}
	else
	{
		printf(FYELLOW BGREEN);
	}
	printf("Report"NONE"\n");
	printf("------------------------------\n");
	printf("Total.....................%04d\n", testUnit.totalCases);
	printf("Pass......................%04d\n", testUnit.totalPass);
	printf("Fail......................%04d\n", testUnit.totalFail);
}

static uint32_t BeautiPrint_GetLongestNameLen(TestCase_t* testCasePtr)
{
	uint32_t maxLen = 0;
	for(; testCasePtr != NULL ; testCasePtr = testCasePtr->next)
	{
		uint32_t nameLen = strlen(testCasePtr->name);
		maxLen = (nameLen > maxLen) ? nameLen : maxLen;
	}

	return (maxLen + 3);
}

static void BeautiPrint_GetCurrentPadLen(char* name, int maxLen)
{
	uint32_t padLen = maxLen - strlen(name);

	while(padLen--)
	{
		putchar('.');
	}
	fflush(stdout);
}

