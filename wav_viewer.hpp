#ifndef WAV_VIEWER_HPP
#define WAV_VIEWER_HPP

#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstring>

enum SampleFormat
{
	PCM_U8,
	PCM_S16,
	PCM_S24,
	PCM_S32,
	FLOAT32,
	UNKNOWN
};

struct AudioBuffer
{
	void* data;
	SampleFormat format;
	uint32_t sampleCount;
};

struct FmtChunk
{
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
};

void	exit_prog(int status, int free_type);
void	manageChunk(std::ifstream& file);

#endif
