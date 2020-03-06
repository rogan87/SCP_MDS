/*
 * TP_SRD_TIM_0132.c - High-level test procedure for Time Management
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

#define     THREAD_STACK            1024   
#define     THREAD_PRIORITY         100
#define     FLAG_SET                1
#define     FLAG_CLEAR              0

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0132_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global Variables */
static Timer           myTimer;
static Thread          threadAId, threadBId, threadCId, threadDId;
static Thread          threadEId, threadFId, tempThread;
static int             flags[15];

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
 *
 * ThreadRoutine - User Thread Routine
 * 
 * DESCRIPTION
 *   This is the entry point of Thread Routine.
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
ThreadRoutine(void)
{
    while(1);
}

/**
 * MyHanlder - timer service routine
 *
 * DESCRIPTION
 *  This function is called when specified timer is expired.
 *
 * PARAMETERS
 *  cookie     - [IN]  parameter data for timer service routine
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
MyHandler(Address cookie)
{
    ThreadStatus thrStatus;
    Priority     newPriority = 150;
    TimeValue    timeCapacity = 100*TEST_MSEC;
    TimeValue    periodicTime = 200*TEST_MSEC;
    TimeValue    budgetTime   = 50*TEST_MSEC;

    /* Delay the execution. */
    if ((ThreadDelay(100*TEST_MSEC)) != SUCCESS)
    {
        flags[0] = FLAG_SET;
    }
    
    /* Start ThreadA. */
    if ((ThreadStart(threadAId, NO_WAIT)) != SUCCESS)
    {
    	flags[1] = FLAG_SET;
    }
    
    /* Stop ThreadB. */
    if ((ThreadStop(threadBId)) != SUCCESS)
    {
    	flags[2] = FLAG_SET;
    }

    /* Get a status of the ThreadA. */
    if ((ThreadGetStatus(threadCId, &thrStatus)) != SUCCESS)
    {
    	flags[3] = FLAG_SET;
    }

    /* Set a priority of the ThreadB. */
    if ((ThreadSetPriority(threadCId, newPriority)) != SUCCESS)
    {
    	flags[4] = FLAG_SET;
    }

    /* Suspend the ThreadC. */
    if ((ThreadSuspend(threadCId)) != SUCCESS)
    {
    	flags[5] = FLAG_SET;
    }

    /* Resume the ThreadD. */
    if ((ThreadResume(threadDId)) != SUCCESS)
    {
    	flags[6] = FLAG_SET;
    }

    /* Get Thread ID using name of ThreadC. */
    if ((ThreadGetByName("ThreadC_TIM132", &tempThread)) != SUCCESS)
    {
    	flags[7] = FLAG_SET;
    }

    /* Create ThreadE. */
    if ((ThreadCreate("ThreadE_TIM132", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadRoutine, 
                              NULL, &threadEId)) != SUCCESS)
    {
    	flags[8] = FLAG_SET;
    }  
    
    /* Create periodic threadF. */
    if ((ThreadPeriodicCreate("ThreadF_TIM132", THREAD_STACK, THREAD_PRIORITY, timeCapacity, 
            periodicTime, SOFT_DEADLINE, (Address)ThreadRoutine, NULL, &threadFId)) != SUCCESS)
    {
    	flags[9] = FLAG_SET;
    }
    
    /* Create periodic threadF. */
    if ((ThreadPeriodicCreate("ThreadF_TIM132", THREAD_STACK, THREAD_PRIORITY, timeCapacity, 
            periodicTime, SOFT_DEADLINE, (Address)ThreadRoutine, NULL, &threadFId)) != SUCCESS)
    {
    	flags[10] = FLAG_SET;
    }
    
    /* Make the current periodic thread till the next release point. */
    if ((ThreadWaitNextPeriod()) != SUCCESS)
    {
    	flags[11] = FLAG_SET;
    }
    
    /* Replenish the deadline of the current periodic thread. */
    if ((ThreadReplenishDeadline(budgetTime)) != SUCCESS)
    {
    	flags[12] = FLAG_SET;
    }
    
    /* Suspend the current thread. */
    if ((ThreadSuspendSelf(100*TEST_MSEC)) != SUCCESS)
    {
    	flags[13] = FLAG_SET;
    }
    
    /* Stop the current thread. */
    if ((ThreadStopSelf()) != SUCCESS)
    {
    	flags[14] = FLAG_SET;
    }
}

/**
 * TestSetPrecondition - setting precondition
 *
 * DESCRIPTION
 *  Initialize environment for this test case before TestDo() function 
 *  is executed.
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
    int     result;

    /* Create ThreadA, ThreadB, ThreadC and ThreadD. */
    if ((result = ThreadCreate("ThreadA_TIM132", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadRoutine, 
                              NULL, &threadAId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }  
    if ((result = ThreadCreate("ThreadB_TIM132", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadRoutine, 
                              NULL, &threadBId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }  
    if ((result = ThreadCreate("ThreadC_TIM132", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadRoutine, 
                              NULL, &threadCId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }  
    if ((result = ThreadCreate("ThreadD_TIM132", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadRoutine, 
                              NULL, &threadDId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }  

    /* Create a software timer with valid name, valid software timer service and in one-shot mode. */
    if ((result = TimerCreate("Timer_TIM132", TIMER_ONESHOT, MyHandler, NULL, 
                    &myTimer)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    return (SUCCESS);
}

/**
 * TestDo - Performing test
 *
 * DESCRIPTION
 *  Test case to exercise if NEOS-178S cannot invoke the below methods(thread) in the timer service routine.
 *   - Method to delay the execution
 *   - Method to start a created thread
 *   - Method to stop a thread
 *   - Method to get a status of a thread
 *   - Method to set a priority of thread
 *   - Method to suspend a thread
 *   - Method to resume a thread
 *   - Method to get a thread ID using a name of thread
 *   - Method to create non-periodic thread
 *   - Method to create periodic thread
 *   - Method to make the current periodic thread till the next release point.
 *   - Method to replenish the deadline of the current periodic thread.
 *   - Method to suspend the current thread.
 *   - Method to stop the current thread.
 *
 * PARAMETERS
 *  N/A
 * 
 * TRACEABILITY
 *  [TP_SRD_TIM_0132 -> TC_SRD_TIM_0132]
 *
 * RETURNS
 *  This routine returns SUCCESS if the test is passed successfullsy,
 *  or it returns error code to indicate API error code.
 *  or it returns FAILURE to indicate testing failure.
 *
 * ERRORS
 *  N/A
 */

static int 
TestDo(void)
{
    int   result, count;
    
    /* Clear flag */
    for (count = 0; count < 15; count++)
    {
        flags[count] = FLAG_CLEAR;
    }

    /* Start ThreadB, ThreadC and threadD. */
    if ((result = ThreadStart(threadBId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    if ((result = ThreadStart(threadCId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    if ((result = ThreadStart(threadDId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Suspend ThreadD. */
    if ((result = ThreadSuspend(threadDId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* start myTimer after 0.1 sec */
    if ((result = TimerStart(myTimer, 100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* delay test context thread. */
    if ((result = ThreadDelay(200*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Check flag. */
    if (flags[0] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[1] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[2] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[3] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[4] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[5] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[6] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[7] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[8] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[9] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[10] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[11] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[12] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    if (flags[13] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }    
    if (flags[14] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    TEST_SUCCESS (SUCCESS);
}
