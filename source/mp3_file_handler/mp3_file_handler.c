
/***************************************************************************/ /**
  @file     mp3_file_handler.c
  @brief    MP3 File Handler functions
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#include "mp3_file_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ff.h"

#define DIRECTORY_ARRAY_SIZE 	20
#define FILE_ARRAY_SIZE 		50
#define FILE_NAME_STRING_SIZE	150

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static MP3Object_t current_directories[DIRECTORY_ARRAY_SIZE];
static unsigned int directoriesCounter = 0;

static MP3Object_t current_songs[FILE_ARRAY_SIZE];
static unsigned int songsCounter = 0;

static MP3Object_t playing_songs[FILE_ARRAY_SIZE];
static unsigned int playingSongsCounter = 0;


static void mp3Files_list_dir(char * path);


static void getParentDirectory(char* filePath, char* parentDir);


static void sortDirectoriesBuffer(void);


static void sortSongsBuffer(void);


static int compPathsAlphabeticalOrder (const void *_elem1, const void *_elem2);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void mp3Files_Init(void)
{
	// Set path in 0
	directoriesCounter = 0;
	songsCounter = 0;
	playingSongsCounter = 0;

	mp3Files_list_dir("");
}


bool mp3Files_isMp3File(char *path)
{
	char *extension;
	if ((extension = strrchr(path, '.')) != NULL)
	{
		if (strcmp(extension, ".mp3") == 0)
		{
			return true;
		}
	}
	return false;
}


void mp3Files_AddDirectory(char *path, mp3_object_type_t object_type)
{
	strcpy(current_directories[directoriesCounter].path, path);
	current_directories[directoriesCounter].index = directoriesCounter;
	current_directories[directoriesCounter].object_type = object_type;
	directoriesCounter++;
}


void mp3Files_AddSong(char *path, mp3_object_type_t object_type)
{
	strcpy(current_songs[songsCounter].path, path);
	current_songs[songsCounter].index = songsCounter;
	current_songs[songsCounter].object_type = object_type;
	songsCounter++;
}


void mp3Files_AddPlayingSongs(char *path, mp3_object_type_t object_type)
{
	strcpy(playing_songs[playingSongsCounter].path, path);
	playing_songs[playingSongsCounter].index = playingSongsCounter;
	playing_songs[playingSongsCounter].object_type = object_type;
	playingSongsCounter++;
}


MP3Object_t mp3Files_GetFirstObject(void)
{
	if (directoriesCounter == 0 && songsCounter == 0)
	{
		// If there are no objects, including RETURN_DIR, return a NULL_OBJECT
		MP3Object_t nullFile = {.object_type = NULL_OBJECT, .index = -1, .path = ""};
		return nullFile;
	}
	else if (directoriesCounter == 1 && songsCounter == 0)
	{
		// Return folder to return to previous folder if there is only one folder
		return current_directories[0];
	}
	else if(directoriesCounter > 1)
	{
		// If there are more than one folder, then the second one is the first directory
		return current_directories[1];
	}
	else if(songsCounter > 0)
	{
		// If there are songs, and no first directory, return the first song
		return current_songs[0];
	}

	// Return the first object, that is not to return to previous folder
	return current_directories[1];
}


MP3Object_t mp3Files_GetNextObject(MP3Object_t currentObject)
{
	unsigned int nextObjectIndex = currentObject.index + 1;

	if(currentObject.object_type == DIRECTORY || currentObject.object_type == RETURN_DIR)
	{
		// If the current Object is a directory
		if (nextObjectIndex == directoriesCounter)
		{
			// If the next index is out of range, go to the songs list
			nextObjectIndex = 0;
			if (nextObjectIndex == songsCounter)
			{
				// If the next index is out of range, return the first directory (no mp3 files)
				return current_directories[nextObjectIndex];
			}
			else
			{
				// Return the first song
				return current_songs[nextObjectIndex];
			}
		}
		else
		{
			// If the next index is on range, return the next directory
			return current_directories[nextObjectIndex];
		}

	}
	else
	{
		// If the current Object is a song
		if (nextObjectIndex == songsCounter)
		{
			// If the next index is out of range, go to the directories list
			nextObjectIndex = 0;
			return current_directories[nextObjectIndex];
		}
		else
		{
			// If the next index is on range, return the next song
			return current_songs[nextObjectIndex];
		}
	}
}



MP3Object_t mp3Files_GetPreviousObject(MP3Object_t currentObject)
{
	unsigned int previousObjectIndex = currentObject.index - 1;

	if(currentObject.object_type == DIRECTORY || currentObject.object_type == RETURN_DIR)
	{
		// If the current Object is a directory
		if (currentObject.index == 0)
		{
			// If the next index is out of range, go to the songs list
			previousObjectIndex = songsCounter;
			if (previousObjectIndex == 0)
			{
				// If the next index is out of range, return the first directory (no mp3 files)
				return current_directories[directoriesCounter-1];
			}
			else
			{
				// Return the first song
				return current_songs[songsCounter-1];
			}
		}
		else
		{
			// If the next index is on range, return the next directory
			return current_directories[previousObjectIndex];
		}

	}
	else
	{
		if (currentObject.index == 0)
		{
			// If the next index is out of range, go to the directories
			return current_directories[directoriesCounter-1];
		}
		else
		{
			// If the next index is on range, return the next directory
			return current_songs[previousObjectIndex];
		}
	}
}


void updatePlayingSongs(void)
{
	uint32_t i;
	for(i = 0; i < songsCounter; i++)
	{
		playing_songs[i] = current_songs[i];
	}

	playingSongsCounter = songsCounter;
}


MP3Object_t mp3Files_GetNextMP3File(MP3Object_t currentObject)
{
	unsigned int nextObjectIndex = currentObject.index + 1;
	if (nextObjectIndex == playingSongsCounter)
	{
		nextObjectIndex = 0;
	}
	return playing_songs[nextObjectIndex];
}


MP3Object_t mp3Files_GetPreviousMP3File(MP3Object_t currentObject)
{
	unsigned int previousObjectIndex = currentObject.index - 1;
	if (currentObject.index == 0)
	{
		previousObjectIndex = playingSongsCounter-1;
	}
	return playing_songs[previousObjectIndex];
}



char* mp3Files_GetObjectName(MP3Object_t object)
{
	char *name;
	name = strrchr(object.path, '/');
	return name;
}


void mp3Files_Reset(void)
{
	directoriesCounter = 0;
	songsCounter = 0;
	playingSongsCounter = 0;
}


MP3Object_t mp3Files_Enter_Dir(MP3Object_t object)
{
	if (object.object_type == DIRECTORY)
	{
		mp3Files_list_dir(object.path);
		return mp3Files_GetFirstObject();
	}
	else
	{
		return mp3Files_GetFirstObject();
	}
}


MP3Object_t mp3Files_Exit_Dir(MP3Object_t object)
{
	if (object.object_type == RETURN_DIR)
	{
		char parentDir[FILE_NAME_STRING_SIZE];
		char parentParentDir[FILE_NAME_STRING_SIZE];

		getParentDirectory(object.path, parentDir);
		getParentDirectory(parentDir, parentParentDir);

		mp3Files_list_dir(parentParentDir);

		return mp3Files_GetFirstObject();
	}
	else
	{
		return mp3Files_GetFirstObject();
	}
}


void getCurrentDirectory(char* buff, unsigned int len)
{
	f_getcwd(buff, len);
}

/*************************************************************************************
 * 		LOCAL FUNCTIONS DECLARATIONS
 ************************************************************************************/
