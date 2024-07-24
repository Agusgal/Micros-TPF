/***************************************************************************/ /**
  @file     equalizer_state.c
  @brief    equalizer state functions.
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <FSM/States/equalizer_state.h>
#include "../../mp3_handler/mp3_handler.h"
#include "equalizer.h"

#include "../../EventQueue/queue.h"
#include "Timer.h"
#include "OLEDdisplay.h"

#include <stdio.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TITLE_TIME 2000
#define OPTIONS_COUNT 6
#define OPTION_VALUES_ARRAY_SIZE	NUMBER_OF_BANDS
#define MAX_BAND_GAIN	MAX_GAIN
#define	MIN_BAND_GAIN	-MAX_GAIN


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
	DEFAULT,
	ROCK,
	JAZZ,
	POP,
	CLASSIC,
	CUSTOM
} options_t;

/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool showingTitle;
static int titleTimerID = -1;
static uint8_t currentOptionIndex = 0;
static char * frequencyBandsTitles [] = {"80Hz Band",
										 "160Hz Band",
										 "320Hz Band",
										 "640Hz Band",
										 "1.28kHz Band",
										 "2.5kHz Band",
										 "5kHz Band",
										 "10kHz Band"};
static bool settingCustom = false;
static uint8_t currentBand = 0;
static int32_t currentBandValue = 0;


int optionValues[5][OPTION_VALUES_ARRAY_SIZE] =
	{{0, 0, 0, 0,  0, 0, 0, 0}, 	//default
	 {-2,-2, 1, 1, 1, 5, 5, 5}, 	//rock
	 {2, 2, 2, -6, -6,-6, 4, 4},	//jazz
	 {5, 5, 0, 0,  2, 2, 5, 5},		//pop
	 {4, 4,-6,-6, 1, 1, 3, 3}		//classic
};


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Show the title of the state on the display. If the user interacts with the system, the title will stop showing.
 */
static void showTitle(void);
/**
 * @brief Stops showing the title of the state on the display.
 */
static void stopShowingTitle(void);

/**
 * @brief Stops showing the title of the state on the display due to a user's interaction.
 */
static void userInteractionStopsTitle(void);

/**
 * @brief Shows the current option in the display.
 */
static void setCurrentOption(void);
/**
 * @brief Shows the current band and its gain in the display.
 */
static void showCustomBandSetting(void);
/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/

void Effects_InitState(void)
{
	showTitle();
	settingCustom = false;
	currentOptionIndex = 0;
}

void Effects_NextOption(void)
{
	if (showingTitle)
	{
		userInteractionStopsTitle();
	}
    else if (settingCustom)
    {
    	currentBandValue += 1;
    	if (currentBandValue == MAX_BAND_GAIN +1)
    	{
    		currentBandValue = MIN_BAND_GAIN;
    	}
    	EQ_Set_Band_Gain(currentBand + 1, currentBandValue);
    	showCustomBandSetting();
    }
    else
    {
        uint8_t max = OPTIONS_COUNT - 1;
        if (currentOptionIndex == max)
        {
        	currentOptionIndex = 0;
        }
        else
        {
        	currentOptionIndex++;
        }
        setCurrentOption();
    }
}

void Effects_PreviousOption(void)
{

	if (showingTitle)
	{
		userInteractionStopsTitle();
	}
    else if(settingCustom)
    {
    	currentBandValue -=1;
		if (currentBandValue == MIN_BAND_GAIN -1)
		{
			currentBandValue = MAX_BAND_GAIN;
		}
		EQ_Set_Band_Gain(currentBand+1, currentBandValue);
		showCustomBandSetting();
    }
    else
    {
        uint8_t max = OPTIONS_COUNT - 1;
        if (currentOptionIndex == 0)
        {
        	currentOptionIndex = max - 1;
        }
        else
        {
        	currentOptionIndex--;
        }
        setCurrentOption();
    }
}

void Effects_SelectOption(void)
{
	if (showingTitle)
	{
		userInteractionStopsTitle();
	}
    else if(settingCustom)
    {
    	if (currentBand == OPTION_VALUES_ARRAY_SIZE - 1)
    	{
    		currentBand = 0;
    		Effects_Back();
    	}
    	else
    	{
    		currentBand += 1;
    		currentBandValue = EQ_Get_Band_Gain(currentBand + 1);
    		showCustomBandSetting();
    	}
    }
    else
    {
    	OLED_Clear();
        if (currentOptionIndex == OPTIONS_COUNT - 1)
        {
        	settingCustom = true;
        	currentBand = 0;
        	currentBandValue = EQ_Get_Band_Gain(currentBand + 1);
        	showCustomBandSetting();
        }
        else
        {
        	for (int i = 0; i < OPTION_VALUES_ARRAY_SIZE; i++)
        	{
        		EQ_Set_Band_Gain(i + 1, optionValues[currentOptionIndex][i]);
        	}
        	push_Queue_Element(CHANGE_MODE_EV);
        }
    }
}


void Effects_Back(void)
{
	if (settingCustom)
	{
		settingCustom = false;
		setCurrentOption();
	}
	else
	{
		push_Queue_Element(CHANGE_MODE_EV);
	}
}


void Equalizer_MP3_UpdateAll(void)
{
	mp3Handler_updateAll();
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void showTitle(void)
{
	OLED_Clear();
	OLED_write_Text(22, 15, "Efectos");

	showingTitle = true;
	titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);
}


static void stopShowingTitle(void)
{
	showingTitle = false;
	OLED_Clear();
	setCurrentOption();
}


static void userInteractionStopsTitle(void)
{
	Timer_Delete(titleTimerID);
	titleTimerID = -1;
	stopShowingTitle();
}


static void showCustomBandSetting(void)
{
	OLED_Clear();
	OLED_write_Text(22, 16, frequencyBandsTitles[currentBand]);

	char bandGainText[16] = "";
	int writtenChars = sprintf(bandGainText, "%ddB", currentBandValue);
	bandGainText[writtenChars] = ' ';

	OLED_write_Text(22, 32, bandGainText);
}


static void setCurrentOption(void)
{
    OLED_Clear();

    switch (currentOptionIndex)
    {
    case DEFAULT:
    	OLED_write_Text(22, 22, "DEFAULT");
		break;
    case ROCK:
    	OLED_write_Text(22, 22, "ROCK");
        break;
    case JAZZ:
    	OLED_write_Text(22, 22, "JAZZ");
        break;
    case POP:
    	OLED_write_Text(22, 22, "POP");
        break;
    case CLASSIC:
    	OLED_write_Text(22, 22, "CLASSIC");
		break;
    case CUSTOM:
    	OLED_write_Text(22, 22, "CUSTOM");
		break;
    default:
    	break;
    }
}

