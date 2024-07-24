/***************************************************************************/ /**
  @file     audioPlayer_state.h
  @brief    Audio Reproductor STATE using the DAC with PDB.
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include <FSM/States/audioPlayer_state.h>
#include <stdbool.h>
#include <string.h>

#include "../../EventQueue/queue.h"
#include "../../mp3_handler/mp3_handler.h"
#include "../../mp3_file_handler/mp3_file_handler.h"

#include "OLEDdisplay.h"


#include "AudioPlayer.h"
#include "ff.h"
#include "Timer.h"


#define VOLUME_TIME		(3000U)
/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool showingVolume = false;
static int volumeTimerID = -1;

/*******************************************************************************
 * 	LOCAL FUNCTION DEFINITIONS
 *******************************************************************************/


static void printFileInfo(void);
static void showVolume(void);
static void stopShowingVolume(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void Player_InitState(void)
{
	printFileInfo();
}


void Player_ToggleMusic(void)
{
	mp3Handler_toggle();
}


void Player_Stop(void)
{
	mp3Handler_stop();
}


void Player_PlayNextSong(void)
{
	mp3Handler_playNextSong();
	printFileInfo();
}


void Player_PlayPreviousSong(void)
{
	mp3Handler_playPreviousSong();
	printFileInfo();
}


void Player_IncVolume(void)
{
	mp3Handler_IncVolume();
	showVolume();
}


void Player_DecVolume(void)
{
	mp3Handler_DecVolume();
	showVolume();
}


void Player_MP3_UpdateAll(void)
{
	mp3Handler_updateAll();
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void printFileInfo(void)
{
	char path[150], gather[400];

	memset(path, 0x20, 150);
	memset(gather, 0x0, 400);

	path[149] = 0;


	char * name = mp3Handler_getTitle();
	char * artist = mp3Handler_getArtist();
	char * album = mp3Handler_getAlbum();
	char * year = mp3Handler_getYear();


	strcat(gather, "Song: ");
	strcat(gather, name);
	strcat(gather, " Artist: ");
	strcat(gather, artist);
	strcat(gather, " Album: ");
	strcat(gather, album);
	strcat(gather, " Year: ");
	strcat(gather,  year);


	memcpy(path, name, strlen(name));

	OLED_Clear();
	OLED_write_Text(10, 16, (char*)path);

	OLED_write_Text(10, 32, gather);
}


static void showVolume(void)
{
	if(!showingVolume)
	{
		OLED_Clear();
		volumeTimerID = Timer_AddCallback(stopShowingVolume, VOLUME_TIME, true);
	}
	else
	{
		Timer_Reset(volumeTimerID);
	}

	char str2wrt[11] = "Volumen: --";

	char vol = mp3Handler_getVolume();

	str2wrt[9] = vol/10 != 0? 0x30 + vol/10 : ' ';
	str2wrt[10] = 0x30 + (char)vol%10;

	OLED_Clear();
	OLED_Refresh();
	OLED_write_Text(20, 22, (char*)str2wrt);
	showingVolume = true;
}


static void stopShowingVolume(void)
{
	showingVolume = false;
	printFileInfo();
}
