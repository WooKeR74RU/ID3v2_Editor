#include "Process.h"
#include "Errors.h"
#include <clocale>
#include <cstring>
#include <cstdlib>

FILE* logFile;
FILE* filePtr;

int main(size_t argc, const char* argv[])
{
	setlocale(LC_ALL, "Russian");
	logFile = stdout;
	if (!checkArgs(argv, argc))
		errorArguments();

	if (fopen_s(&filePtr, argv[1] + 11, "rb") != 0)
		errorOpeningFile();
	MP3 mp3;
	readMP3(mp3);

	if (strcmp(argv[2], "--show") == 0)
		show(mp3);
	if (strncmp(argv[2], "--get=", 6) == 0)
		get(mp3, argv[2] + 6);
	if (strncmp(argv[2], "--set=", 6) == 0)
		set(mp3, argv[1] + 11, argv[2] + 6, argv[3] + 8);

	fclose(filePtr);
	fclose(logFile);
	return EXIT_SUCCESS;
}