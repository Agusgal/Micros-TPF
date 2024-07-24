/***************************************************************************//**
 @file     AudioPlayer.c
 @brief    Audio Reproductor using the DAC with PDB.
 @author   Grupo 5
******************************************************************************/


/*
 * Based on the SDK example of DAC with PDB:
 *
 * 	See pdb_dac_trigger.c and PDB For Kinetis K Series MCUs from Freescale
 *
 * 	In this file, PDB triggers the DAC. When the DAC buffer of 16 bytes empties,
 * 	a DMA request is generated, loading the next 16 bytes of the activeBuffer to the
 * 	DAC buffer.
 *
 * 	When a DMA major loop of the 16 bytes ends, a DMA interruption is called, which
 * 	changes the source address of the next 16 bytes to the next 16 bytes of the activeBuffer.
 *
 * 	In this way, when the DAC buffer empties again, the new 16 bytes will be loaded.
 *
 * 	When the activeBuffer reaches its end, the backBuffer is loaded as the new activeBuffer.
 * 	A variable that indicates that the backBuffer is free is then set true, and a function
 * 	that retreives this value is to be used to indicate that the backBuffer should be loaded with new
 * 	data.
 *
 *	We are counting on that the DMA interrupt subroutine that updates the source adress and the
 *	new DMA configuration will last less than the time that the DAC takes to transfer all its 16
 *	bytes.
 *
 *	Maybe the use of PIT and DMA would have been more efficient, as almost no intervention
 *	of the CPU would be needed to update the DMA.
 *
 *
 * 	*/

 /*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "AudioPlayer.h"
#include "fsl_dac.h"
#include "fsl_edma.h"
#include "fsl_pdb.h"
#include "fsl_dmamux.h"
#include "fsl_common.h"


#include "ftm.h"
#include "board.h"
#include "MK64F12.h"



/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define PDB_BASEADDR           PDB0
#define PDB_MODULUS_VALUE      0xFFFU
#define PDB_DELAY_VALUE        0U
#define PDB_DAC_CHANNEL        kPDB_DACTriggerChannel0
#define PDB_DAC_INTERVAL_VALUE 0xFFFU
#define DAC_BASEADDR           DAC0
#define DMAMUX_BASEADDR        DMAMUX
#define DMA_CHANNEL            1U
#define DMA_DAC_SOURCE         45U
#define DMA_BASEADDR           DMA0
#define DAC_DATA_REG_ADDR      0x400cc000U
#define DMA_IRQ_ID             DMA0_IRQn

#define DMA_CHANEL (0U)

/*******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
/*!
 * @brief Initialize the EDMA.
 */
static void EDMA_Configuration(void);


/*!
 * @brief Initialize the DMAMUX.
 */
static void DMAMUX_Configuration(void);


/*!
 * @brief Initialize the PDB.
 */
static void PDB_Configuration(void);


/*!
 * @brief Initialize the DAC.
 */
static void DAC_Configuration(void);


/*!
 * @brief Callback function for EDMA.
 */
static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);


/*!
 * @brief Performs the division between two unsigned ints, and returns the rounded result
 */
static uint32_t roundedDivide(uint32_t dividend, uint32_t divisor);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static edma_handle_t audioPlayer_EDMA_Handle;                             /* Edma handler */
static edma_transfer_config_t audioPlayer_transferConfig;                 /* Edma transfer config. */
static volatile uint32_t sampleIndex = 0; 							/* Index of the g_dacDataArray array. */

// Buffers
static int16_t buffers[2][AUDIO_PLAYER_BUFF_SIZE];
static int16_t * activeBuffer = buffers[0];					// Active has the current playing sound
static int16_t * backBuffer= buffers[1];					// Back has the next frame of sound to be played

static uint32_t backBufferSampleRate;
static uint32_t activeBufferSize = AUDIO_PLAYER_BUFF_SIZE;
static uint32_t nextBufferSize = AUDIO_PLAYER_BUFF_SIZE;

static bool backBufferFree = false;
static bool pause = false;
static bool stop = true;

static int16_t muteAudioBuffer[DAC_DATL_COUNT] = {DAC_ZERO_VOLT_VALUE};


