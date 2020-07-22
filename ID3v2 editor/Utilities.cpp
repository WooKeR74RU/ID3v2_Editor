#include "Utilities.h"

#include "Errors.h"

extern FILE* logFile;

size_t getSize(FILE* file)
{
	int temp = ftell(file);
	if (temp == -1 || fseek(file, 0, SEEK_END) != 0)
		errorReadingFile();
	int res = ftell(file);
	if (res == -1 || fseek(file, temp, SEEK_SET) != 0)
		errorReadingFile();
	return (size_t)res;
}
void printToLog(byte_t byte)
{
	if (byte < 16)
		fprintf_s(logFile, "0");
	fprintf_s(logFile, "%X", (int)byte);
}