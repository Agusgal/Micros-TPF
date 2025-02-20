/***************************************************************************/ /**
  @file     matrix display.h
  @brief    8x8 RGB display hearder
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef MATRIX_DISPLAY_H_
#define MATRIX_DISPLAY_H_

/******************************************************************************
 * 		INCLUDES HEADER FILES
 ****************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/***************************************************************************
*	CONSTANT AND MACRO DEFINITIONS
****************************************************************************/
#define DISPLAY_SIZE 64

/***************************************************************************
*	STRUCTS DEFINITIONS
****************************************************************************/
typedef enum { RED, YELLOW, GREEN, CLEAN}colors_t;
typedef struct
{
  bool R;
  bool G;
  bool B;
} pixel_t;


/***************************************************************************
*	GLOBAL FUNCTION DECLARATIONS
****************************************************************************/
/*
 * @brief: Initialize the matrix, preparing FTM0 channel 0 and DMA0 channel 1
 */
void md_Init(void);


/*
 * @brief: Update the matrix buffer, it will be updated in the next dma callback, upto 4ms leter
 * @param: buffer, new buffer to write MUST be DISPLAY_SIZE long
 */
void md_writeBuffer(colors_t *buffer);

/*
 * @brief: Create a pixel_t with the given conditions
 * @param: Each pixel presence or not
 * @return: The structed data
 */
pixel_t md_makeColor(bool R, bool G, bool B);

#endif /* MATRIX_DISPLAY_H_ */
 
