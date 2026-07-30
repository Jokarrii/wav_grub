#include "wav_viewer.hpp"

bool checkID(std::ifstream& file, const char expected[4])
{
	char id[4];

	file.read(id, 4);
	if (!file)
		return false;
	return (std::memcmp(id, expected, 4) == 0);
}

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

int	main(int argc, char **argv)
{
	FmtChunk fmt;

	if (argc != 2)
	{
		std::cout << "Wrong number of argument. Usage: execute the binary with the wav file name" << std::endl;
		return (0);
	}
	std::ifstream file(argv[1], std::ios::binary);
	if (!file)
	{
		std::cerr << "wav file can't be opened" << std::endl;
		return 1;
	}

	if (checkID(file, "RIFF"))
		std::cout << "RIFF OK" << std::endl;
	else
	{
		std::cout << "RIFF not OK" << std::endl;
		return 1;
	}

	uint32_t fileSize;

	file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
	std::cout << "Taille du fichier: " << fileSize << std::endl;

	if (checkID(file, "WAVE"))
		std::cout << "WAVE OK" << std::endl;
	else
	{
		std::cout << "WAVE not OK" << std::endl;
		return 1;
	}

	char chunkID[4];
	uint32_t chunkSize;

	while (readChunkHeader(file, chunkID, chunkSize))
	{
		std::cout << "Chunk : ";

		for (int i = 0; i < 4; i++)
			std::cout << chunkID[i];
		std::cout << " taille : " << chunkSize << std::endl;
		if (memcmp(chunkID, "fmt ", 4) == 0)
		{
			if (chunkSize != sizeof(FmtChunk))
			{
				std::cerr << "fmt inattendu\n";
				return 1;
			}
			file.read(reinterpret_cast<char*>(&fmt), chunkSize);
			if (!file)
			{
				std::cerr << "Erreur de lecture du chunk fmt\n";
				return 1;
			}
			std::cout << "audioFormat: " << fmt.audioFormat << std::endl;
			std::cout << "numChannels: " << fmt.numChannels << std::endl;
			std::cout << "sampleRate: " << fmt.sampleRate << std::endl;
			std::cout << "byteRate: " << fmt.byteRate << std::endl;
			std::cout << "blockAlign: " << fmt.blockAlign << std::endl;
			std::cout << "bitsPerSample: " << fmt.bitsPerSample << std::endl;
		}
		if (memcmp(chunkID, "data", 4) == 0)
			file.seekg(chunkSize, std::ios::cur);
	}

	std::cout << "Position : " << file.tellg() << std::endl;
}
