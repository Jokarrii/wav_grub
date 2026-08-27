#include "Wav.hpp"

/*-------------- builder -----------------*/

Wav::Wav(const std::string& filename)
{
	std::cout << "---- " << filename << " acquisition ----" << std::endl << std::endl;
	std::ifstream file(filename, std::ios::binary);
	if (!file)
		throw std::runtime_error("wav file can't be opened");
	if (!checkID(file, "RIFF"))
		throw std::runtime_error("Unsupported file format (RIFF chunk expected)");
	file.read(reinterpret_cast<char*>(&_fileSize), sizeof(_fileSize));
	if (!checkID(file, "WAVE"))
		throw std::runtime_error("Unsupported file format (WAVE chunk expected)");
	readChunks(file);
}

/*---------------- getter -----------------*/

uint32_t Wav::fileSize() const
{
	return (_fileSize);
}

uint16_t Wav::audioFormat() const
{
	return (_audioFormat);
}

uint16_t Wav::channels() const
{
	return (_channels);
}

uint32_t Wav::sampleRate() const
{
	return (_sampleRate);
}

uint32_t Wav::byteRate() const
{
	return (_byteRate);
}

uint16_t Wav::blockAlign() const
{
	return (_blockAlign);
}

uint16_t Wav::bitsPerSample() const
{
	return (_bitsPerSample);
}

uint32_t Wav::dataSize() const
{
	return (_dataSize);
}

void Wav::print_fmt_info() const
{
		std::cout << std::endl << "Audio format: " << this->audioFormat() << std::endl;
		std::cout << "Channels: " << this->channels() << std::endl;
		std::cout << "sample rate: " << this->sampleRate() << std::endl;
		std::cout << "byte rate: " << this->byteRate() << std::endl;
		std::cout << "Block align: " << this->blockAlign() << std::endl;
		std::cout << "bits per sample: " << this->bitsPerSample() << std::endl;
		std::cout << "Data size: " << this->dataSize() << std::endl;
}

/*-------------- time info ---------------*/

uint32_t Wav::sampleCount() const
{
	return (_dataSize / (_bitsPerSample / 8));
}

uint32_t Wav::framesCount() const
{
	return (sampleCount() / _channels);
}

void Wav::wav_time() const
{
	uint32_t Frames = framesCount();
	uint32_t Millisecond;
	uint32_t SS;
	uint32_t MM;
	uint32_t HH;

	SS = Frames / _sampleRate;
	Frames %= _sampleRate;
	Millisecond = Frames * 1000 / _sampleRate;

	std::cout	<< "Second.Millisecond :  " << SS << "."
				<< std::setfill('0') << std::setw(4) << Millisecond << std::endl;

	MM = SS / 60;
	SS %= 60;
	HH = MM / 60;
	MM %= 60;

	std::cout	<< "HH:MM:SS:frames :  "
				<< std::setfill('0') << std::setw(2) << HH << ":"
				<< std::setw(2) << MM << ":"
				<< std::setw(2) << SS << ":"
				<< Frames << std::endl;
}

void Wav::print_time_info() const
{
	std::cout << "Sample count: " << this->sampleCount() << std::endl;
	std::cout << "Frame count: " << this->framesCount() << std::endl;
	this->wav_time();
}

/*------------ audio analysis -------------*/

void Wav::print_loudness_info() const
{
	std::cout << "Loudness info be implemented soon !!" << std::endl;
}


/*-------------- Data look ---------------*/

void Wav::take_a_look_to_data() const
{
	std::string input;

	while (true)
	{
		std::cout << "\nEnter number of frames to display brut data (m to return to menu): ";
		std::cin >> input;

		if (input == "m" || input == "M")
			return;
		try
		{
			uint32_t frameNb = std::stoul(input);
			if (frameNb > framesCount())
			{
				std::cout << "Max frame : " << framesCount() << std::endl;
				frameNb = framesCount();
			}
			switch (_format)
			{
				case SampleFormat::PCM_U8:
					printPCM8(frameNb);
					break;
				case SampleFormat::PCM_S16:
					printPCM16(frameNb);
					break;
				case SampleFormat::PCM_S24:
					printPCM24(frameNb);
					break;
				case SampleFormat::PCM_S32:
					printPCM32(frameNb);
					break;
				case SampleFormat::FLOAT32:
					printFloat32(frameNb);
					break;
				case SampleFormat::FLOAT64:
					printFloat64(frameNb);
					break;
				default:
					throw std::runtime_error("Unsupported sample format");
			}
			break;
		}
		catch (const std::exception&)
		{
			std::cout << "Invalid input." << std::endl;
		}
	}
}

