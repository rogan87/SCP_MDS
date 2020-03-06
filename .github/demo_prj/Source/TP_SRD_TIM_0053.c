/*
 * TP_SRD_TIM_0053.c - High-level test procedure for Time Management
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

#define INVALID_NAME "ABCDEFGHIJKLMNOPQRSTUVWXYZA_TIM53"
#define FLAG_CLEAR  0
#define FLAG_SET    1

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0053_ID =
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
static int    FLAGA, FLAGB, FLAGC, FLAGD, FLAGE;
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
    /* Clear FLAGs. */
    FLAGA = FLAG_CLEAR;
    FLAGB = FLAG_CLEAR;
    FLAGC = FLAG_CLEAR;
    FLAGD = FLAG_CLEAR;
    FLAGE = FLAG_CLEAR;
    
    /* Create a software timer with invalid name that is NULL. */
    if ((result = TimerCreate(NULL, TIMER_ONESHOT, myTimer, NULL, 
                    &myTimerId)) != SUCCESS)
    {
        FLAGA = FLAG_SET;
    }
    
    /* 
     * Create a software timer with invalid name that is greater than 
     * 32 ASCII characters. 
     */
    if ((result = TimerCreate(INVALID_NAME, TIMER_ONESHOT, myTimer, NULL, 
                    &myTimerId)) != SUCCESS)
    {
        FLAGB = FLAG_SET;
    }
    
    /* 
     * Create a software timer with invalid type that is neither one shot mode 
     * nor periodical mode.
     */
    if ((result = TimerCreate("myTimerA_TIM53", TIMER_ONESHOT - 1, myTimer, NULL, 
                    &myTimerId)) != SUCCESS)
    {
        FLAGC = FLAG_SET;
    }

    if ((result = TimerCreate("myTimerB_TIM53", TIMER_PERIODIC + 1, myTimer, NULL, 
                    &myTimerId)) != SUCCESS)
    {
        FLAGD = FLAG_SET;
    }
    
    /* Create a software timer with invalid call that is NULL. */
    if ((result = TimerCreate("myTimerC_TIM53", TIMER_ONESHOT, NULL, NULL, 
                    &myTimerId)) != SUCCESS)
    {
        FLAGE = FLAG_SET;
    }   
    
    return (SUCCESS);
}

/**
 * TestDo - preforming test
 *
 * DESCRIPTION
 *  Test code to exercise if NEOS-178S cannot create a software timer 
 *  with invalid parameters.
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0053 -> TC_SRD_TIM_0053]
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
    /* Check FLAGs. */
    if (FLAGA != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (FLAGB != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (FLAGC != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (FLAGD != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (FLAGE != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
            
    TEST_SUCCESS (SUCCESS);
}

