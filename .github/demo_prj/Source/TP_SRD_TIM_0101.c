/*
 * TP_SRD_TIM_0101.c - High-level test procedure for Time Management
 *
 * Copyright(c) 2009 MDS Technology Co.,Ltd.
 * All rights reserved.
 *
 * This software contains confidential information of MDS Technology Co.,Ltd.
 * and unauthorized distribution of this software, or any portion of it, are
 * prohibited.
 */

#include <TFX/TFX.h>
#include <NEOS178S.h>

#define     TIMER_NAME              "Timer_TIM101"
#define     FLAG_SET                1
#define     FLAG_CLEAR              0
#define     MAX_ITERATION           10

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0101_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global variables */
static Timer     myTimer;
static TimeValue myTimeValue[2];
static int       flag;

/**
 * TestCleanup - cleaning test
 *
 * DESCRIPTION
 *  Delete any test threads and other test artifacts
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  N/A
 *
 * RETURNS
 *  N/A
 *
 * ERRORS
 *  N/A
 */

static void 
TestCleanUp(void)
{
    TimerStop(myTimer);
}

/**
 * MyTimer_h - handler fucntion
 *
 * DESCRIPTION
 *  This is callback function for myTimer 
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  N/A
 *
 * RETURNS
 *  N/A
 *
 * ERRORS
 *  N/A
 */

static void
MyTimer_h(Address args)
{
    /* Get the system time */
    myTimeValue[1] = SystemClockGetTime();

    /* If get the system time, flag set */ 
    if (myTimeValue[1] > myTimeValue[0])   
    {
        flag = FLAG_SET;
    }   
}

/**
 * TestSetPrecondition - setting precondition
 *
 * DESCRIPTION
 *   Initialize environment for this test case before TestDo() function 
 *   is executed. 
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  N/A
 *
 * RETURNS
 *  This routine returns SUCCESS if the test is passed successfully,
 *  or it returns ERROR to indicate the test error.
 *
 * ERRORS
 *  N/A
 */

static int
TestSetPrecondition(void)
{
    int result;
    /* 
     * Create a software timer with valid name, valid software timer service 
     * and in periodical mode.
     */
    if ((result = TimerCreate(TIMER_NAME, TIMER_PERIODIC, MyTimer_h, 
                        NULL, &myTimer)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    return (SUCCESS);
}

/**
 * TestDo - preforming test
 *
 * DESCRIPTION
 *  Test code to exercise if NEOS-178S starts a software timer in periodical mode with 
 *  timeout value and registers user-defined function for software timer service.
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0101 -> TC_SRD_TIM_0101]
 *
 * RETURNS
 *  This routine returns SUCCESS if the test is passed successfully,
 *  or it returns ERROR to indicate the test error.
 *
 * ERRORS
 *  N/A
 */

static int
TestDo(void)
{
    int timeDifference;
    int result;
    int count;
    
    /* Clear FLAG */
    flag = FLAG_CLEAR;

    /* Delay execution of this thread for 0.1 seconds */
    if ((result = ThreadDelay(100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Get the system time elapsed since system is powered ON.  */
    myTimeValue[0] = SystemClockGetTime();

    /* Check the system time */ 
    if (myTimeValue[0] == 0)   
    {
        TEST_FAILURE (FAILURE);
    }   

    /* start myTimer after 0.1 sec */
    if ((result = TimerStart(myTimer, 100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* wait for calling callback function */
    if ((result = ThreadDelay(100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Verify if the variable is incremented periodically. */
    for (count = 0; count < MAX_ITERATION; count++)
    {
        /* check if flag is changed by myTimer_h() */
        if (flag != FLAG_SET)
        {
            TEST_FAILURE (FAILURE);
        } 
    
        timeDifference = myTimeValue[1] - myTimeValue[0];
    
        /* 
         * check time differnece if the time difference is same as the timeout 
         * duration (approximately 0.1 second)
         */
        if ((timeDifference > (100*TEST_MSEC + TFX_TIME_MARGIN)) ||
            (timeDifference < (100*TEST_MSEC - TFX_TIME_MARGIN)))
        {
            TEST_FAILURE (FAILURE);
        } 
    
        /* Clear FLAG */
        flag = FLAG_CLEAR;

        myTimeValue[0] = myTimeValue[1];

        /* wait for calling callback function */
        if ((result = ThreadDelay(100*TEST_MSEC)) != SUCCESS)
        {
            TEST_FAILURE (result);
        }
    }
    
    TEST_SUCCESS (SUCCESS);
}

