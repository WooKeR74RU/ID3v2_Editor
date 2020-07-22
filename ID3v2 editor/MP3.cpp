#include "MP3.h"

#include "Errors.h"
#include <cstdlib>

extern FILE* filePtr;

void readMP3(MP3& mp3)
{
	readID3Metadata(mp3.metaData);
	readAudio(mp3);
}
void readAudio(MP3& mp3)
{
	size_t begin = mp3.metaData.size;
	mp3.audioDataSize = getSize(filePtr) - begin;
	mp3.audioData = (byte_t*)malloc(mp3.audioDataSize * sizeof(byte_t));
	if (fseek(filePtr, begin, SEEK_SET) != 0)
		errorReadingFile();
	if (fread_s(mp3.audioData, mp3.audioDataSize, sizeof(byte_t), mp3.audioDataSize, filePtr) != mp3.audioDataSize)
		errorReadingFile();
}
void writeAudio(const MP3& mp3)
{
	if (fwrite(mp3.audioData, sizeof(byte_t), mp3.audioDataSize, filePtr) != mp3.audioDataSize)
		errorWritingFile();
}
void writeToFile(const MP3& mp3)
{
	writeToFile(mp3.metaData);
	writeAudio(mp3);
}