/*
 * TP_SRD_TIM_0061.c - High-level test procedure for Time Management
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

#define     TIMER_NAME      "Timer_TIM61"

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0061_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global variables */
static Timer  myTimer;

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
myTimer_h(Address args)
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
     * Create a software timer with valid name, valid software timer service 
     * and in one shot mode.
     */
    if ((result = TimerCreate(TIMER_NAME, TIMER_ONESHOT, myTimer_h, NULL, 
                         &myTimer)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    return (SUCCESS);
}

/**
 * TestDo - perfroming test
 *
 * DESCRIPTION
 *   Test code to exercise if NEOS-178S provides software timer ID of a specified
 *   software timer name that is set ASCII character string that is limited to 
 *   32 ASCII characters.. 
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0061 -> TC_SRD_TIM_0061]
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
    Timer    myTimer2;
    int      result;

    /* 
     * check if software timer with "Timer" name is exsit. 
     */    
    if ((result = TimerGetByName(TIMER_NAME, &myTimer2)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }     

    /* check software timer ID */
    if (myTimer2 != myTimer)
    {
        TEST_FAILURE (FAILURE);
    }
   
    TEST_SUCCESS (SUCCESS);
}
