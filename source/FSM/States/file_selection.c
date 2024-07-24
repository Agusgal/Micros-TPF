/***************************************************************************/ /**
  @file     file_selection.c
  @brief    File Selection State Functions
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <FSM/States/file_selection.h>
#include <stdbool.h>
#include <string.h>

#include "../../mp3_handler/mp3_handler.h"

#include "../../EventQueue/queue.h"


#include "Timer.h"
#include "OLEDdisplay.h"
#include "power_mode_switch.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/**
 * @brief Show the title of the state on the display. If the user interacts with the system, the title will stop showing.
 */
static void showTitle(void);



/**
 * @brief Prints the file's name and artist
 */
static void printFileInfo(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void FileSelection_InitState(void)
{
	showTitle();

	mp3Handler_init();

	printFileInfo();
}

void FileSelection_ReturnState(void)
{
	showTitle();

	printFileInfo();
}

void FileSelection_NextFile(void)
{
	mp3Handler_nextObject();
	printFileInfo();
}

void FileSelection_PreviousFile(void)
{
	mp3Handler_prevObject();
	printFileInfo();
}

void FileSelection_SelectFile(void)
{
	// If the file is an MP3 File
	if(mp3Handler_selectObject())
	{
		/* Start decoding the file and play the audio player */
		mp3Handler_play();
		push_Queue_Element(FILE_SELECTED_EV);
		return;
	}
	else
	{
		printFileInfo();
	}
}


void FileSelection_PlayNextSong(void)
{
	mp3Handler_playNextSong();
}


void FileSelection_PlayPrevSong(void)
{
	mp3Handler_playPreviousSong();
}


void FileSelection_MP3_UpdateAll(void)
{
	mp3Handler_updateAll();
}


/*******************************************************************************
 *******************************************************************************
 *                         LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void showTitle(void)
{
	OLED_Clear();
	OLED_Refresh();
	OLED_write_Text(10, 16, "Choose file:");
}


static void printFileInfo(void)
{
	OLED_Clear();
	OLED_write_Text(10, 16, "Choose file:");

	//Get file name from audio module
	char * name = mp3Handler_getCurrentName();

	OLED_write_Text(10, 32, (char*)name);
}
