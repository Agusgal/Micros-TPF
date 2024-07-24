/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief	Handles MP3 functions like playing songs and finding metadata
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef _MP3_HANDLER_H_
#define _MP3_HANDLER_H_

#include <stdbool.h>

/**
 * @brief Initializes the mp3 Handler
 */
void mp3Handler_init(void);


/**
 * @brief Loads next object
 */
void mp3Handler_nextObject(void);


/**
 * @brief Loads next object
 */
void mp3Handler_nextMP3File(void);


/**
 * @brief Loads previous
 */
void mp3Handler_prevObject(void);


/**
 * @brief Loads previous
 */
void mp3Handler_prevMP3File(void);


/**
 * @brief Selects the current object
 * @return bool. True if the object selected is a MP3 File. False otherwise
 */
bool mp3Handler_selectObject(void);


/**
 * @brief Update AudioPlayer Back buffer
 *
 */
void mp3Handler_updateAudioPlayerBackBuffer(void);


/**
 *  @brief Play next song from directory. If the song is last,
 *  	   then the first song from the directory will be played.
 */
void mp3Handler_playNextSong(void);


/**
 *  @brief Play previous song from directory. If the song is first,
 *  	   then the last from the directory will be played.
 */
void mp3Handler_playPreviousSong(void);

/**
 *  @brief Stops handler functions, shuts down decoder, disconnects SD,
 *         resets objects and clears Display.
 */
void mp3Handler_deinit(void);

/**
 *  @brief Gets song's title.
 *  @return Pointer to char of the title name.
 */
char * mp3Handler_getTitle(void);

/**
 *  @brief Gets object name, either file or directory.
 *  @return Pointer to char of the object name.
 */
char * mp3Handler_getCurrentName(void);

/**
 *  @brief Gets song's artist.
 *  @return Pointer to char of the artist's name.
 */
char * mp3Handler_getArtist(void);

/**
 *  @brief Gets song's album.
 *  @return Pointer to char of the album's name.
 */
char * mp3Handler_getAlbum(void);

/**
 *  @brief Gets song's releasing year.
 *  @return Pointer to char of the releasing year.
 */
char * mp3Handler_getYear(void);

/**
 *  @brief Updates Audio Player BackBuffer and shows FFT.
 */
void mp3Handler_updateAll(void);

/**
 *  @brief Draws Display.
 */
void mp3Handler_showFFT(void);
/**
 *  @brief Plays song
 */
void mp3Handler_play(void);

/**
 *  @brief If song is playing, it pauses. If it is paused, it starts playing.
 */
void mp3Handler_toggle(void);

/**
 *  @brief Pauses song.
 */
void mp3Handler_stop(void);

/**
 *  @brief Increases volume in a fixed step every time the function is called.
 */
void mp3Handler_IncVolume(void);

/**
 *  @brief Decreases volume in a fixed step every time the function is called.
 */
void mp3Handler_DecVolume(void);

/**
 *  @brief Gets the song's volume.
 *  @return song's volume.
 */
char mp3Handler_getVolume(void);

/**
 *  @brief Sets song's volume.
 *  @param value: volume value wanting to set.
 */
void mp3Handler_setVolume(char value);


#endif /* _MP3_HANDLER_H_ */
