/*******************************************************************************
  @file     mp3_decoder.c
  @brief    file with the decoder function to use the library helix
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

 /*******************************************************************************
  *							INCLUDE HEADER FILES
  ******************************************************************************/


#include <string.h>
#include <stdbool.h>

#include "ff.h"
#include "pub/mp3dec.h"
#include "read_id3.h"
#include "mp3_decoder.h"

 /*******************************************************************************
 *					CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define NUMBER_BYTES_PER_FRAME 	3200

#define BYTES_PER_READ_OPERATION	500

#define DECODER_NORMAL_MODE 0

#define NO_ERROR_INFO 0

#define DEFAULT_ID3 "Unknown"

/*******************************************************************************
 *      FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

 /*
 * @brief  Open the file to read the information inside
 * @returns  the number of bytes of the file
 */
static bool openFile(const char* filename);


/*
* @brief  This function reads the ID3 Tag of the file if it has and move the read position according to the size of the ID3Tag to avoid read it twice
*/
static void readID3Tag(void);


/*
* @brief  calculate the number of bytes of the file
* @returns  the number of bytes of the file
*/
static uint32_t mp3FileObjectSize(void);


/*
* @brief  This function fills buffer with info encoded in mp3 file and update the pointers
*/
static void fill_buffer_with_mp3_frame(void);


/**
 * @brief res if the file is open and read as much data as passed in count parameter
 * @params buf: here we store the data
 * @params count: the number of data to be readed
 */
static uint32_t readMp3Data(void* buf, uint32_t count);


/*****************************************************************************
 *  					VARIABLES WITH LOCAL SCOPE
 *****************************************************************************/
// Helix data
static HMP3Decoder   	helixDecoder;       // Helix MP3 decoder instance
static MP3FrameInfo 	lastFrameInfo;      // MP3 frame info


// MP3 file data
static FIL 				mp3FileObject;		// MP3 file object
static uint32_t      	fileSize;          	// Size of the file used
static uint32_t      	remainingBytes;		// Encoded MP3 bytes remaining to be decoded
static bool          	fileIsOpened;       // true if there is an open file, false if is not
static uint32_t      	lastFrameLength;   	// Last frame length


// MP3-encoded buffer data
static uint8_t 		mp3FrameBuffer[NUMBER_BYTES_PER_FRAME] __attribute__((aligned(4)));
static int32_t    	mp3BufferOut;  // Index of the next element to be read (first element in buffer)
static int32_t     mp3BufferIn;   // Index of the next element to be loaded (after the last element in buffer)


// ID3 tag data
static bool hasID3;								// True if the file has valid ID3 tag
static char title[ID3_MAX_NUM_CHARS];         	// Title of the song
static char artist[ID3_MAX_NUM_CHARS];        	// Artist of the song
static char album[ID3_MAX_NUM_CHARS];         	// Album of the song
static char trackNum[ID3_MAX_NUM_CHARS];      	// Number of the track inside the album of the song
static char year[ID3_MAX_NUM_CHARS];          	// Year of the songs' album