/*******************************************************************************
*******************************************************************************
                       GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
void AudioPlayer_Init(void)
{
	for(uint8_t i = 0; i < DAC_DATL_COUNT; i++)
	{
		muteAudioBuffer[i] = DAC_ZERO_VOLT_VALUE;
	}

	// Initialize DMAMUX.
	DMAMUX_Configuration();

	// Initialize EDMA.
	EDMA_Configuration();

	// Initialize the HW trigger source.
	PDB_Configuration();

	// Initialize DAC.
	DAC_Configuration();
}


void AudioPlayer_LoadSong(uint16_t * firstSongFrame, uint16_t _sampleRate, uint32_t _activeBufferSize)
{
	memcpy(activeBuffer, firstSongFrame, AUDIO_PLAYER_BUFF_SIZE * sizeof(uint16_t));

	backBufferFree = true;
	sampleIndex = 0;

	backBufferSampleRate = _sampleRate;

	activeBufferSize = _activeBufferSize;

	AudioPlayer_UpdateSampleRate(backBufferSampleRate);
}


void AudioPlayer_setSampleRate(uint32_t _sampleRate)
{
	backBufferSampleRate = _sampleRate;
}


void AudioPlayer_UpdateSampleRate(uint32_t _sampleRate)
{
	// PDB_Configuration
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;
    PDB_GetDefaultConfig(&pdbConfigStruct);

	pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor1;
	pdbConfigStruct.prescalerDivider = kPDB_PrescalerDivider1;

	// Perform the required division
	PDB_SetModulusValue(PDB_BASEADDR, roundedDivide(60000000UL, _sampleRate));
	PDB_SetDACTriggerIntervalValue(PDB_BASEADDR, PDB_DAC_CHANNEL, roundedDivide(60000000UL, _sampleRate));

	pdbConfigStruct.enableContinuousMode = true;

	PDB_Init(PDB_BASEADDR, &pdbConfigStruct);
	PDB_EnableInterrupts(PDB_BASEADDR, kPDB_DelayInterruptEnable);
	PDB_SetCounterDelayValue(PDB_BASEADDR, PDB_DELAY_VALUE);

    // Set DAC trigger
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger      = true;
    PDB_SetDACTriggerConfig(PDB_BASEADDR, PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);

    // Load PDB values
    PDB_DoLoadValues(PDB_BASEADDR);
}


audioPlayerError_t AudioPlayer_UpdateBackBuffer(uint16_t * newBackBuffer, uint32_t _sampleRate, uint32_t _nextBufferSize)
{
	if(backBufferFree)
	{
		memcpy(backBuffer, newBackBuffer, AUDIO_PLAYER_BUFF_SIZE * sizeof(uint16_t));
		backBufferFree = false;

		// Update the backBuffer
		backBufferSampleRate = _sampleRate;

		nextBufferSize = _nextBufferSize;
		return AP_NO_ERROR;
	}
	else
	{
		return AP_ERROR_BB_NOT_FREE;
	}
}


bool AudioPlayer_IsBackBufferFree(void)
{
	return backBufferFree;
}


void AudioPlayer_Play(void)
{
	sampleIndex = 0;
	stop = false;

	if(pause)
	{
		pause = false;
		return;
	}

	// DMAMUX:
    DMAMUX_EnableChannel(DMAMUX_BASEADDR, DMA_CHANNEL);

	// EDMA:
    edma_transfer_config_t * config = &audioPlayer_transferConfig;

    // Source
    void *srcAddr = (void *)(activeBuffer + sampleIndex);
    uint32_t srcWidth = sizeof(uint16_t);

    // Address
    void *destAddr = (void *)DAC_DATA_REG_ADDR;
    uint32_t destWidth =  sizeof(uint16_t);
    uint32_t bytesEachRequest = DAC_DATL_COUNT * sizeof(uint16_t);
    uint32_t transferBytes = DAC_DATL_COUNT * sizeof(uint16_t);
    edma_transfer_type_t type = kEDMA_MemoryToMemory;

    EDMA_PrepareTransfer(config,
						  srcAddr,
						  srcWidth,
						  destAddr,
						  destWidth,
						  bytesEachRequest,
						  transferBytes,
						  type);

	EDMA_SubmitTransfer(&audioPlayer_EDMA_Handle, &audioPlayer_transferConfig);

	// Enable interrupt when transfer is done.
	EDMA_EnableChannelInterrupts(DMA_BASEADDR, DMA_CHANNEL, kEDMA_MajorInterruptEnable);

	// Enable transfer.
	EDMA_StartTransfer(&audioPlayer_EDMA_Handle);

	// DAC:
	// Enable DMA.

	/* At this point, the DMA is configured so that when the pointer to the internal buffer
	 * of the DAC (16 bytes) generate a DMA request. In this way, the new 16 bytes are loaded
	 * to the buffer. When the transfer is complete, the source address is updated in the
	 * DMA major loop interrupt
	 */
	DAC_EnableBufferInterrupts(DAC_BASEADDR, kDAC_BufferReadPointerTopInterruptEnable);
	DAC_EnableBufferDMA(DAC_BASEADDR, true);

	PDB_DoSoftwareTrigger(PDB_BASEADDR);
}


void AudioPlayer_Pause(void)
{
	pause = true;
}

void AudioPlayer_Stop(void)
{
	stop = true;
}