static void mp3Files_list_dir(char * path)
{
	FRESULT res;
	DIR dir;
	FILINFO fno;
	int nfile, ndir;

	unsigned int i = strlen(path);
	char newPath[FILE_NAME_STRING_SIZE] = {0};

	strcpy(newPath, path);
	char * fn = "...";
	*(newPath+i) = '/';
	strcpy(newPath+i+1, fn);

	// Save the first object as a return to previous directory object
	directoriesCounter = 0;
	songsCounter = 0;

	mp3Files_AddDirectory(newPath, RETURN_DIR);

	// Save the remaining objects

	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK)
	{
		nfile = ndir = 0;
		for (;;)
		{
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Error or end of dir */

			// !(fno.fattrib & (AM_HID | AM_SYS))
			// Checks that the file is not a hidden file or system file
			if ((!(fno.fattrib & (AM_HID | AM_SYS))) && (fno.fattrib & AM_DIR))
			{            /* Directory */
				// Complete the path of the new directory
				char * fn = fno.fname;
				*(newPath+i) = '/';
				strcpy(newPath+i+1, fn);

				mp3_object_type_t object_type = DIRECTORY;

				mp3Files_AddDirectory(newPath, object_type);

			}
			else if (!(fno.fattrib & (AM_HID | AM_SYS)))
			{                               /* File */
				// Complete the path of the new file
				char * fn = fno.fname;
				*(newPath+i) = '/';
				strcpy(newPath+i+1, fn);

				mp3_object_type_t object_type = MP3_FILE;

				if(mp3Files_isMp3File(newPath))
				{
					mp3Files_AddSong(newPath, object_type);
				}
			}
		}
		f_closedir(&dir);

		// Sort the buffers
		sortDirectoriesBuffer();
		sortSongsBuffer();
	}
	else
	{
		printf("Failed to open \"%s\". (%u)\n", path, res);
	}
}


static void getParentDirectory(char* filePath, char* parentDir)
{
    // Copy the input path to the result path
    strncpy(parentDir, filePath, FILE_NAME_STRING_SIZE);

    // Find the last occurrence of the directory separator '/'
    char* lastSlash = strrchr(parentDir, '/');

    if (lastSlash != NULL) {
        // Terminate the string at the last slash to remove the filename
        *lastSlash = '\0';
    } else {
        // No directory separator found, assuming the file is in the current directory
    	char * a = "";
    	strcpy(parentDir, a);
    }
}


static void sortDirectoriesBuffer(void)
{
	// Sort
	qsort(current_directories, directoriesCounter, sizeof(MP3Object_t), compPathsAlphabeticalOrder);

	// Update indexes
	uint32_t i;
	for (i = 0; i < directoriesCounter; i++)
	{
		current_directories[i].index = i;
	}
}


static void sortSongsBuffer(void)
{
	// Sort
	qsort(current_songs, songsCounter, sizeof(MP3Object_t), compPathsAlphabeticalOrder);

	// Update indexes
	uint32_t i;
	for (i = 0; i < songsCounter; i++)
	{
		current_songs[i].index = i;
	}
}


static int compPathsAlphabeticalOrder (const void *_elem1, const void *_elem2)
{
	MP3Object_t * elem1, * elem2;

	elem1 = (MP3Object_t *) _elem1;
	elem2 = (MP3Object_t *) _elem2;

	return strcmp(elem1->path, elem2->path);
}