/*******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/
void MP3Decoder_Init(void)
{
    helixDecoder = MP3InitDecoder();
    fileIsOpened = false;
    mp3BufferIn = 0;
    mp3BufferOut = 0;
    fileSize = 0;
    remainingBytes = 0;
    hasID3 = false;
}


bool MP3Decoder_LoadFile(const char* filename)
{
    bool res = false;

    // If the file was already opened, close it, to open it again
    if (fileIsOpened)
    {
        // Close the file
        f_close(&mp3FileObject);

        // Reset pointers and variables
        fileIsOpened = false;
        mp3BufferIn = 0;
        mp3BufferOut = 0;
        fileSize = 0;
        remainingBytes = 0;
        hasID3 = false;
    }

    // try to open the file an if it can modify the variables inside decoder.
    if (openFile(filename))
    {
        fileIsOpened = true;
        fileSize = mp3FileObjectSize();

        // Initialize remainingBytes
        remainingBytes = fileSize;

        // Read ID3 tag if it exits
        readID3Tag();

        // Fill buffer with the first frame
        fill_buffer_with_mp3_frame();

        res = true;

    }
    return res;
}


decoder_result_t MP3Decoder_DecodeFrame	(short* decodedDataBuffer,
										uint32_t decodedBufferSize,
										uint32_t* numSamplesDecoded,
										int* sampleRate)
{
    decoder_result_t res = DECODER_WORKED;

    // Initialize the number of PCM samples decoded in 0
    *numSamplesDecoded = 0;
    *sampleRate = 0;

    if (!fileIsOpened)
    {
        res = DECODER_NO_FILE;
    }

    // If there are still bytes to decode
    else if (remainingBytes)
    {
        // Fill the frame buffer with new data
    	// To do that, move the data again to the beginning
    	//	From this:

    	/*	Start     Output	Data	  Input     Space to read      End
    	 * 											 new data
    	 *
    	 * 	|---------|O|-----------------|I|..........................|
    	 *
    	 *							(mp3FrameBuffer)
    	 */

    	// To this:

    	/*	Start     Data    Input     	Space to read      		End
    	 * 	Output								new data
    	 *
    	 * 	|O|----------------|I|....................................|
    	 *
    	 *							(mp3FrameBuffer)
    	 */

        if ((mp3BufferOut > 0) && ((mp3BufferIn - mp3BufferOut) > 0) && (mp3BufferIn - mp3BufferOut < NUMBER_BYTES_PER_FRAME))
        {
            memmove(mp3FrameBuffer, mp3FrameBuffer + mp3BufferOut, mp3BufferIn - mp3BufferOut);
            mp3BufferIn = mp3BufferIn - mp3BufferOut;
            mp3BufferOut = 0;
        }

        // Read data and store it in the buffer
        fill_buffer_with_mp3_frame();

    	/*	Start     Data    Input     	     					End
    	 * 	Output
    	 *
    	 * 	|O|xxxxx-HEADER-DATA-HEADER-DATA-HEADER-DATA-....|I|......|
    	 *
    	 *							(mp3FrameBuffer)
    	 */
        // Search for the mp3 frame header in the buffer
        int32_t offset = MP3FindSyncWord(mp3FrameBuffer + mp3BufferOut, mp3BufferIn);
        if (offset >= 0)
        {
            mp3BufferOut += offset;
            remainingBytes -= offset;
        }

        // If offset is negative, we should continue (maybe the SyncWord is in the next buffer)
        // Bytes remaining is te variable that analizes this cases

        MP3FrameInfo nextFrameInfo;

        // Read the next frame information and check that the number of PCM
        // samples do not exceed the
        int err = MP3GetNextFrameInfo(helixDecoder, &nextFrameInfo, mp3FrameBuffer + mp3BufferOut);

        if (err == NO_ERROR_INFO)
        {
        	// If no error, but the number of samples exceedes the output buffer, it is an error
            if (nextFrameInfo.outputSamps > decodedBufferSize)
            {
                return DECODER_OVERFLOW;
            }
        }

        // If there was an error, maybe the found SYNCWord was not a proper SYNCWord
        // Continue reading to find the proper SYNCWord

        uint8_t* mp3DataStart = mp3FrameBuffer + mp3BufferOut;
        int bytesLeft = mp3BufferIn - mp3BufferOut;

        // DECODE A MP3 FRAME (Finally, what we came here for!)
        int res = MP3Decode(helixDecoder, &mp3DataStart, &(bytesLeft), decodedDataBuffer, DECODER_NORMAL_MODE);

        if (res == ERR_MP3_NONE)
        {
        	//If no error

            // Calculate the length of the mp3Frame that was decoded
            uint32_t decodedBytes = mp3BufferIn - mp3BufferOut - bytesLeft;
            lastFrameLength = decodedBytes;

            // Update the mp3Buffer pointers
            mp3BufferOut += decodedBytes;
            remainingBytes -= decodedBytes;

            // Get the last frame info (should be the same info as before)
            MP3GetLastFrameInfo(helixDecoder, &(lastFrameInfo));

            // Return the number of PCM samples decoded
            *numSamplesDecoded = lastFrameInfo.outputSamps;
            *sampleRate = lastFrameInfo.samprate;
            res = DECODER_WORKED;
        }
        else
        {
        	// We'll indicate that the file has ended
			return DECODER_END_OF_FILE;
        }
    }
    else
    {
    	// If the file came to the end:
        res = DECODER_END_OF_FILE;
    }

    return res;
}


