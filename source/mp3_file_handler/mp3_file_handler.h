/***************************************************************************/ /**
  @file     mp3_file_handler.h
  @brief    Mp3 File Handler header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef MP3_FILE_HANDLER_H_
#define MP3_FILE_HANDLER_H_

#include <stdbool.h>
#include <stdint.h>

#define STR_SIZE 500
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum mp3_object_type
{
	MP3_FILE,
	DIRECTORY,
	RETURN_DIR,
	NULL_OBJECT
} mp3_object_type_t;

typedef struct
{
  char path[STR_SIZE];
  int index;
  mp3_object_type_t object_type;

} MP3Object_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/*
 * @brief Scan all mp3 files in the storage device
 */
void mp3Files_Init(void);


/*
 * @brief Detects if a file's path corresponds to a .mp3 file.
 * @param path: complete file's path.
 * @return Flag that is true if the file extension is MP3
 * */
bool mp3Files_isMp3File(char *path);


/*
 * @brief Adds an object to the file system.
 * @param path: complete file's path.
 * @param object_type: file, directory, return_dir or Null
 *
 * */
void mp3Files_AddDirectory(char *path, mp3_object_type_t object_type);

/*
 * @brief Adds an object to the file system.
 * @param path: complete file's path.
 * @param object_type: file, directory, return_dir or Null
 *
 * */
void mp3Files_AddSong(char *path, mp3_object_type_t object_type);

/*
 * @brief Adds an object to the file system.
 * @param path: complete file's path.
 * @param object_type: file, directory, return_dir or Null
 *
 * */
void mp3Files_AddPlayingSongs(char *path, mp3_object_type_t object_type);


/*
 * @brief Gets the first object of the file system.
 * @return MP3Object struct containing path and object type.
 *
 * */
MP3Object_t mp3Files_GetFirstObject(void);


/*
 * @brief	Updates the playing songs list
 *
 * */
void updatePlayingSongs(void);

/*
 * @brief Gets the next object of the file system.
 * @param currentObject: The object that is considered the 'current' one. The 'next' object will be defined based on this param.
 * @return MP3Object_t next object of the file system.  If the current object is the last one, returns the first one.
 *
 * */
MP3Object_t mp3Files_GetNextObject(MP3Object_t currentObject);


/*
 * @brief Gets the next MP3 File of the file system.
 * @param currentObject: The object that is considered the 'current' one. The 'next' object will be defined based on this param.
 * @return MP3Object_t next object of the file system.  If the current object is the last one, returns the first one.
 *
 * */
MP3Object_t mp3Files_GetNextMP3File(MP3Object_t currentObject);


/*
 * @brief Gets the previous object of the file system.
 * @param currentObject: The object that is considered the 'current' one. The 'previous' object will be defined based on this param.
 * @return MP3Object_t previous object of the file system.  If the current object is the first one, returns the last one.
 *
 * */
MP3Object_t mp3Files_GetPreviousObject(MP3Object_t currentObject);


/*
 * @brief Gets the previous MP3 File of the file system.
 * @param currentObject: The object that is considered the 'current' one. The 'previous' object will be defined based on this param.
 * @return MP3Object_t previous object of the file system.  If the current object is the first one, returns the last one.
 *
 * */
MP3Object_t mp3Files_GetPreviousMP3File(MP3Object_t currentObject);


/*
 * @brief Gets the object's name of a MP3Object_t.
 * @param The object whose name is wanted.
 * @return char * file's name. (Ex.: if file's path is "/dir0/dir1/hello.mp3", it's file's name is "hello.mp3");
 *
 * */
char* mp3Files_GetObjectName(MP3Object_t object);


/*
 * @brief Reset the file system.
 *
 * */
void mp3Files_Reset(void);


/*
 * @brief Enter the specified directory.
 * @param Object (directory wanting to enter to).
 * @return First object in the directory.
 *
 * */
MP3Object_t mp3Files_Enter_Dir(MP3Object_t object);


/*
 * @brief Exit to the specified directory
 * @param Object (directory wanting to exit from).
 * @return First object in the directory.
 * */
MP3Object_t mp3Files_Exit_Dir(MP3Object_t object);


/*
 * @Brief Retrieves the current directory of the current drive.
 * @param Buffer to return path name.
 * @param The length of the buffer.
 */
void getCurrentDirectory(char* buff, unsigned int len);


#endif /* MP3_FILE_HANDLER_H_ */
