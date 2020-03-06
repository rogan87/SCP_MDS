/*
 * TP_SRD_TIM_0102.c - High-level test procedure for Time Management
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

#define     TIMER_NAME      "Timer_TIM102"

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0102_ID =
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
    return;
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
 *  This routine returns SUCCESS to indicate this function is passed,
 *  or it returns error code or FAILURE to indicate testing failure.
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
    if ((result = TimerCreate(TIMER_NAME, TIMER_PERIODIC, MyTimer_h, NULL, 
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
 *  Test code to exercise if NEOS-178S fails to start a software timer in periodical mode 
 *  with zero timeout.
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0102 -> TC_SRD_TIM_0102]
 *
 * RETURNS
 *  This routine returns SUCCESS if the test is passed successfully,
 *  or it returns ERROR to indicate the test error.
 *
 * ERRORS
 *  N/A
 */

int
TestDo(void)
{
    /* Start myTimer immediately */
    if (TimerStart(myTimer, NO_WAIT) == SUCCESS)
    {
        TEST_FAILURE (FAILURE);
    }
    
    TEST_SUCCESS (SUCCESS);
}