bool MP3Decoder_GetLastFrameNumOfChannels(uint8_t* channelCount)
{
    // we assume that there are no last frame.
    bool ret = false;
    if (remainingBytes < fileSize)
    {
        *channelCount = lastFrameInfo.nChans;
        ret = true;
    }
    return ret;
}


void MP3Decoder_GetTagData(char* _title_, char* _album_, char* _artist_, char* _trackNum_, char* _year_)
{
    strcpy(album, _album_);
    strcpy(artist, _artist_);
    strcpy(title, _title_);
    for(uint8_t i = 0; i < ID3_MAX_NUM_CHARS; i++)
    {
    	trackNum[i] = _trackNum_[i];
    	year[i] = _year_[i];
    }
}


bool MP3Decoder_hasID3(void)
{
    return hasID3;
}


bool MP3Decoder_shutDown(void)
{
	if (fileIsOpened)
	{
		// Close the file
		f_close(&mp3FileObject);

		// Reset pointers and variables
		fileIsOpened = false;
		mp3BufferIn = 0;
		mp3BufferOut = 0;
		fileSize = 0;
		remainingBytes = 0;
		hasID3 = false;

	}
	return true;
}


bool MP3Decoder_getFileTitle(char ** title_)
{
    if(hasID3)
    {
        // Verify that the title is not Unknown
        if ( strcmp(title,"Unknown") != 0)
        {
            (*title_) = title;
            return true;
        }
    }
    return false;
}


bool MP3Decoder_getFileAlbum(char** album_)
{
	if(hasID3)
    {
        // Verify that the album is not Unknown
        if ( strcmp(album,"Unknown") != 0)
        {
            (*album_) = album;
            return true;
        }
    }
    return false;
}


bool MP3Decoder_getFileArtist(char** artist_)
{
	if(hasID3)
    {
        // Verify that the artist is not Unknown
        if (strcmp(artist, "Unknown") != 0)
        {
            (*artist_) = artist;
            return true;
        }
    }
    return false;
}


bool MP3Decoder_getFileYear(char** year_)
{
	if(hasID3)
    {
        // Verify that the year is not Unknown
        if ( strcmp(year,"Unknown") != 0)
        {
            (*year_) = year;
            return true;
        }
    }
    return false;
}


bool MP3Decoder_getFileTrackNum(char** trackNum_)
{
	if(hasID3)
    {
        // Verify that the trackNum is not Unknown
        if ( strcmp(trackNum,"Unknown") != 0)
        {
            (*trackNum_) = trackNum;
            return true;
        }
    }
    return false;
}


void MP3Decoded_rewindFile(void)
{
	FSIZE_t currentFilePointer = f_tell(&mp3FileObject);
	currentFilePointer -= lastFrameLength;
	remainingBytes += lastFrameLength;

	// Check that rewind does not go out of range
	if (fileSize - remainingBytes < 0)
	{
		// If that is the case, return to the beginning
		f_lseek(&mp3FileObject, 0);
	}
	else
	{
		f_lseek(&mp3FileObject, currentFilePointer);
	}
}


void MP3Decoded_fastForwardFile(void)
{
	FSIZE_t currentFilePointer = f_tell(&mp3FileObject);
	currentFilePointer += lastFrameLength;	// An approximate of next frame
	remainingBytes -= lastFrameLength;

	// Check that fast forward does not go out of range
	if (remainingBytes < 0)
	{
		// If that is the case, go to the end
		f_lseek(&mp3FileObject, f_size(&mp3FileObject));
	}
	else
	{
		f_lseek(&mp3FileObject, currentFilePointer);
	}
}


