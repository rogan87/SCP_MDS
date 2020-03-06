/*
 * TP_SRD_TIM_0131.c - High-level test procedure for Time Management
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
#include <private/String.h>

#define     FLAG_SET                1
#define     FLAG_CLEAR              0
#define     PRIORITY                10
#define     THREAD_STACK            1024
#define     THREAD_PRIORITY         50
#define     INTR_IRQ0               48	    /* IRQ #0 */



static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0131_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global variables */
static Timer           myTimer;
static Semaphore       binSemIdA, binSemIdB, cntSemIdA, cntSemIdB;
static int             flags[19];

static Thread          threadId;
static TimeValue       sysTimeValue;
static TimeValue       testDoSysTimeValue;
static SystemState     sysState;
static SystemState     testDoSysState;
static char *          sysGetVersionString;
static char *          testDoSysGetVersionString;
static Size            strSize;
static Value           valueInterrupt;
static Thread          threadAId, threadBId, threadCId, threadDId;
static Thread          threadEId, threadFId;

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
 * ThreadA - User ThreadA
 * 
 * DESCRIPTION
 *   This is the entry point of ThreadA.
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
ThreadA(void)
{
    /* acquire the binary semaphore binSemIdA. */
    SemaphoreWait(binSemIdA, WAIT_FOREVER);
    flags[13] = FLAG_SET;
}

/**
 *
 * ThreadB - User ThreadB
 * 
 * DESCRIPTION
 *   This is the entry point of ThreadB.
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
ThreadB(void)
{
    /* acquire the binary semaphore binSemIdB. */
    SemaphoreWait(binSemIdB, WAIT_FOREVER);
    flags[14] = FLAG_SET;
}

/**
 *
 * ThreadC - User ThreadC
 * 
 * DESCRIPTION
 *   This is the entry point of ThreadC.
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
ThreadC(void)
{
    /* acquire the binary semaphore binSemIdB. */
    SemaphoreWait(binSemIdB, WAIT_FOREVER);
    flags[15] = FLAG_SET;
}

/**
 *
 * ThreadD - User ThreadD
 * 
 * DESCRIPTION
 *   This is the entry point of ThreadD.
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
ThreadD(void)
{
    /* acquire the counting semaphore cntSemIdA. */
    SemaphoreWait(cntSemIdA, WAIT_FOREVER);
    flags[16] = FLAG_SET;
}

/**
 *
 * ThreadE - User ThreadE
 * 
 * DESCRIPTION
 *   This is the entry point of ThreadE.
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
ThreadE(void)
{
    /* acquire the counting semaphore cntSemIdB. */
    SemaphoreWait(cntSemIdB, WAIT_FOREVER);
    flags[17] = FLAG_SET;
}

/**
 *
 * ThreadF - User ThreadF
 * 
 * DESCRIPTION
 *   This is the entry point of ThreadF.
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
ThreadF(void)
{
    /* acquire the counting semaphore cntSemIdB. */
    SemaphoreWait(cntSemIdB, WAIT_FOREVER);
    flags[18] = FLAG_SET;
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
    /* Get the thread Id of current thread. */
    threadId = ThreadGetIdSelf();
    if (threadId != NULL)
    {
        flags[0] = FLAG_SET;
    }
	
    /* Release the binary semaphore binSemIdA. */
    if (SemaphoreReleaseAsync(binSemIdA) == SUCCESS)
    {
    	flags[1] = FLAG_SET;
    }   
    
    /* Release the couning semaphore. */
    if (SemaphoreReleaseAsync(cntSemIdA) == SUCCESS)
    {
    	flags[2] = FLAG_SET;
    }  
    
    /* Flush the binary semaphore binSemIdB. */
    if (SemaphoreFlushAsync(binSemIdB) == SUCCESS)
    {
    	flags[3] = FLAG_SET;
    }  

    /* Flush the counting semaphore cntSemIdB. */
    if (SemaphoreFlushAsync(cntSemIdB) == SUCCESS)
    {
    	flags[4] = FLAG_SET;
    } 

    /* Get the system time elapsed since the system is powered ON. */
    sysTimeValue = SystemClockGetTime();
    if (sysTimeValue >= testDoSysTimeValue)
    {
        flags[5] = FLAG_SET;
    }

    /* Get the current system state value. */
    sysState = SystemStateGet();
    if (sysState == testDoSysState)
    {
        flags[6] = FLAG_SET;
    }

    /* Read the software configuration item identifier. */
    sysGetVersionString = (char *)VersionGetString();
    strSize = StringLength(sysGetVersionString, 100);	
    if (StringCompare(sysGetVersionString, testDoSysGetVersionString, strSize) == 0)
    {
        flags[7] = FLAG_SET;
    }

    /* Disable all external hardware interrupts. */
    valueInterrupt = InterruptLock();
	
    /* Enable all external hardware interrupts. */
    InterruptUnLock(valueInterrupt);
	
