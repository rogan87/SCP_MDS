/*
 * TP_SRD_TIM_0001.c - High-level test procedure for Time Management
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

static int    TestSetPrecondition(void);
static int    TestDo(void);
static void   TestCleanUp(void);

TEST_ID TP_SRD_TIM_0001_ID =
{
    __FILE__,
    "$Id$",
    "$Revision$",
    TestSetPrecondition,
    TestDo,
    TestCleanUp        
};

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
 * TestSetPrecondition - setting preconditon
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
 *  This routine returns SUCCESS.
 *
 * ERRORS
 *  N/A
 */

static int 
TestSetPrecondition(void)
{
    /* Start RTC (Real Time Clock) in the system */
    TFXSetRTC(0);
    
    return (SUCCESS);
}

/**
 * TestDo - performing test
 *
 * DESCRIPTION
 *  Test code to exercise if NEOS-178S provides the system time since 
 *  the system is powered ON.
 *
 * PARAMETERS
 *  N/A
 *
 * TRACEABILITY
 *  [TP_SRD_TIM_0001 -> TC_SRD_TIM_0001]
 *
 * RETURNS
 *  This routine returns SUCCESS to indicate this function is passed,
 *  or it returns error code or FAILURE to indicate testing failure.
 *
 * ERRORS
 *  N/A
 */

static int
TestDo(void)
{
    TimeValue   myTimeValue[2];
    SInt64      timeDifference;
    SInt64      RTCDifference;
    int         result;
    UInt64      rtcSecond[2];

    if ((result = ThreadDelay(100*TEST_MSEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Get the system time elapsed since system is powered ON.  */
    myTimeValue[0] = SystemClockGetTime();

    /* Get real time clock */
    rtcSecond[0] = TFXGetRTC();

    /* Check the system time */ 
    if (myTimeValue[0] == 0)   
    {
        TEST_FAILURE (FAILURE);
    }   

    /* Delay execution of this thread for 1 second */
    if ((result = ThreadDelay(1*TEST_SEC)) != SUCCESS)
    {
        TEST_FAILURE (result);
    }

    /* Get the system time */
    myTimeValue[1] = SystemClockGetTime();

    /* Get real time clock */
    rtcSecond[1] = TFXGetRTC();

    /* Check the difference between the time values (TIME2 - TIME1) */
    timeDifference = myTimeValue[1] - myTimeValue[0];

    /* Check the difference between the time values (RTC2- RTC1) */
    RTCDifference = rtcSecond[1] - rtcSecond[0];

    /* Check if the time value (TIME2 - TIME1) is same as (RTC2 - RTC1). */
    if ((timeDifference < RTCDifference - TFX_TIME_MARGIN) ||
    	(timeDifference > RTCDifference + TFX_TIME_MARGIN))
    {
        TEST_FAILURE (FAILURE);
    }
   
    TEST_SUCCESS (SUCCESS);
}
