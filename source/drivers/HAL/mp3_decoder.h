/***************************************************************************//**
  @file     mp3_decoder.h
  @brief    decoder header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
*							INCLUDE HEADER FILES
******************************************************************************/

#ifndef _MP3_DECODER_H_
#define _MP3_DECODER_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>


/*******************************************************************************
*				  CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define ID3_MAX_NUM_CHARS 50


#define DECODED_BUFFER_SIZE 5000

/*******************************************************************************
 *					ENUMERATIONS, STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
	DECODER_WORKED,
	DECODER_ERROR,
	DECODER_FRAME_NOT_VALID,
	DECODER_NO_FILE,
	DECODER_END_OF_FILE,
	DECODER_OVERFLOW
} decoder_result_t;


/*******************************************************************************
 *					FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*
* @brief Initialize the values inside the decoder_t decoder struct
*/
void MP3Decoder_Init(void);


/**
 * @brief This function open the encoded mp3 file, if it was open
		  close it reset the values and open again
 * @param filename: file's path.
 * @return true if it can open the mp3 file and false if it can not.
 */
bool MP3Decoder_LoadFile(const char* filename);


/**
 * @brief Decode next frame
 * @param decodedDataNuffer: the buffer were we will store the decoded data.
 * @param decodedBufferSize: the size of the buffer were we will store the decoded data.
 * @param numSamplesDecoded: The pointer to an uint_32 data to store there the number of samples decoded
 * @param sampleRate: pointer to the sampleRate needed.
 * @return true if it can open the mp3 file and false if it can not.
 */
decoder_result_t MP3Decoder_DecodeFrame	(short* decodedDataBuffer,
										uint32_t decodedBufferSize,
										uint32_t* numSamplesDecoded,
										int* sampleRate);


/**
 * @brief get the number of channels from last frame.
 * @param channelCount: pointer to a variable where we will store the number of channels.
 * @return true if the function can get the info of the last frame.
 */
bool MP3Decoder_GetLastFrameNumOfChannels(uint8_t* channelCount);


/**
 * @brief get the Data of the mp3's tag.
 * @param title: pointer to the char array where we will store the songs title.
 * @param album: pointer to the char array where we will store the songs album.
 * @param artist: pointer to the char array where we will store the songs artist.
 * @param trackNum: pointer to the uint8_t array where we will store the songs track number in the album.
 * @param year: pointer to the uint8_t array where we will store the album year.
 */
void MP3Decoder_GetTagData(char* _title_, char* _album_, char* _artist_, char* _trackNum_, char* _year_);


/**
 * @brief getter of hasID3
 * @return: the value of hasID3, true if the son has ID3 false if it doesn't.
 */
bool MP3Decoder_hasID3(void);


/**
 * @brief: Close file if it is open.
 * @return true if successfully shut down the file.
 */
bool MP3Decoder_shutDown(void);


/**
 * @brief: Get the current file's title.
 * @param title_: is a pointer to the char pointer (array of chars) that forms the title word.
 * @return: true if hasID3 returns true.
 */
bool MP3Decoder_getFileTitle(char ** title_);


/**
 * @brief: Get the current file's album.
 * @param album_: is a pointer to the char pointer (array of chars) that forms the album word.
 * @return: true if hasID3 returns true.
 */
bool MP3Decoder_getFileAlbum(char** album_);


/**
 * @brief: Get the current file's artist.
 * @param artist_: is a pointer to the char pointer (array of chars) that forms the artist word.
 * @return: true if hasID3 returns true.
 */
bool MP3Decoder_getFileArtist(char** artist_);


/**
 * @brief: Get the current file's year.
 * @param year_: is a pointer to the char pointer (array of chars) that forms the year word.
 * @return: true if hasID3 returns true.
 */
bool MP3Decoder_getFileYear(char** year_);


/**
 * @brief: Get the current file's track number.
 * @param trackNum_: is a pointer to the char pointer (array of chars) that forms the Track number word.
 * @return: true if hasID3 returns true.
 */
bool MP3Decoder_getFileTrackNum(char** trackNum_);


#endif /* _MP3_DECODER_H_ */
