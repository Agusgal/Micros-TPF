/***************************************************************************/ /**
  @file     daytime.h
  @brief    Time Service Daytime Header
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/

#ifndef DAYTIME_H_
#define DAYTIME_H_

#include <stdint.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



typedef struct DateTimeDate_t
{
    uint16_t year;  /*!< Range from 1970 to 2099.*/
    uint8_t month;  /*!< Range from 1 to 12.*/
    uint8_t day;    /*!< Range from 1 to 31 (depending on month).*/
    uint8_t hour;   /*!< Range from 0 to 23.*/
    uint8_t minute; /*!< Range from 0 to 59.*/
    uint8_t second; /*!< Range from 0 to 59.*/
} DateTimeDate_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void DateTime_Init(void (*callback)(void));
DateTimeDate_t DateTime_GetCurrentDateTime(void);

void DateTime_Enable(void);
void DateTime_Disable(void);



#endif /* DAYTIME_H_ */
