#include "Wav.hpp"

void Wav::printPCM8(unsigned int frameNb) const
{
	std::cout << std::endl << "Take a look to the " << frameNb << " first frame data" << std::endl << std::endl;
	std::cout << std::left << std::setw(12) << "";
	for (int i = 1; i <= _channels; ++i)
		std::cout << std::right << std::setw(15) << "Canal " + std::to_string(i);
	std::cout << std::endl;

	uint8_t sample;

	for (unsigned int i = 0; i < frameNb; ++i)
	{
		std::cout << std::left << std::setw(12) << ("Frame " + std::to_string(i));
		for (int j = 0; j < _channels; ++j)
		{
			sample = std::to_integer<uint8_t>(_data[i * _channels + j]);
			std::cout << std::right << std::setw(15) << static_cast<unsigned int>(sample);
		}
		std::cout << std::endl;
	}
}

void Wav::printPCM16(unsigned int frameNb) const
{
	std::cout << std::endl << "Take a look to the " << frameNb << " first frame data" << std::endl << std::endl;
	std::cout << std::left << std::setw(12) << "";

	for (int i = 1; i <= _channels; ++i)
		std::cout << std::right << std::setw(15) << "Canal " + std::to_string(i);
	std::cout << std::endl;

	int16_t sample;

	for (unsigned int i = 0; i < frameNb; ++i)
	{
		std::cout << std::left << std::setw(12) << ("Frame " + std::to_string(i));

		for (unsigned int j = 0; j < _channels; ++j)
		{
			std::size_t index = (i * _channels + j) * 2;

			uint8_t low = std::to_integer<uint8_t>(_data[index]);
			uint8_t high = std::to_integer<uint8_t>(_data[index + 1]);

			uint16_t value = low | (static_cast<uint16_t>(high) << 8);

			sample = static_cast<int16_t>(value);

			std::cout << std::right << std::setw(15) << sample;
		}
		std::cout << std::endl;
	}
}

void Wav::printPCM24(unsigned int frameNb) const
{
	std::cout << std::endl << "Take a look to the " << frameNb << " first frame data" << std::endl << std::endl;
	std::cout << std::left << std::setw(12) << "";

	for (unsigned int i = 1; i <= _channels; ++i)
		std::cout << std::right << std::setw(15) << "Canal " + std::to_string(i);
	std::cout << std::endl;

	int32_t sample;

	for (unsigned int i = 0; i < frameNb; ++i)
	{
		std::cout << std::left << std::setw(12) << ("Frame " + std::to_string(i));

		for (unsigned int j = 0; j < _channels; ++j)
		{
			std::size_t index = (i * _channels + j) * 3;

			uint32_t byte0 = std::to_integer<uint8_t>(_data[index]);
			uint32_t byte1 = std::to_integer<uint8_t>(_data[index + 1]);
			uint32_t byte2 = std::to_integer<uint8_t>(_data[index + 2]);

			uint32_t value =
				byte0
				| (byte1 << 8)
				| (byte2 << 16);

			if (value & 0x800000)
				value |= 0xFF000000;

			sample = static_cast<int32_t>(value);

			std::cout << std::right << std::setw(15) << sample;
		}
		std::cout << std::endl;
	}
}

void Wav::printPCM32(unsigned int frameNb) const
{
	std::cout << std::endl << "Take a look to the " << frameNb << " first frame data" << std::endl << std::endl;
	std::cout << std::left << std::setw(12) << "";

	for (unsigned int i = 1; i <= _channels; ++i)
		std::cout << std::right << std::setw(15) << "Canal " + std::to_string(i);
	std::cout << std::endl;

	int32_t sample;

	for (unsigned int i = 0; i < frameNb; ++i)
	{
		std::cout << std::left << std::setw(12) << ("Frame " + std::to_string(i));

		for (unsigned int j = 0; j < _channels; ++j)
		{
			std::size_t index = (i * _channels + j) * 4;

			uint32_t byte0 = std::to_integer<uint8_t>(_data[index]);
			uint32_t byte1 = std::to_integer<uint8_t>(_data[index + 1]);
			uint32_t byte2 = std::to_integer<uint8_t>(_data[index + 2]);
			uint32_t byte3 = std::to_integer<uint8_t>(_data[index + 3]);

			uint32_t value =
				byte0
				| (byte1 << 8)
				| (byte2 << 16)
				| (byte3 << 24);

			sample = static_cast<int32_t>(value);

			std::cout << std::right << std::setw(15) << sample;
		}
		std::cout << std::endl;
	}
}

void Wav::printFloat32(unsigned int frameNb) const
{
	std::cout << std::endl << "Take a look to the " << frameNb << " first frame data" << std::endl << std::endl;
	std::cout << std::left << std::setw(12) << "";

	for (unsigned int i = 1; i <= _channels; ++i)
		std::cout << std::right << std::setw(15) << "Canal " + std::to_string(i);
	std::cout << std::endl;

	float sample;

	for (unsigned int i = 0; i < frameNb; ++i)
	{
		std::cout << std::left << std::setw(12) << ("Frame " + std::to_string(i));

		for (unsigned int j = 0; j < _channels; ++j)
		{
			std::size_t index = (i * _channels + j) * 4;

			uint32_t byte0 = std::to_integer<uint8_t>(_data[index]);
			uint32_t byte1 = std::to_integer<uint8_t>(_data[index + 1]);
			uint32_t byte2 = std::to_integer<uint8_t>(_data[index + 2]);
			uint32_t byte3 = std::to_integer<uint8_t>(_data[index + 3]);

			uint32_t value =
				byte0
				| (byte1 << 8)
				| (byte2 << 16)
				| (byte3 << 24);

			std::memcpy(&sample, &value, sizeof(sample));

			std::cout << std::right << std::setw(15) << sample;
		}
		std::cout << std::endl;
	}
}

void Wav::printFloat64(unsigned int frameNb) const
{
	std::cout << std::endl << "Take a look to the " << frameNb << " first frame data" << std::endl << std::endl;
	std::cout << std::left << std::setw(12) << "";

	for (unsigned int i = 1; i <= _channels; ++i)
		std::cout << std::right << std::setw(15) << "Canal " + std::to_string(i);
	std::cout << std::endl;

	double sample;

	for (unsigned int i = 0; i < frameNb; ++i)
	{
		std::cout << std::left << std::setw(12) << ("Frame " + std::to_string(i));

		for (unsigned int j = 0; j < _channels; ++j)
		{
			std::size_t index = (i * _channels + j) * 8;

			uint64_t byte0 = std::to_integer<uint8_t>(_data[index]);
			uint64_t byte1 = std::to_integer<uint8_t>(_data[index + 1]);
			uint64_t byte2 = std::to_integer<uint8_t>(_data[index + 2]);
			uint64_t byte3 = std::to_integer<uint8_t>(_data[index + 3]);
			uint64_t byte4 = std::to_integer<uint8_t>(_data[index + 4]);
			uint64_t byte5 = std::to_integer<uint8_t>(_data[index + 5]);
			uint64_t byte6 = std::to_integer<uint8_t>(_data[index + 6]);
			uint64_t byte7 = std::to_integer<uint8_t>(_data[index + 7]);

			uint64_t value =
				byte0
				| (byte1 << 8)
				| (byte2 << 16)
				| (byte3 << 24)
				| (byte4 << 32)
				| (byte5 << 40)
				| (byte6 << 48)
				| (byte7 << 56);

			std::memcpy(&sample, &value, sizeof(sample));

			std::cout << std::right << std::setw(15) << sample;
		}
		std::cout << std::endl;
	}
}
