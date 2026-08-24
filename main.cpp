
#include <iostream>
#include <string>
#include "Wav.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong number of argument. Usage: ./wav_viewer <wav_file>" << std::endl;
		return (0);
	}

	try
	{
		Wav wav(argv[1]);

		std::cout << "File size: " << wav.fileSize() << std::endl;
		std::cout << "Audio format: " << wav.audioFormat() << std::endl;
		std::cout << "Channels: " << wav.channels() << std::endl;
		std::cout << "sample rate: " << wav.sampleRate() << std::endl;
		std::cout << "byte rate: " << wav.byteRate() << std::endl;
		std::cout << "Block align: " << wav.blockAlign() << std::endl;
		std::cout << "bits per sample: " << wav.bitsPerSample() << std::endl;
		std::cout << "Data size: " << wav.dataSize() << std::endl;
		std::cout << "Sample count: " << wav.sampleCount() << std::endl;
		std::cout << "Frame count: " << wav.frameCount() << std::endl;

		std::string input;

		while (true)
		{
			std::cout << "\nEnter number of frames to display brut data (q to quit): ";

			std::cin >> input;

			if (input == "q" || input == "Q")
				break;

			try
			{
				unsigned int frameNb = std::stoul(input);

				wav.take_a_look_to_data(frameNb);
			}
			catch (const std::exception&)
			{
				std::cout << "Invalid input." << std::endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
