#pragma once

#include "Utilities.h"

const size_t FRAME_HEADER_SIZE = 10;
struct Frame
{
	char name[4];
	byte_t* data;
	size_t dataSize;
	byte_t flags1;
	byte_t flags2;
};
bool checkFrame(const Frame& frame);
bool isTextFrame(const Frame& frame);
void printToLog(const Frame& frame);

bool isTextFrameName(const char* str);