/*******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 ******************************************************************************/
static bool openFile(const char* filename)
{
    FRESULT fr = f_open(&mp3FileObject, _T(filename), FA_READ);
    if (fr == FR_OK)
        return true;
    return false;
}


static void readID3Tag(void)
{
    // Checks if the file has an ID3 Tag. (ID3 library)
    if (has_ID3_tag(&mp3FileObject))
    {
        hasID3 = true;

        if (!read_ID3_info(TITLE_ID3, title, ID3_MAX_NUM_CHARS, &mp3FileObject))
            strcpy(title, DEFAULT_ID3);

        if (!read_ID3_info(ALBUM_ID3, album, ID3_MAX_NUM_CHARS, &mp3FileObject))
            strcpy(album, DEFAULT_ID3);

        if (!read_ID3_info(ARTIST_ID3, artist, ID3_MAX_NUM_CHARS, &mp3FileObject))
            strcpy(artist, DEFAULT_ID3);

        if (!read_ID3_info(YEAR_ID3, (char *)year, ID3_MAX_NUM_CHARS, &mp3FileObject))
            strcpy(year, (char*)DEFAULT_ID3);

        if (!read_ID3_info(TRACK_NUM_ID3, (char *)trackNum, ID3_MAX_NUM_CHARS, &mp3FileObject))
            strcpy(trackNum, (char*)DEFAULT_ID3);

        unsigned int tagSize = get_ID3_size(&mp3FileObject);

        // Position the internal file pointer where the data starts
        f_lseek(&mp3FileObject, tagSize);
        remainingBytes -= tagSize;
    }
    else
    {
    	// Position the internal file pointer where the data starts (the beginning)
        f_rewind(&mp3FileObject);
    }
}


static uint32_t mp3FileObjectSize()
{
    uint32_t size = 0;
    if (fileIsOpened)
    {
        size = f_size(&mp3FileObject);
    }
    return size;
}


static void fill_buffer_with_mp3_frame(void)
{
	/*	Start     Output	Data   Input     Space to read      End
	 * 											new data
	 *
	 * 	|---------|O|--------------|I|............................|
	 *
	 *							(mp3FrameBuffer)
	 */

    // Fill the remaining buffer space with the new mp3 data
    uint16_t bytesRead;

    uint8_t* free_space = &mp3FrameBuffer[mp3BufferIn];

    // Read from mp3BufferIn to the end of the buffer or until there's no more data
    bytesRead = readMp3Data(free_space, (NUMBER_BYTES_PER_FRAME - mp3BufferIn));

    // Update Input Index (if bytesRead == NUMBER_BYTES_PER_FRAME - mp3BufferIn - 1)
    // Then mp3BufferIn will point just outside the buffer
    mp3BufferIn += bytesRead;
}


static uint32_t readMp3Data(void* buffer, uint32_t bytes_to_read)
{
	uint32_t total_bytes_read = 0;
	uint32_t bytes_read;
    uint32_t i;

    char * auxBufferPointer = buffer;

    FRESULT fr;

    // Read per multiples of BYTES_PER_READ_OPERATION
    for(i = 0; i < bytes_to_read / BYTES_PER_READ_OPERATION; i++)
    {
    	fr = f_read(&mp3FileObject, auxBufferPointer, BYTES_PER_READ_OPERATION, &bytes_read);
    	if (fr != FR_OK)
    	     break;

    	// Update buffer pointer
    	auxBufferPointer += BYTES_PER_READ_OPERATION;

    	// Update total number of bytes read
    	total_bytes_read += bytes_read;
    }

    i = bytes_to_read % BYTES_PER_READ_OPERATION;

    // If there are still bytes to read (bytes_to_read is not multiple of
    // BYTES_PER_READ_OPERATION
    if(fr == FR_OK && i != 0)
    {
    	fr = f_read(&mp3FileObject, auxBufferPointer, i, &bytes_read);
    	if (fr == FR_OK)
    		total_bytes_read += bytes_read;
    }

    // Return the number of bytes read
    return fr == FR_OK ? total_bytes_read : 0;
}


