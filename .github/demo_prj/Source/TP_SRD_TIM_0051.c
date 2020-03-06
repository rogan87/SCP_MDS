/*
 * TP_SRD_TIM_0051.c - High-level test procedure for Time Management
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

#define     TIMER1_NAME      "Timer1_TIM51"
#define     TIMER2_NAME      "Timer2_TIM51"
#define     TIMER3_NAME      "Timer3_TIM51"
#define     TIMER4_NAME      "Timer4_TIM51"

#define     TIMER_MAX       4

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0051_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global variables */
static Timer  myTimer[TIMER_MAX];

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
 *  This routine returns SUCCESS
 *
 * ERRORS
 *  N/A
 */

static void 
TestCleanUp(void)
{
}

/**
 * myTimer_h1 - handler fucntion
 *
 * DESCRIPTION
 *  This is callback function for Mytimer[0] 
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
myTimer_h1(Address args)
{

}

/**
 * myTimer_h2 - handler fucntion
 *
 * DESCRIPTION
 *  This is callback function for Mytimer[1] 
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
myTimer_h2(Address args)
{

}

/**
 * myTimer_h3 - handler fucntion
 *
 * DESCRIPTION
 *  This is callback function for Mytimer[2] 
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
myTimer_h3(Address args)
{

}

/**
 * myTimer_h4 - handler fucntion
 *
 * DESCRIPTION
 *  This is callback function for Mytimer[3] 
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
myTimer_h4(Address args)
{

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
     * Create two software timers with valid name, valid software timer service 
     * and in one shot mode.
     */
    if ((result = TimerCreate(TIMER1_NAME, TIMER_ONESHOT, myTimer_h1, NULL, 
                    &myTimer[0])) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    if ((result = TimerCreate(TIMER2_NAME, TIMER_ONESHOT, myTimer_h2, NULL, 
                    &myTimer[1])) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* 
     * Create two software timers with valid name, valid software timer service 
     * and in periodical mode.
     */
    if ((result = TimerCreate(TIMER3_NAME, TIMER_PERIODIC, myTimer_h3, NULL, 
                    &myTimer[2])) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    if ((result = TimerCreate(TIMER4_NAME, TIMER_PERIODIC, myTimer_h4, NULL, 
                    &myTimer[3])) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    return (SUCCESS);
}

/**
 * TestDo - preforming test
 *
 * DESCRIPTION
 *   Test code to exercise if NEOS-178S creates and establishes unique ID for 
 *   software timer during software timer creation.>
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0051 -> TC_SRD_TIM_0051]
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
    int index;
    int timerId_index;

    /* compare four Software Timer ID's */
    for (index = 0; index < TIMER_MAX-1; index++)
    {
        for (timerId_index = index + 1; timerId_index < TIMER_MAX; 
             timerId_index++)
        {
            if (myTimer[timerId_index] == myTimer[index])
            {
                TEST_FAILURE (FAILURE);
            }
        }
    }
    
    TEST_SUCCESS (SUCCESS);
}

