/*
 * TP_SRD_TIM_0133.c - High-level test procedure for Timer Service
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

#define     FLAG_SET                1
#define     FLAG_CLEAR              0

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0133_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global Variables */
static Semaphore       semIdA, semIdB, semIdC, semIdD, semIdE, semIdF;
static int             flags[15];
static MessageQueue    myMessageId, myMessageIdA;
static char *          sendStr = "Hello";
static Size            length    = 5;
static Priority        messagePriority = 10;
static Timer           myTimer;

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
    Size                msgSize;
    Priority            myPriority;
    char                msgBuf[10];
    SemaphoreStatus     semStatus;
    Priority            newPriority = 190;
    MessageQueueStatus  messageStatus;
    
    /* Get a binary semaphore. */ 
    if ((SemaphoreWait(semIdA, WAIT_FOREVER)) != SUCCESS)
    {
        flags[0] = FLAG_SET;
    }

    /* Get a counting semahpore. */
    if ((SemaphoreWait(semIdB, WAIT_FOREVER)) != SUCCESS)
    {
    	flags[1] = FLAG_SET;
    }

    /* Get status of the counting semaphore. */
    if ((SemaphoreGetStatus(semIdB, &semStatus)) != SUCCESS)
    {
    	flags[2] = FLAG_SET;
    }
    
    /* Set a ceiling priority of mutex sempahore. */
    if ((SemaphoreSetPriority(semIdC, newPriority)) != SUCCESS)
    {
    	flags[3] = FLAG_SET;
    }
    
    /* Send a message to message queue. */
    if ((MessageQueueSend(myMessageId, (Address)sendStr, length, messagePriority, WAIT_FOREVER)) != SUCCESS)
    {
    	flags[4] = FLAG_SET;
    }
    
    /* Recevie a message from message queue. */
    if ((MessageQueueReceive(myMessageId, &msgSize, WAIT_FOREVER, (Address)&msgBuf, &myPriority)) != SUCCESS)
    {
    	flags[5] = FLAG_SET;
    }
    
    /* Create a binary semaphore. */
    if ((SemaphoreBinaryCreate("BinSemD_TIM133", QUEUE_TYPE_FIFO, 
                              SEM_UNLOCKED, &semIdD)) != SUCCESS)
    {
        flags[6] = FLAG_SET;
    }    
    
    /* Create a counting semaphore. */
    if ((SemaphoreCountCreate("CntSemE_TIM133", QUEUE_TYPE_FIFO, 
                             5, 10, &semIdE)) != SUCCESS)
    {
    	flags[7] = FLAG_SET;
    }    
    
    /* Create a mutex semaphore. */
    if ((SemaphoreMutexCreate("MutexSemF_TIM133", SEM_CEILING, 
                                180, &semIdF)) != SUCCESS)
    {
    	flags[8] = FLAG_SET;
    }    
    
    /* Get the mutex semaphore. */
    if ((SemaphoreWait(semIdC, NO_WAIT)) != SUCCESS)
    {
    	flags[9] = FLAG_SET;
    }
    
    /* Release the mutex semaphore. */
    if ((SemaphoreReleaseAsync(semIdC)) != SUCCESS)
    {
    	flags[10] = FLAG_SET;
    }    
    
    /* Create a message queue. */
    if ((MessageQueueCreate("myMSGA_TIM133", QUEUE_TYPE_FIFO, 10, 1, 
                                &myMessageIdA)) != SUCCESS)
    {
    	flags[11] = FLAG_SET;
    }   
    
    /* Get a status of the message queue. */
    if ((MessageQueueGetStatus(myMessageId, &messageStatus)) != SUCCESS)
    {
    	flags[12] = FLAG_SET;
    }
    
    /* Get a message queue ID by the name. */
    if ((MessageQueueGetByName("myMSG_TIM133", &myMessageIdA)) != SUCCESS)
    {
    	flags[13] = FLAG_SET;
    }
        
    /* Get a semaphore ID by the name. */
    if ((SemaphoreGetByName("binsem_TIM133", &semIdE)) != SUCCESS)
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
  
    /* create binary semaphore with valid parameters. */
    if ((result = SemaphoreBinaryCreate("binsem_TIM133", QUEUE_TYPE_FIFO, 
                              SEM_LOCKED, &semIdA)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create counting semaphore with valid parameters. */
    if ((result = SemaphoreCountCreate("CntSem_TIM133", QUEUE_TYPE_FIFO, 
                             0, 10, &semIdB)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Create mutex semaphore with valid parameters. */
    if ((result = SemaphoreMutexCreate("MutexSem_TIM133", SEM_CEILING, 
                                180, &semIdC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create message queue. */
    if((result = MessageQueueCreate("myMSG_TIM133", QUEUE_TYPE_FIFO, 10, 1, 
                                &myMessageId)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Create a software timer with valid name, valid software timer service and in one-shot mode. */
    if ((result = TimerCreate("Timer_TIM133", TIMER_ONESHOT, MyHandler, NULL, 
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
 *  Test case to exercise if NEOS-178S cannot invoke the below methods(semaphore and 
 *  message queue) in the timer service routine.
 *   - Method to get a binary semaphore
 *   - Method to get a counting semaphore
 *   - Method to get a status of a semaphore
 *   - Method to set a ceiling priority of mutex semaphore
 *   - Method to send a message to message queue
 *   - Method to receive a message from message queue
 *   - Method to create a binary semaphore.
 *   - Method to create a counting semaphore.
 *   - Method to create a mutex semaphore.
 *   - Method to get a mutex semaphore.
 *   - Method to release a mutex semaphore.
 *   - Method to create a message queue.
 *   - Method to get a status of the message queue.
 *   - Method to get a message queue ID by the name.
 *   - Method to get a semaphore ID by the name.
 *
 * PARAMETERS
 *  N/A
 * 
 * TRACEABILITY
 *  [TP_SRD_TIM_0133 -> TC_SRD_TIM_0133]
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
