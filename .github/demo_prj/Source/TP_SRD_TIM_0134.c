/*
 * TP_SRD_TIM_0134.c - High-level test procedure for Timer Service
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

TEST_ID TP_SRD_TIM_0134_ID =
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
static int             flags[9];
static Thread          threadAId;
static MemoryPool      memPoolId, tempMemPoolId;
UserExtension          myExtension;
UserExtensionID        myExtensionIdA, myExtensionIdB;

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
    ThreadStop(threadAId);
    return;
}

/**
 *
 * CbDummy - Dummy extention point
 * 
 * DESCRIPTION
 *   This is the dummy extention point.
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
CbDummy (Thread theThread)
{
    return;
}

/**
 *
 * CbThreadSwitch - Thread switch extention point
 * 
 * DESCRIPTION
 *   This is the thread switch extention point.
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
CbThreadSwitch (Thread previousThread, Thread nextThread)
{
    return;
}

/**
 *
 * CbSystemClockTick - System clock tick extention point
 * 
 * DESCRIPTION
 *   This is the system clock tick extention point.
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
CbSystemClockTick (void)
{
    return;
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
    Address             buf;
    MemoryPoolStatus    memStatus;
    EventSet            events = 0;
    EventSet            expected = 0;
    EventSet            received;

    /* Allocate memory from user-defined memory pool. */
    if ((MemoryPoolAllocate(memPoolId, 1024, &buf)) != SUCCESS)
    {
        flags[0] = FLAG_SET;
    }

    /* Get status of memory pool. */
    if ((MemoryPoolGetStatus(memPoolId, &memStatus)) != SUCCESS)
    {
    	flags[1] = FLAG_SET;
    }

    /* Get memory pool ID using name of user-defined memory pool. */
    if ((MemoryPoolGetByName("MPA", &tempMemPoolId)) != SUCCESS)
    {
    	flags[2] = FLAG_SET;
    }

    /* Get status of system memory pool. */
    if ((SystemMemoryPoolGetStatus(&memStatus)) != SUCCESS)
    {
    	flags[3] = FLAG_SET;
    }

    events |= 0x1 << 5;

    /* Send events to ThreadA. */
    if ((EventFlagSend(threadAId, events)) != SUCCESS)
    {
    	flags[4] = FLAG_SET;
    }

    /* Register user extension points. */
    if ((UserExtensionCreate(&myExtension, &myExtensionIdB)) != SUCCESS)
    {
        flags[5] = FLAG_SET;
    } 

    /* Unregister user extension points. */
    if ((UserExtensionDelete(myExtensionIdA)) != SUCCESS)
    {
        flags[6] = FLAG_SET;
    }
    
    expected |= 0x1 << 5;    
    /* Recevie an event. */
    if ((EventFlagReceive(expected, EVENT_WAIT_ANY, 100*TEST_MSEC, &received)) != SUCCESS)
    {
    	flags[7] = FLAG_SET;
    }
    
    /* Clear an eventflag. */
    if ((EventFlagClear()) != SUCCESS)
    {
    	flags[8] = FLAG_SET;
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

    /* Create Thread A with low priority */
    if ((result = ThreadCreate("ThreadA_TIM134", THREAD_STACK, THREAD_PRIORITY, (Address)ThreadA, 
                              NULL, &threadAId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Start ThreadA. */
    if ((result = ThreadStart(threadAId, NO_WAIT)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    myExtension.createCallback    = CbDummy;
    myExtension.startCallback     = CbDummy;
    myExtension.stopCallback      = CbDummy;
    myExtension.restartCallback   = CbDummy;
    myExtension.switchCallback    = CbThreadSwitch;
    myExtension.clockTickCallback = CbSystemClockTick;
    
    /* Register user extension points. */
    if ((result = UserExtensionCreate(&myExtension, &myExtensionIdA)) != SUCCESS)
    {
    	TEST_FAILURE (result);
    } 
    
    /* Create a software timer with valid name, valid software timer service and in one-shot mode. */
    if ((result = TimerCreate("Timer_TIM134", TIMER_ONESHOT, MyHandler, NULL, 
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
 *  Test case to exercise if NEOS-178S cannot invoke the below methods(memory, event and 
 *  UserExtension) in the timer service routine.
 *   - Method to allocate a memory from user-defined memory pool
 *   - Method to get a status of memory pool
 *   - Method to get a memory pool Id using name of memory pool
 *   - Method to get the status of the system memory pool
 *   - Method to send specified event to a thread
 *   - Method to register the user extensions
 *   - Method to unregister the user extensions
 *   - Method to receive an event.
 *   - Method to clear an event flag.
 *
 * PARAMETERS
 *  N/A
 * 
 * TRACEABILITY
 *  [TP_SRD_TIM_0134 -> TC_SRD_TIM_0134]
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
    for (count = 0; count < 9; count++)
    {
        flags[count] = FLAG_CLEAR;
    }
    
    /* Get Id of user-defined memory pool using name. */
    if ((result = MemoryPoolGetByName("MPA", &memPoolId)) != SUCCESS)
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
    
    TEST_SUCCESS (SUCCESS);
}
