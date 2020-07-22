#include "Process.h"

extern FILE* logFile;
extern FILE* filePtr;

#include "Errors.h"
#include <cstdlib>
#include <cstring>

bool checkArgs(const char* argv[], size_t argc)
{
	if (argc != 3 && argc != 4)
		return false;
	if (strncmp(argv[1], "--filepath=", 11) != 0)
		return false;
	if (strcmp(argv[2], "--show") != 0 && strncmp(argv[2], "--get=", 6) != 0 && strncmp(argv[2], "--set=", 6) != 0)
		return false;
	if (strncmp(argv[2], "--set=", 6) == 0 && argc != 4)
		return false;
	if (strncmp(argv[2], "--set=", 6) != 0 && argc == 4)
		return false;
	if (strncmp(argv[2], "--set=", 6) == 0 && strncmp(argv[3], "--value=", 8) != 0)
		return false;
	return true;
}

void show(const MP3& mp3)
{
	for (size_t i = 0; i < mp3.metaData.framesCount; i++)
		printToLog(mp3.metaData.frames[i]);
}

void get(const MP3& mp3, const char* frameName)
{
	if (!isTextFrameName(frameName))
		errorFrameName();
	bool exists = false;
	for (size_t i = 0; i < mp3.metaData.framesCount; i++)
	{
		if (strncmp(mp3.metaData.frames[i].name, frameName, strlen(frameName)) == 0)
		{
			printToLog(mp3.metaData.frames[i]);
			exists = true;
			break;
		}
	}
	if (!exists)
		fprintf_s(logFile, "Such a frame doesn't exist\n");
}

void set(MP3& mp3, const char* filename, const char* frameName, const char* frameValue)
{
	if (!isTextFrameName(frameName))
		errorFrameName();
	bool exists = false;
	for (size_t i = 0; i < mp3.metaData.framesCount; i++)
	{
		Frame& frame = mp3.metaData.frames[i];
		if (strncmp(frame.name, frameName, strlen(frameName)) == 0)
		{
			mp3.metaData.realSize -= frame.dataSize;
			frame.dataSize = strlen(frameValue) + 1;
			frame.data = (byte_t*)realloc(frame.data, frame.dataSize * sizeof(byte_t));
			frame.data[0] = 0;
			memcpy_s(frame.data + 1, strlen(frameValue), frameValue, strlen(frameValue));
			mp3.metaData.realSize += frame.dataSize;
			exists = true;
			break;
		}
	}
	if (!exists)
	{
		mp3.metaData.frames = (Frame*)realloc(mp3.metaData.frames, (mp3.metaData.framesCount + 1) * sizeof(Frame));
		Frame& frame = mp3.metaData.frames[mp3.metaData.framesCount];
		memcpy(frame.name, frameName, strlen(frameName));
		frame.dataSize = strlen(frameValue) + 1;
		frame.data = (byte_t*)malloc(frame.dataSize * sizeof(byte_t));
		frame.data[0] = 0;
		memcpy_s(frame.data + 1, strlen(frameValue), frameValue, strlen(frameValue));
		frame.flags1 = 0;
		frame.flags2 = 0;
		mp3.metaData.framesCount++;
		mp3.metaData.realSize += FRAME_HEADER_SIZE;
		mp3.metaData.realSize += frame.dataSize;
	}
	fclose(filePtr);
	if (fopen_s(&filePtr, filename, "w+b") != 0)
		errorOpeningFile();
	writeToFile(mp3);
	if (exists)
		fprintf_s(logFile, "Frame's value changed\n");
	else
		fprintf_s(logFile, "Frame added\n");
}