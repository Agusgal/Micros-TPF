/*******************************************************************************
  @file     file_selection.h
  @brief   	File Selection state header
  @author   Grupo 5 - Lab de Micros
 **********/

#ifndef FILE_SELECTION_H
#define FILE_SELECTION_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialization of the state.
 */
void FileSelection_InitState(void);
/**
 * @brief Gets the next file.
 */
void FileSelection_NextFile(void);
/**
 * @brief Gets the previous file.
 */
void FileSelection_PreviousFile(void);

/*
 * @brief	Selects the current file.
 */
void FileSelection_SelectFile(void);

/**
 * @brief Returns to the fileSelection state
 */
void FileSelection_ReturnState(void);

/*
 * @brief	Selects the current file.
 */
void FileSelection_MP3_UpdateAll(void);


/*
 * @brief Start playing the next song in the background
 */
void FileSelection_PlayNextSong(void);

/*
 * @brief Start playing the previous song in the background
 */
void FileSelection_PlayPrevSong(void);

#endif //FILE_SELECTION_H
