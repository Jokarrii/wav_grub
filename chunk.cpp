#include "wav_viewer.hpp"

bool readChunkHeader(std::ifstream& file, char id[4], uint32_t& size)
{
	file.read(id, 4);
	if (!file)
		return false;
	file.read(reinterpret_cast<char*>(&size), sizeof(size));
	if (!file)
		return false;
	return true;
}

void printFmt(FmtChunk const & fmt)
{
	std::cout << "audioFormat: " << fmt.audioFormat << std::endl;
	std::cout << "numChannels: " << fmt.numChannels << std::endl;
	std::cout << "sampleRate: " << fmt.sampleRate << std::endl;
	std::cout << "byteRate: " << fmt.byteRate << std::endl;
	std::cout << "blockAlign: " << fmt.blockAlign << std::endl;
	std::cout << "bitsPerSample: " << fmt.bitsPerSample << std::endl;
}

void fmtChunk(FmtChunk & fmt, uint32_t const & chunkSize, std::ifstream & file)
{
	if (chunkSize != sizeof(FmtChunk))
	{
		std::cerr << "fmt inattendu\n";
		exit_prog(1, 0);
	}
	file.read(reinterpret_cast<char*>(&fmt), chunkSize);
	if (!file)
	{
		std::cerr << "Erreur de lecture du chunk fmt\n";
		exit_prog(1, 0);
	}
	printFmt(fmt);
}

SampleFormat getSampleFormat(const FmtChunk & fmt)
{
	if (fmt.audioFormat == 3)
		return (FLOAT32);
	else if (fmt.audioFormat == 1)
	{
		if (fmt.byteRate == 8)
			return (PCM_U8);
		else if (fmt.byteRate == 16)
			return (PCM_S16);
		else if (fmt.byteRate == 24)
			return (PCM_S24);
		else if (fmt.byteRate == 32)
			return (PCM_S32);
		else
			return (UNKNOWN);
	}
	else
		return (UNKNOWN);
}

void dataChunk(AudioBuffer & buffer, FmtChunk const & fmt, uint32_t const & chunkSize, std::ifstream & file)
{
	buffer.sampleCount = chunkSize / (fmt.bitsPerSample / 8);
	buffer.format = getSampleFormat(fmt);
	//comment faire ?
}

void manageChunk(std::ifstream& file)
{
	char		chunkID[4];
	uint32_t	chunkSize;
	FmtChunk	fmt;
	AudioBuffer buffer;

	while (readChunkHeader(file, chunkID, chunkSize))
	{
		std::cout << "Chunk : ";

		for (int i = 0; i < 4; i++)
			std::cout << chunkID[i];
		std::cout << " -> taille : " << chunkSize << std::endl;
		if (memcmp(chunkID, "fmt ", 4) == 0)
			fmtChunk(fmt, chunkSize, file);
		else if (memcmp(chunkID, "data", 4) == 0)
			dataChunk(buffer, fmt, chunkSize, file);
		else
			file.seekg(chunkSize, std::ios::cur);
	}
}
