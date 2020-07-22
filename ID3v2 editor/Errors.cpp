#include "Errors.h"

#include <cstdio>
#include <cstdlib>

extern FILE* logFile;
extern FILE* filePtr;

void exit()
{
	if (logFile != nullptr)
		fclose(logFile);
	if (filePtr != nullptr)
		fclose(filePtr);
	exit(EXIT_FAILURE);
}

void errorArguments()
{
	fprintf_s(logFile, "Incorrect arguments\n");
	exit();
}
void errorOpeningFile()
{
	fprintf_s(logFile, "Opening file error\n");
	exit();
}
void errorReadingFile()
{
	fprintf_s(logFile, "Reading file error\n");
	exit();
}
void errorWritingFile()
{
	fprintf_s(logFile, "Writing file error\n");
	exit();
}
void errorMP3Format()
{
	fprintf_s(logFile, "Incorrect MP3 file format\n");
	exit();
}
void errorFrameFormat()
{
	fprintf_s(logFile, "Incorrect MP3 frame format\n");
	exit();
}
void errorFrameName()
{
	fprintf_s(logFile, "Frame isn't text\n");
	exit();
}