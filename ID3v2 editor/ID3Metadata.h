#pragma once

#include "Frame.h"

const size_t ID3HEADER_SIZE = 10;
struct ID3Metadata
{
	char fileIdentifier[3];
	byte_t majorVersion;
	byte_t revisionVersion;
	byte_t flags;
	size_t size;
	size_t realSize;
	Frame* frames;
	size_t framesCount;
};
bool checkHeader(const ID3Metadata& data);
void readHeader(ID3Metadata& data);
void readFrames(ID3Metadata& data);
void readID3Metadata(ID3Metadata& data);
void writeHeaderToFile(const ID3Metadata& data);
void writeFramesToFile(const ID3Metadata& data);
void writeToFile(const ID3Metadata& data);