
#include "os_common.h"
#include "unity.h"
// DEFINES
#define SIZE_CB_BUFF   10
#define SIZE_TEST_BUFF 15
#define SIZE_TEST_STEP 3
// VARIABLES
osCirBuf test_cb;
uint8_t test_cb_buff[SIZE_CB_BUFF] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void setUp(void)
{
    /* This is run before EACH TEST */
    // Counter = 0x5a5a;
}

void tearDown(void)
{
    memset(&test_cb, 0x00, sizeof(test_cb));
}

void test_IgnoredTest(void)
{
    TEST_IGNORE_MESSAGE("This Test Was Ignored On Purpose");
}

void test_AnotherIgnoredTest(void)
{
    TEST_IGNORE_MESSAGE("These Can Be Useful For Leaving Yourself Notes On "
                        "What You Need To Do Yet");
}

void test_ThisFunctionHasNotBeenTested_NeedsToBeImplemented(void)
{
    TEST_IGNORE(); /* Like This */
}

void test_SizeZeroError_osCirBufCreate(void)
{
    TEST_ASSERT_EQUAL(-1, osCirBufCreate(&test_cb, test_cb_buff, 0));
}

void test_CBNullError_osCirBufCreate(void)
{
    TEST_ASSERT_EQUAL(-1, osCirBufCreate(NULL, test_cb_buff, SIZE_CB_BUFF));
}

void test_BufferNullError_osCirBufCreate(void)
{
    TEST_ASSERT_EQUAL(-1, osCirBufCreate(&test_cb, NULL, SIZE_CB_BUFF));
}

void test_isWorking_osCirBufCreate(void)
{
    TEST_ASSERT_EQUAL(1, osCirBufCreate(&test_cb, test_cb_buff, SIZE_CB_BUFF));
}

void test_CheckInitVariables_osCirBufCreate(void)
{
    osCirBufCreate(&test_cb, test_cb_buff, SIZE_CB_BUFF);
    TEST_ASSERT_EQUAL_PTR(test_cb_buff, test_cb.ptr);
    TEST_ASSERT_EQUAL_PTR(test_cb_buff, test_cb.head);
    TEST_ASSERT_EQUAL_PTR(test_cb_buff, test_cb.tail);
    // TEST_ASSERT_EQUAL_PTR (&test_cb_buff[SIZE_CB_BUFF - 1] , test_cb.last);
    TEST_ASSERT_EACH_EQUAL_UINT8(0, test_cb.ptr, test_cb.bufsize);
    TEST_ASSERT_EQUAL(0, test_cb.used);
    TEST_ASSERT_EQUAL(SIZE_CB_BUFF, test_cb.bufsize);
}

void test_PointerNullError_osCirBufClear(void)
{
    TEST_ASSERT_EQUAL(-1, osCirBufClear(NULL));
}

void test_BufferNullError_osCirBufClear(void)
{
    TEST_ASSERT_EQUAL(-1, osCirBufClear(&test_cb));
}

void test_BufferSizeZeroError_osCirBufClear(void)
{
    test_cb.ptr = test_cb_buff;
    TEST_ASSERT_EQUAL(-1, osCirBufClear(&test_cb));
}
