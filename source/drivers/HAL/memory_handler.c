/***************************************************************************/ /**
  @file     memory_handler.h
  @brief    Memory Handler header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>

#include "fsl_port.h"
#include "component/SD/fsl_sd.h"
#include "fsl_sd_disk.h"

#include "ff.h"
#include "diskio.h"
#include "component/SD/sdmmc_config.h"

#include "memory_handler.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum SD_state{IDLE = 0, CONNECTED, DISCONNECTED};

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool SD_connected = false;
static bool SD_error = false;
static bool SD_init_done = false;
static FATFS g_fileSystem;
static uint8_t SD_status = 0;
static const TCHAR driveBuffer[] = {SDDISK + '0', ':', '/'}; 	// Path name

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void memory_handler_insertCB(bool isInserted, void *userData);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void memory_handler_init()
{
	SYSMPU->CESR &= ~SYSMPU_CESR_VLD_MASK;

	// Initializes SD pins and detection callback
	BOARD_SD_Config(&g_sd,  memory_handler_insertCB, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);

	// Initializes Host
	if (SD_HostInit(&g_sd) != kStatus_Success)
	{
		SD_error = true;
	}

	SD_init_done = true;

}


bool mh_is_SD_connected(void)
{
	return SD_connected;
}

bool mh_SD_connected(void)
{
	bool check = SD_status == CONNECTED;
	if(check)
		SD_status = IDLE;
	return check;
}


bool mh_SD_disconnected(void)
{
	bool check = SD_status == DISCONNECTED;
	if(check)
		SD_status = IDLE;
	return check;
}


void mh_SD_mount(void)
{
	FRESULT error;

	if(!SD_init_done)
	{
		SD_error = true;
		return;
	}

	// Mount SD
	if (f_mount(&g_fileSystem, driveBuffer, 1))
	{
		SD_error = true;
		return;
	}

	// Set current Drive
    error = f_chdrive((char const *)&driveBuffer[0]);
	if (error)
	{
		SD_error = true;
		return;
	}

}


void mh_SD_disconnect(void)
{
	f_mount(NULL, driveBuffer, 1U);
	g_sd.initReady = false;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void memory_handler_insertCB(bool isInserted, void *userData)
{
	// If the previous state matches the actual state, there is an error
	if(SD_connected == isInserted)
	{
		SD_error = true;
	}
	// If there is no error and the SD card is connected
	else if(isInserted)
	{
		SD_status = CONNECTED;
	}
	else
	{
		SD_status = DISCONNECTED;
	}

	SD_connected = isInserted;
}