/*******************************************************************************
*******************************************************************************
                       LOCAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
static void EDMA_Configuration(void)
{
	EDMA_CreateHandle(&audioPlayer_EDMA_Handle, DMA_BASEADDR, DMA_CHANNEL);
    EDMA_SetCallback(&audioPlayer_EDMA_Handle, Edma_Callback, NULL);
}


static void DMAMUX_Configuration(void)
{
    // Configure DMAMUX
    DMAMUX_SetSource(DMAMUX_BASEADDR, DMA_CHANNEL, DMA_DAC_SOURCE);
}


static void PDB_Configuration(void)
{
	// Enable the trigger source of PDB.
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;

    PDB_GetDefaultConfig(&pdbConfigStruct);

    pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor1;
    pdbConfigStruct.enableContinuousMode = true;

    PDB_Init(PDB_BASEADDR, &pdbConfigStruct);
    PDB_EnableInterrupts(PDB_BASEADDR, kPDB_DelayInterruptEnable);
    PDB_SetModulusValue(PDB_BASEADDR, PDB_MODULUS_VALUE);
    PDB_SetCounterDelayValue(PDB_BASEADDR, PDB_DELAY_VALUE);

    // Set DAC trigger.
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger = true;
    PDB_SetDACTriggerConfig(PDB_BASEADDR, PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);
    PDB_SetDACTriggerIntervalValue(PDB_BASEADDR, PDB_DAC_CHANNEL, PDB_DAC_INTERVAL_VALUE);

    // Load PDB values.
    PDB_DoLoadValues(PDB_BASEADDR);
}


static void DAC_Configuration(void)
{
    dac_config_t dacConfigStruct;
    dac_buffer_config_t dacBufferConfigStruct;

    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DAC_BASEADDR, true);

    // Configure the DAC buffer.
    DAC_EnableBuffer(DAC_BASEADDR, true);
    DAC_GetDefaultBufferConfig(&dacBufferConfigStruct);
    dacBufferConfigStruct.triggerMode = kDAC_BufferTriggerByHardwareMode;
    DAC_SetBufferConfig(DAC_BASEADDR, &dacBufferConfigStruct);

    // Make sure the read pointer to the start.
    DAC_SetBufferReadPointer(DAC_BASEADDR, 0U);
}


static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
	//gpioWrite(TP, true);
    // Clear Edma interrupt flag.
    EDMA_ClearChannelStatusFlags(DMA_BASEADDR, DMA_CHANNEL, kEDMA_InterruptFlag);

    // Setup transfer
    void * srcAdd = NULL;

    if(pause || stop)
	{
		srcAdd = muteAudioBuffer;
	}

    else
    {
    	sampleIndex += DAC_DATL_COUNT;

    	// This should allow playing any sampleRate (MPEG2 and MPEG3 have different MP3_FRAME_SIZES)
    	if (sampleIndex >= activeBufferSize)
		{
    		// If the activeBuffer was completely transferred
			sampleIndex = 0;

			// Load backBuffer as new activeBuffer
			// and clear backBuffer
			void * temp = activeBuffer;
			activeBuffer = backBuffer;
			backBuffer = temp;
			backBufferFree = true;

			activeBufferSize = nextBufferSize;

			// Update the sampleRate with the one loaded in the backBuffer
			AudioPlayer_UpdateSampleRate(backBufferSampleRate);
		}

		srcAdd = (activeBuffer + sampleIndex);
    }

	// EDMA:
    edma_transfer_config_t * config = &audioPlayer_transferConfig;

    // Source
    uint32_t srcWidth = sizeof(uint16_t);

    // Address
    void *destAddr = (void *)DAC_DATA_REG_ADDR;
    uint32_t destWidth =  sizeof(uint16_t);
    uint32_t bytesEachRequest = DAC_DATL_COUNT * sizeof(uint16_t);
    uint32_t transferBytes = DAC_DATL_COUNT * sizeof(uint16_t);
    edma_transfer_type_t type = kEDMA_MemoryToMemory;

    EDMA_PrepareTransfer(config,
						  (void*) srcAdd,
						  srcWidth,
						  destAddr,
						  destWidth,
						  bytesEachRequest,
						  transferBytes,
						  type);

    EDMA_SetTransferConfig(DMA_BASEADDR, DMA_CHANNEL, &audioPlayer_transferConfig, NULL);

    // Enable transfer.
    EDMA_StartTransfer(&audioPlayer_EDMA_Handle);

    //gpioWrite(TP, false);
}


static uint32_t roundedDivide(uint32_t dividend, uint32_t divisor)
{
    // Ensure divisor is not zero to avoid division by zero
    if (divisor == 0)
    {
        return 0;
    }

    // Perform the division
    uint32_t quotient = dividend / divisor;
    uint32_t remainder = dividend % divisor;

    // Calculate the rounded result
    if (2 * remainder >= divisor)
    {
        // Round up
        quotient += 1;
    }

    return quotient;
}
