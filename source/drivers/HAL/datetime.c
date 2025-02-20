/***************************************************************************/ /**
  @file     time_service.c
  @brief    Time Service Functions
  @author   Grupo 5 - Lab. de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "clock_config.h"
#include "fsl_rtc.h"
#include "datetime.h"
#include "Timer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
 * DEFINITIONS
 ******************************************************************************/
#define EXAMPLE_OSC_WAIT_TIME_MS 1000UL

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


#if !(defined(FSL_FEATURE_RTC_HAS_NO_CR_OSCE) && FSL_FEATURE_RTC_HAS_NO_CR_OSCE)
/* Wait for 32kHz OSC clock start up. */
static void EXAMPLE_WaitOSCReady(uint32_t delay_ms);
#endif

static void DateTime_FinishInit(void);

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


static rtc_datetime_t date;
void (*rtc_callback)(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*!
 * @brief Main function
 */
void DateTime_Init(void (*callback)(void))
{

	/* Set a start date time */
	date = (rtc_datetime_t) {.year = 2024U,
			.month = 2U,
			.day = 5U,
			.hour = 20U,
			.minute = 20U,
			.second = 0};

    rtc_config_t rtcConfig;

    RTC_GetDefaultConfig(&rtcConfig);
    RTC_Init(RTC, &rtcConfig);

    /* If the oscillator has not been enabled. */
    if (0U == (RTC->CR & RTC_CR_OSCE_MASK))
    {
        /* Select RTC clock source */
        RTC_SetClockSource(RTC);

        /* Wait for OSC clock steady. */
        Timer_AddCallback(DateTime_FinishInit, 1000, true);
    }
    else
    {
    	DateTime_FinishInit();
    }
    rtc_callback = callback;
}


DateTimeDate_t DateTime_GetCurrentDateTime(void)
{
	/* Get date time */

	RTC_GetDatetime(RTC, &date);
	DateTimeDate_t newDate =  {.year = date.year,
		.month = date.month,
		.day = date.day,
		.hour = date.hour,
		.minute = date.minute,
		.second = date.second};

	return newDate;
}

void DateTime_Enable(void)
{

    /* Enable RTC second interrupt */
    RTC_EnableInterrupts(RTC, kRTC_SecondsInterruptEnable);

}

void DateTime_Disable(void)
{
	/* Disable RTC second interrupt */
	RTC_DisableInterrupts(RTC, kRTC_SecondsInterruptEnable);
}

/*!
 * @brief ISR for Alarm interrupt
 *
 * This function changes the state of busyWait.
 */
#include "gpio.h"
#include "board.h"

void RTC_Seconds_IRQHandler(void)
{
	//gpioWrite(TP, true);

    if (rtc_callback != NULL)
    	rtc_callback();

    //gpioWrite(TP, false);

    SDK_ISR_EXIT_BARRIER;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void DateTime_FinishInit(void)
{

    /* RTC time counter has to be stopped before setting the date & time in the TSR register */
    RTC_StopTimer(RTC);

    /* Set RTC time to default */
    RTC_SetDatetime(RTC, &date);

    /* Disable Interrupts */
    DateTime_Disable();

    /* Enable at the NVIC */
    EnableIRQ(RTC_Seconds_IRQn);

    /* Start the RTC time counter */
    RTC_StartTimer(RTC);
}


#if !(defined(FSL_FEATURE_RTC_HAS_NO_CR_OSCE) && FSL_FEATURE_RTC_HAS_NO_CR_OSCE)
/*!
 * @brief Waitting for the OSC clock steady.
 *
 * Due to the oscillator startup time is depending on the hardware design and usually
 * take hundreds of milliseconds to make the oscillator stable. Here, we just delay a certain
 * time to ensure the oscillator stable, please use the suitable delay time to adapt
 * to your real usage if needed.
 */
static void EXAMPLE_WaitOSCReady(uint32_t delay_ms)
{
    uint32_t ticks = 0UL;
    uint32_t count = delay_ms;

    /* Make a 1 milliseconds counter. */
    ticks = SystemCoreClock / 1000UL;
    assert((ticks - 1UL) <= SysTick_LOAD_RELOAD_Msk);

    /* Enable the SysTick for counting. */
    SysTick->LOAD = (uint32_t)(ticks - 1UL);
    SysTick->VAL  = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (; count > 0U; count--)
    {
        /* Wait for the SysTick counter reach 0. */
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
        {
        }
    }

    /* Disable SysTick. */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->LOAD = 0UL;
    SysTick->VAL  = 0UL;
}
#endif
