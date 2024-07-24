/***************************************************************************/ /**
  @file     memory_handler.h
  @brief    Memory Handler header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/


#ifndef MEMORY_HANDLER_H_
#define MEMORY_HANDLER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
#define SD_BUFFER_ALIGN_SIZE 	(4U)

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*
 * @brief Init routine
 */
void memory_handler_init(void);

/*!
 * @brief Checks if SD is connected.
 */
bool mh_is_SD_connected(void);

/*!
 * @brief Checks if there was a SD insertion and then SD status turns into IDLE.
 * @return true if connected event occurred.
 */
bool mh_SD_connected(void);

/*!
 * @brief Checks if SD card was pulled out and them SD status turns into IDLE.
 * @return true if disconnected event occurred.
 */
bool mh_SD_disconnected(void);


/*!
 * @brief Mounts SD and sets current Drive.
 */
void mh_SD_mount(void);


/*!
 * @brief Unregisters the registered filesystem object.
 */
void mh_SD_disconnect(void);


#endif /* MEMORY_HANDLER_H_ */
