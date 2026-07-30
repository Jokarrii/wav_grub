#ifndef WAV_VIEWER_HPP
#define WAV_VIEWER_HPP

#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstring>

struct FmtChunk
{
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
};

#endif
