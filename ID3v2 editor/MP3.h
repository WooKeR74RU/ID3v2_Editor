#pragma once

#include "ID3Metadata.h"

struct MP3
{
	ID3Metadata metaData;
	byte_t* audioData;
	size_t audioDataSize;
};
void readMP3(MP3& mp3);
void readAudio(MP3& mp3);
void writeAudio(const MP3& mp3);
void writeToFile(const MP3& mp3);