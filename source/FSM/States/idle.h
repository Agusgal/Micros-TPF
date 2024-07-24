/***************************************************************************/ /**
  @file     Idle.h
  @brief    Idle state
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef IDLE_H
#define IDLE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initializes the idle state
 */
void Idle_InitState(void);

/**
 * @brief Stop showing the time when the user interacts with the system.
 */
void Idle_OnUserInteraction(void);

void UpdateTime(void);

#endif /* IDLE_H */
