#include "ID3Metadata.h"

#include "Errors.h"
#include <cstdlib>

extern FILE* filePtr;

bool checkHeader(const ID3Metadata& data)
{
	bool res = true;
	res &= data.fileIdentifier[0] == 'I';
	res &= data.fileIdentifier[1] == 'D';
	res &= data.fileIdentifier[2] == '3';
	res &= data.majorVersion == 3;
	res &= data.revisionVersion == 0;
	res &= data.flags == 0;
	return res;
}
void readHeader(ID3Metadata& data)
{
	byte_t header[ID3HEADER_SIZE];
	if (fread_s(header, ID3HEADER_SIZE, sizeof(byte_t), ID3HEADER_SIZE, filePtr) != ID3HEADER_SIZE)
		errorReadingFile();
	for (size_t i = 0; i < 3; i++)
		data.fileIdentifier[i] = header[i];
	data.majorVersion = header[3];
	data.revisionVersion = header[4];
	data.flags = header[5];
	data.size = 0;
	for (size_t i = 6; i < 10; i++)
		data.size = data.size << 7 | header[i];
	if (!checkHeader(data))
		errorMP3Format();
}
void readFrames(ID3Metadata& data)
{
	size_t maxFramesCount = data.size / FRAME_HEADER_SIZE;
	data.frames = (Frame*)malloc(maxFramesCount * sizeof(Frame));
	data.framesCount = 0;
	size_t bytesCount = 0;
	while (bytesCount + FRAME_HEADER_SIZE < data.size)
	{
		byte_t frameHeader[FRAME_HEADER_SIZE];
		if (fread_s(frameHeader, FRAME_HEADER_SIZE, sizeof(byte_t), FRAME_HEADER_SIZE, filePtr) != FRAME_HEADER_SIZE)
			errorReadingFile();
		bytesCount += FRAME_HEADER_SIZE;
		if (frameHeader[0] == 0)
			break;
		Frame& frame = data.frames[data.framesCount];
		data.framesCount++;
		for (size_t i = 0; i < 4; i++)
			frame.name[i] = frameHeader[i];
		frame.dataSize = 0;
		for (size_t i = 4; i < 8; i++)
			frame.dataSize = frame.dataSize << 8 | frameHeader[i];
		frame.flags1 = frameHeader[8];
		frame.flags2 = frameHeader[9];
		frame.data = (byte_t*)malloc(frame.dataSize * sizeof(byte_t));
		if (fread_s(frame.data, frame.dataSize, sizeof(byte_t), frame.dataSize, filePtr) != frame.dataSize)
			errorReadingFile();
		bytesCount += frame.dataSize;
		if (!checkFrame(frame))
			errorFrameFormat();
	}
	data.realSize = bytesCount;
	data.frames = (Frame*)realloc(data.frames, data.framesCount * sizeof(Frame));
}
void readID3Metadata(ID3Metadata& data)
{
	readHeader(data);
	readFrames(data);
}
void writeHeaderToFile(const ID3Metadata& data)
{
	byte_t header[ID3HEADER_SIZE];
	for (size_t i = 0; i < 3; i++)
		header[i] = data.fileIdentifier[i];
	header[3] = data.majorVersion;
	header[4] = data.revisionVersion;
	header[5] = data.flags;
	size_t temp = data.realSize;
	for (size_t i = 0; i < 4; i++)
	{
		header[9 - i] = (byte_t)(temp & ~(1 << 7));
		temp >>= 7;
	}
	if (fwrite(header, sizeof(byte_t), ID3HEADER_SIZE, filePtr) != ID3HEADER_SIZE)
		errorWritingFile();
}
void writeFramesToFile(const ID3Metadata& data)
{
	for (size_t k = 0; k < data.framesCount; k++)
	{
		Frame& frame = data.frames[k];
		byte_t frameHeader[FRAME_HEADER_SIZE];
		for (size_t i = 0; i < 4; i++)
			frameHeader[i] = frame.name[i];
		size_t temp = frame.dataSize;
		for (size_t i = 0; i < 4; i++)
		{
			frameHeader[7 - i] = (byte_t)temp;
			temp >>= 8;
		}
		frameHeader[8] = frame.flags1;
		frameHeader[9] = frame.flags2;
		if (fwrite(frameHeader, sizeof(byte_t), FRAME_HEADER_SIZE, filePtr) != FRAME_HEADER_SIZE)
			errorWritingFile();
		if (fwrite(frame.data, sizeof(byte_t), frame.dataSize, filePtr) != frame.dataSize)
			errorWritingFile();
	}
}
void writeToFile(const ID3Metadata& data)
{
	writeHeaderToFile(data);
	writeFramesToFile(data);
}