void Wav::visualiser() const
{
	std::cout << "Visualiser be implemented soon !!" << std::endl;
}

/*---------------- member fonction -----------------*/

Wav::SampleFormat Wav::determineFormat() const
{
	switch (_audioFormat)
	{
		case 1:

			switch (_bitsPerSample)
			{
				case 8:  return SampleFormat::PCM_U8;
				case 16: return SampleFormat::PCM_S16;
				case 24: return SampleFormat::PCM_S24;
				case 32: return SampleFormat::PCM_S32;
				default: return SampleFormat::UNKNOWN;
			}

		case 3:

			switch (_bitsPerSample)
			{
				case 32: return SampleFormat::FLOAT32;
				case 64: return SampleFormat::FLOAT64;
				default: return SampleFormat::UNKNOWN;
			}

		default:
			return SampleFormat::UNKNOWN;
	}
}

bool Wav::checkID(std::ifstream& file, const char expected[4])
{
	char id[4];

	file.read(id, 4);
	if (!file)
		return false;
	return (std::memcmp(id, expected, 4) == 0);
}

bool Wav::readChunkHeader(std::ifstream& file, char id[4], uint32_t& size)
{
	file.read(id, 4);
	if (!file)
		return false;
	file.read(reinterpret_cast<char*>(&size), sizeof(size));
	if (!file)
		return false;
	return true;
}

void Wav::readChunks(std::ifstream& file)
{
	char		chunkID[4];
	uint32_t	chunkSize;

	while (readChunkHeader(file, chunkID, chunkSize))
	{
		if (memcmp(chunkID, "fmt ", 4) == 0)
			readFmtChunk(file, chunkSize);
		else if (memcmp(chunkID, "data", 4) == 0)
			readDataChunk(file, chunkSize);
		else if (memcmp(chunkID, "JUNK", 4) == 0)
			file.seekg(chunkSize, std::ios::cur);
		else
			readOtherChunk(file, chunkID, chunkSize);
	}
}

void Wav::readFmtChunk(std::ifstream& file, uint32_t chunkSize)
{
	if (chunkSize < 16)
		throw std::runtime_error("Unsupported file format (fmt chunk size invalid)");
	file.read(reinterpret_cast<char*>(&_audioFormat), sizeof(_audioFormat));
	file.read(reinterpret_cast<char*>(&_channels), sizeof(_channels));
	file.read(reinterpret_cast<char*>(&_sampleRate), sizeof(_sampleRate));
	file.read(reinterpret_cast<char*>(&_byteRate), sizeof(_byteRate));
	file.read(reinterpret_cast<char*>(&_blockAlign), sizeof(_blockAlign));
	file.read(reinterpret_cast<char*>(&_bitsPerSample), sizeof(_bitsPerSample));
	_format = determineFormat();
	if (chunkSize > 16)
		file.seekg(chunkSize - 16, std::ios::cur);
	std::cout << "fmt chunk acquisition completed" << std::endl;
}

void Wav::readDataChunk(std::ifstream& file, uint32_t chunkSize)
{
	_dataSize = chunkSize;
	_data.resize(chunkSize);
	file.read(reinterpret_cast<char*>(_data.data()), chunkSize);
	if (!file)
		throw std::runtime_error("Error while reading data chunk");
	std::cout << "DATA chunk acquisition completed" << std::endl;
}

void Wav::readOtherChunk(std::ifstream& file, char *chunkID, uint32_t chunkSize)
{
	std::vector<std::byte> data(chunkSize);

	file.read(reinterpret_cast<char*>(data.data()), chunkSize);
	if (!file)
		throw std::runtime_error("Error while reading Other chunk");

	std::cout << chunkID << " chunk (" << chunkSize << " bytes):" << std::endl;

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		uint8_t byte = std::to_integer<uint8_t>(data[i]);

		if (byte >= 32 && byte <= 126)
			std::cout << static_cast<char>(byte);
		else
			std::cout << '.';
	}

	std::cout << std::endl;
}
