#include "Frame.h"

#include <cstring>

extern FILE* logFile;

bool checkFrame(const Frame& frame)
{
	bool res = true;
	res &= frame.flags1 == 0;
	res &= frame.flags2 == 0;
	return res;
}
bool isTextFrame(const Frame& frame)
{
	return frame.name[0] == 'T' && (frame.name[1] != 'X' || frame.name[2] != 'X' || frame.name[3] != 'X');
}
void printToLog(const Frame& frame)
{
	for (size_t i = 0; i < 4; i++)
		fprintf_s(logFile, "%c", frame.name[i]);
	fprintf_s(logFile, ": ");
	if (isTextFrame(frame))
	{
		if (frame.data[0] == 0)
		{
			for (size_t i = 1; i < frame.dataSize; i++)
				fprintf_s(logFile, "%c", frame.data[i]);
		}
		else
		{
			fprintf_s(logFile, "Not a ISO-8859-1");
		}
	}
	else
	{
		fprintf_s(logFile, "Not a text frame");
	}
	fprintf_s(logFile, "\n");
}

bool isTextFrameName(const char* str)
{
	return strlen(str) == 4 && str[0] == 'T' && (str[1] != 'X' || str[2] != 'X' || str[3] != 'X');
}