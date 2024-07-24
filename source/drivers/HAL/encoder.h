/***************************************************************************//**
  @file     encoder.h
  @brief    Encoder driver Header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef _ENCODER_H_
#define _ENCODER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum encoderStates {ACW1=1, ACW2, ACW3, ACW4, CW1, CW2, CW3, CW4};
enum encoderOutPuts {NO_MOVE, RIGHT_TURN,LEFT_TURN};
enum encoderSwitchOutPuts {RELEASED, PRESSED, RISING_FLANK, FIVE_SEC_PRESS, IDLE_, FIVE_SEC_PRESSING};


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void Encoder_Init(void);

void Encoder_Update(void);
void EncoderSwitch_Update(void);

int getEncoderSwitch_State(void);
int getEncoder_State(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _ENCODER_H_
