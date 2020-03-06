/*
 * TP_SRD_TIM_0135.c - High-level test procedure for Timer Service
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

#define     INTERRUPT_STACK_SIZE    1024
#define     THREAD_STACK            1024
#define     THREAD_PRIORITY         100
#define     FLAG_SET                1
#define     FLAG_CLEAR              0

static int  TestSetPrecondition(void);
static int  TestDo(void);
static void TestCleanUp(void);

TEST_ID TP_SRD_TIM_0135_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp
};

/* Global Variables */
static Timer           myTimerA, myTimerB, myTimerC, myTimerD;
static int             flags[10];

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
 * MyExceptionHandler - Exception Handler Routine.
 *
 * DESCRIPTION
 *  This function is called as exception handler routine.
 *
 * PARAMETERS
 *  eFrame     - [IN]  excpetion data for exception handler routine
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
MyExceptionHandler(ExceptionFrame *eFrame)
{
    return;
}

/**
 * MyInterruptHanlder - Interrupt service routine
 *
 * DESCRIPTION
 *  This function is called as interrupt service routine.
 *
 * PARAMETERS
 *  cookie     - [IN]  parameter data for interrupt service routine
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

static Value
MyInterruptHandler(Address cookie)
{
    return (INTERRUPT_HANDLED);
}

/**
 * MyTimerHanlder - Timer service routine
 *
 * DESCRIPTION
 *  This function is called as timer service routine.
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
MyTimerHandler(Address cookie)
{
    return;
}

/**
 * DummyErrorHanlder - Dummy Error handler routine
 *
 * DESCRIPTION
 *  This function is called as dummy error handler.
 *
 * PARAMETERS
 *  cookie     - [IN]  parameter data for dummy error handler routine
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

static void *
DummyErrorHandler(void *args)
{
    return NULL;
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
    Interrupt      myInterrupt;
    Timer          theTimer;
    ErrorStatus    errorStatus;

    /* Attach an interrupt service routine. */
    myInterrupt.handler = MyInterruptHandler;
    myInterrupt.cookie  = NULL;
    myInterrupt.irq     = TFX_INTR_TIMER0;

    /* register the ISR */
    if ((InterruptAttach(TFX_INTR_TIMER0, INTERRUPT_STACK_SIZE, 
                                 &myInterrupt)) != SUCCESS)
    {
        flags[0] = FLAG_SET;
    }  

    /* Create software timerD. */
    if ((TimerCreate("TimerD_TIM135", TIMER_ONESHOT, MyTimerHandler, NULL, 
                    &myTimerD)) != SUCCESS)
    {
        flags[1] = FLAG_SET;
    }
    
    /* Start software timer with created timerB. */
    if ((TimerStart(myTimerB, 100*TEST_MSEC)) != SUCCESS)
    {
        flags[2] = FLAG_SET;
    }

    /* Stop software timer with running timerC. */
    if ((TimerStop(myTimerC)) != SUCCESS)
    {
        flags[3] = FLAG_SET;
    }

    /* Get ID using a name of software timerC.  */
    if ((TimerGetByName("TimerC_TIM135", &theTimer)) != SUCCESS)
    {
        flags[4] = FLAG_SET;
    }

    /* Create and start the error handler thread. */
    if ((HmCreateErrorHandler(THREAD_STACK, (Address)DummyErrorHandler)) != SUCCESS)
    {
        flags[5] = FLAG_SET;
    }

    /* Provide error from error status queue to the error handler thread. */
    if ((HmGetError(&errorStatus)) != SUCCESS)
    {
        flags[6] = FLAG_SET;
    }

    /* Raise error to the error handler thread. */
    if ((HmRaiseError(APPLICATION_ERROR, (ErrorMessage *)"test", 4)) != SUCCESS)
    {
        flags[7] = FLAG_SET;
    }

    /* Register an excpeiton handler for the system reset. */
    if ((ExceptionSetHandler(EXC_OFF_SYS_RST, MyExceptionHandler)) != SUCCESS)
    {
    	flags[8] = FLAG_SET;
    }    
    
    /* Unregister the registered excpetion handler for the machine check. */
    if ((ExceptionClearHandler(EXC_OFF_MCHECK)) != SUCCESS)
    {
    	flags[9] = FLAG_SET;
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

    /* Register the exception handler for the machine check. */
    if ((result = ExceptionSetHandler(EXC_OFF_MCHECK, MyExceptionHandler)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    
    /* Create a software timerA with valid name, valid software timer service and in one-shot mode. */
    if ((result = TimerCreate("TimerA_TIM135", TIMER_ONESHOT, MyHandler, NULL, 
                    &myTimerA)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Create a software timerB with valid name, valid software timer service and in one-shot mode. */
    if ((result = TimerCreate("TimerB_TIM135", TIMER_ONESHOT, MyTimerHandler, NULL, 
                    &myTimerB)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }
    /* Create a software timerC with valid name, valid software timer service and in periodical mode. */
    if ((result = TimerCreate("TimerC_TIM135", TIMER_PERIODIC, MyTimerHandler, NULL, 
                    &myTimerC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    return (SUCCESS);
}

/**
 * TestDo - Performing test
 *
 * DESCRIPTION
 *  Test case to exercise if NEOS-178S cannot invoke the below methods(Interrupt, Timer and 
 *  HealthMonitor) in the timer service routine.
 *   - Method to attach an interrupt service routine
 *   - Method to create a software timer
 *   - Method to start a software timer
 *   - Method to stop a software timer
 *   - Method to get an ID using a name of software timer
 *   - Method to create and start the error handler thread
 *   - Method to provide an error from error status queue to the error handler thread
 *   - Method to raise an error to the error handler thread
 *   - Method to register an exception handler.
 *   - Method to unregister an exception handler.
 *
 * PARAMETERS
 *  N/A
 * 
 * TRACEABILITY
 *  [TP_SRD_TIM_0135 -> TC_SRD_TIM_0135]
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
    for (count = 0; count < 10; count++)
    {
        flags[count] = FLAG_CLEAR;
    }
    
    /* start myTimerA after 0.1 sec */
    if ((result = TimerStart(myTimerA, 100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* start myTimerC after 0.2 sec */
    if ((result = TimerStart(myTimerC, 200*TEST_MSEC)) != SUCCESS)
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
    
    TEST_SUCCESS (SUCCESS);
}