#if 0  /* Not Support */
    /* Disable a timer0 interrupt request. */
    if (InterruptDisable(TFX_INTR_TIMER0) == SUCCESS)
    {
        flags[8] = FLAG_SET;
    }
    /* Enable a timer0 interrupt request. */
    if (InterruptEnable(TFX_INTR_TIMER0) == SUCCESS)
    {
        flags[9] = FLAG_SET;
    }
	
    /* Set valid interrupt sense property. */
    if (InterruptSetSense(INTR_IRQ0, SENSE_LEVEL) == SUCCESS)
    {
    	flags[10] = FLAG_SET;
    }
#endif
	
    /* Set valid interrupt priority property. */
    if(InterruptSetPriority(TFX_INTR_TIMER0, PRIORITY) == SUCCESS)
    {
    	flags[11] = FLAG_SET;
    }
    
#if 0
    /* Set valid interrupt polarity property. */
    if (InterruptSetPolarity(INTR_IRQ0, POLARITY_NEG) == SUCCESS)
    {
    	flags[12] = FLAG_SET;
    }
#endif
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

    /* Create Thread A with low priority */
    if ((result = ThreadCreate("ThreadA_TIM131", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadA, 
                              NULL, &threadAId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create Thread B with low priority */
    if ((result = ThreadCreate("ThreadB_TIM131", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadB, 
                              NULL, &threadBId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create Thread C with low priority */
    if ((result = ThreadCreate("ThreadC_TIM131", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadC, 
                              NULL, &threadCId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create Thread D with low priority */
    if ((result = ThreadCreate("ThreadD_TIM131", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadD, 
                              NULL, &threadDId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create Thread E with low priority */
    if ((result = ThreadCreate("ThreadE_TIM131", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadE, 
                              NULL, &threadEId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create Thread F with low priority */
    if ((result = ThreadCreate("ThreadF_TIM131", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadF, 
                              NULL, &threadFId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }    
  
    /* create binary semaphore with valid parameters. */
    if ((result = SemaphoreBinaryCreate("binSemA_TIM131", QUEUE_TYPE_FIFO, 
                              SEM_LOCKED, &binSemIdA)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* create binary semaphore with valid parameters. */
    if ((result = SemaphoreBinaryCreate("binSemB_TIM131", QUEUE_TYPE_FIFO, 
                              SEM_LOCKED, &binSemIdB)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create counting semaphore with valid parameters. */
    if ((result = SemaphoreCountCreate("cntSemA_TIM131", QUEUE_TYPE_FIFO, 
                             0, 10, &cntSemIdA)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create counting semaphore with valid parameters. */
    if ((result = SemaphoreCountCreate("cntSemB_TIM131", QUEUE_TYPE_FIFO, 
                             0, 10, &cntSemIdB)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create a software timer with valid name, valid software timer service and in one-shot mode. */
    if ((result = TimerCreate("Timer_TIM131", TIMER_ONESHOT, (CallBack)MyTimer_h, NULL, 
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
 *   Test code to exercise if NEOS-178S allow the user to invoke the below methods  
 *   in the software timer service function.
 *  - Method to provide the thread ID of current thread.
 *  - Method to release a specified binary semaphore
 *  - Method to release a specified counting semaphore
 *  - Method to wake-up all threads waiting on a specified binary semaphore.
 *  - Nethod to wake-up all threads waiting on a specified couting semaphore.
 *  - Method to provide system time elapsed since the system is powered ON
 *  - Method to provide the current system state value
 *  - Method to read the software configuration item identifier
 *  - Method to disable all external hardware interrupts
 *  - Method to enable all external hardware interrupts
 *  - Method to disable a specified interrupt request
 *  - Method to enable a specified interrupt request
 *  - Method to set interrupt sense property
 *  - Method to set interrupt priority property
 *  - Method to set interrupt polarity property
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0131 -> TC_SRD_TIM_0131]
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
    int result, count;

    for (count = 0; count < 19; count++)
    {
        flags[count] = FLAG_CLEAR;
    }
    
    /* Start threadAId. */
    if ((result = ThreadStart(threadAId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Start threadBId. */
    if ((result = ThreadStart(threadBId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Start threadCId. */
    if ((result = ThreadStart(threadCId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Start threadDId. */
    if ((result = ThreadStart(threadDId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Start threadEId. */
    if ((result = ThreadStart(threadEId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Start threadFId. */
    if ((result = ThreadStart(threadFId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* start myTimer after 0.1 sec */
    if ((result = TimerStart(myTimer, 100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* need to compare return value of functions in timer service routine. */
    testDoSysTimeValue = SystemClockGetTime();
    testDoSysState = SystemStateGet();
    testDoSysGetVersionString = (char *)VersionGetString();

    /* wait for calling callback function */
    if ((result = ThreadDelay(200*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
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
    
#if 0 /* Not Support */
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
#endif
    
    if (flags[11] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
#if 0
    if (flags[12] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
#endif
    
    if (flags[13] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (flags[14] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (flags[15] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (flags[16] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (flags[17] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }
    
    if (flags[18] != FLAG_SET)
    {
        TEST_FAILURE (FAILURE);
    }

    TEST_SUCCESS (SUCCESS);
}

