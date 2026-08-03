
#include <iostream>
#include <string>
#include <vector>
#include "Wav.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong number of argument. Usage: execute the binary with the wav file name" << std::endl;
		return (0);
	}
	std::string filename = argv[1];
	Wav wav1(filename);

	std::cout << "File size: " << wav1.fileSize() << std::endl;
	std::cout << "Audio format: " << wav1.audioFormat() << std::endl;
	std::cout << "Channels: " << wav1.channels() << std::endl;
	std::cout << "sample rate: " << wav1.sampleRate() << std::endl;
	std::cout << "byte rate: " << wav1.byteRate() << std::endl;
	std::cout << "Block align: " << wav1.blockAlign() << std::endl;
	std::cout << "bits per sample: " << wav1.bitsPerSample() << std::endl;
	std::cout << "Data size: " << wav1.dataSize() << std::endl;
	std::cout << "Sample count: " << wav1.sampleCount() << std::endl;
	std::cout << "Frame count: " << wav1.frameCount() << std::endl;

	return 0;
}
