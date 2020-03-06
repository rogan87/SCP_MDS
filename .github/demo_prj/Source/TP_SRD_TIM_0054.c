/*
 * TP_SRD_TIM_0054.c - High-level test procedure for Time Management
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

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0054_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global variables */
static Timer  myTimerId;
static int    result;

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
    return;
}

/**
 * myTimer - handler fucntion
 *
 * DESCRIPTION
 *  This is callback function for timer.
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
myTimer(Address args)
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
 *  This routine returns SUCCESS if the test is passed successfully,
 *  or it returns ERROR to indicate the test error.
 *
 * ERRORS
 *  N/A
 */

static int 
TestSetPrecondition(void)
{
    return (SUCCESS);
}

/**
 * TestDo - preforming test
 *
 * DESCRIPTION
 *  Test code to exercise if NEOS-178S cannot create a software timer 
 *  in normal state.
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0054 -> TC_SRD_TIM_0054]
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
    /* 
     * Create two software timers with valid name, valid software timer service 
     * and in one shot mode.
     */
    if ((result = TimerCreate("TimerA_TIM", TIMER_ONESHOT, myTimer, NULL, 
                    &myTimerId)) == SUCCESS)
    {
        TEST_FAILURE (result);
    }
            
    TEST_SUCCESS (SUCCESS);
}

