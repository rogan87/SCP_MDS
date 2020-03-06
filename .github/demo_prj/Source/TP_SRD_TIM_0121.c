/*
 * TP_SRD_TIM_0121.c - High-level test procedure for Time Management
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

#define     TIMER_NAME      "Timer_TIM121"

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0121_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global variables */
static Timer myTimer;
static int   index = 0;

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
}

/**
 * myTimer_h - handler fucntion
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
    /* increase variable */ 
    index++;
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
     * and in one-shot mode.
     */
    if ((result = TimerCreate(TIMER_NAME, TIMER_ONESHOT, MyTimer_h, NULL, 
                    &myTimer)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    return (SUCCESS);
}

/**
 * TestDo - performing test
 *
 * DESCRIPTION
 *   Test code to exercise if NEOS-178S stops a software timer in one-shot 
 *   mode. 
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0121 -> TC_SRD_TIM_0121]
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
    int result;

    /* start myTimer after 0.1 sec */
    if ((result = TimerStart(myTimer, 100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Stop the software timer */
    if ((result = TimerStop(myTimer)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* wait for calling callback function */
    if ((result = ThreadDelay(100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* check if index is changed by myTimer_h() */
    if (index != 0)
    {
        TEST_FAILURE (FAILURE);
    }
    
    TEST_SUCCESS (SUCCESS);
}

