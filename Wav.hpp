#ifndef WAV_HPP
#define WAV_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>

class Wav
{
public:

	enum class SampleFormat
	{
		PCM_U8,
		PCM_S16,
		PCM_S24,
		PCM_S32,
		FLOAT32,
		FLOAT64,
		UNKNOWN
	};

/*-------------- builder -----------------*/
	explicit Wav(const std::string& filename);

/*---------------- getter -----------------*/
	uint32_t	fileSize() const;
	uint16_t	audioFormat() const;
	uint16_t	channels() const;
	uint32_t	sampleRate() const;
	uint32_t	byteRate() const;
	uint16_t	blockAlign() const;
	uint16_t	bitsPerSample() const;
	uint32_t	dataSize() const;
	/*print previously ^^ */
	void		print_fmt_info() const;

/*-------------- time info ---------------*/
	uint32_t	sampleCount() const;
	uint32_t	framesCount() const;
	void		wav_time() const;
	/*print previously ^^ */
	void		print_time_info() const;

/*------------ audio analysis -------------*/
	void		print_loudness_info() const;

/*-------------- Data look ---------------*/
	void		take_a_look_to_data() const;
	void		visualiser() const;
private:

	static bool checkID(std::ifstream& file, const char expected[4]);
	bool readChunkHeader(std::ifstream& file, char id[4], uint32_t& size);
	void readChunks(std::ifstream& file);
	void readFmtChunk(std::ifstream& file, uint32_t chunkSize);
	void readDataChunk(std::ifstream& file, uint32_t chunkSize);
	void readOtherChunk(std::ifstream& file, char *chunkID, uint32_t chunkSize);
	SampleFormat determineFormat() const;

	void printPCM8(unsigned int frameNb) const;
	void printPCM16(unsigned int frameNb) const;
	void printPCM24(unsigned int frameNb) const;
	void printPCM32(unsigned int frameNb) const;
	void printFloat32(unsigned int frameNb) const;
	void printFloat64(unsigned int frameNb) const;

	uint32_t				_fileSize{};
	uint16_t 				_audioFormat{};
	uint16_t 				_channels{};
	uint32_t 				_sampleRate{};
	uint32_t 				_byteRate{};
	uint16_t 				_blockAlign{};
	uint16_t 				_bitsPerSample{};
	SampleFormat			_format{SampleFormat::UNKNOWN};
	uint32_t 				_dataSize{};
	std::vector<std::byte>	_data;
};

#endif
