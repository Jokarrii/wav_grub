
#include "wav_viewer.hpp"

bool checkID(std::ifstream& file, const char expected[4])
{
	char id[4];

	file.read(id, 4);
	if (!file)
		return false;
	return (std::memcmp(id, expected, 4) == 0);
}

int	main(int argc, char **argv)
{
	uint32_t fileSize;

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
	file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
	std::cout << "Taille du fichier: " << fileSize << std::endl;
	if (checkID(file, "WAVE"))
		std::cout << "WAVE OK" << std::endl;
	else
	{
		std::cout << "WAVE not OK" << std::endl;
		return 1;
	}
	manageChunk(file);
	std::cout << "Position : " << file.tellg() << std::endl;
}
