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
	std::cout << "\n===== FORMAT INFO =====" << std::endl;
	std::cout << std::endl << "Audio format: " << this->audioFormat() << std::endl;
	std::cout << "Channels: " << this->channels() << std::endl;
	std::cout << "sample rate: " << this->sampleRate() << std::endl;
	std::cout << "byte rate: " << this->byteRate() << std::endl;
	std::cout << "Block align: " << this->blockAlign() << std::endl;
	std::cout << "bits per sample: " << this->bitsPerSample() << std::endl;
	std::cout << "Data size: " << this->dataSize() << std::endl;
}

void Wav::print_iXML_info() const
{
	if (_ixml.empty())
	{
		std::cout << "No iXML chunk in this wav file" << std::endl;
		return ;
	}
	std::cout << "iXML chunk:" << std::endl;
	std::cout << _ixml;
	std::cout << std::endl;
}

void Wav::printBextInfo() const
{
	if (!_bext.exists)
	{
		std::cout << "No bext chunk in this wav file" << std::endl;
		return ;
	}
	std::cout << "\n===== BEXT INFO =====" << std::endl;
	std::cout << "Description: " << _bext.description << std::endl;
	std::cout << "Originator: " << _bext.originator << std::endl;
	std::cout << "Originator Reference: " << _bext.originatorReference << std::endl;
	std::cout << "Origination Date: " << _bext.originationDate << std::endl;
	std::cout << "Origination Time: " << _bext.originationTime << std::endl;
	std::cout << "Time Reference Low: " << _bext.timeReferenceLow << std::endl;
	std::cout << "Time Reference High: " << _bext.timeReferenceHigh << std::endl;
	std::cout << "Version: " << _bext.version << std::endl;
	std::cout << "UMID: ";
	for (std::size_t i = 0; i < _bext.umid.size(); ++i)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<unsigned int>(_bext.umid[i]) << ' ';
	std::cout << std::dec << std::setfill(' ') << std::endl;
	std::cout << "LoudnessValue: " << _bext.LoudnessValue << std::endl;
	std::cout << "LoudnessRange: " << _bext.LoudnessRange << std::endl;
	std::cout << "MaxTruePeakLevel: " << _bext.MaxTruePeakLevel << std::endl;
	std::cout << "MaxMomentaryLoudness: " << _bext.MaxMomentaryLoudness << std::endl;
	std::cout << "MaxShortTermLoudness: " << _bext.MaxShortTermLoudness << std::endl;
	std::cout << "Coding History:" << std::endl;
	std::cout << _bext.codingHistory << std::endl;
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
	std::cout << std::dec << std::setfill(' ') << std::endl;
}

void Wav::print_time_info() const
{
	std::cout << "Sample count: " << this->sampleCount() << std::endl;
	std::cout << "Frame count: " << this->framesCount() << std::endl;
	this->wav_time();
}

/*-------------- Data look ---------------*/

void Wav::take_a_look_to_data() const
{
	std::string input;

	while (true)
	{
		std::cout << "\nEnter number of frames to display brut data (m to return to menu): ";
		if (!(std::cin >> input))
		{
			std::cin.clear();
			return ;
		}
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
		else if (memcmp(chunkID, "iXML", 4) == 0)
			readIXMLChunk(file, chunkSize);
		else if (memcmp(chunkID, "bext", 4) == 0)
			readBextChunk(file, chunkSize);
		else if (memcmp(chunkID, "JUNK", 4) == 0)
			file.seekg(chunkSize, std::ios::cur);
		else
			readOtherChunk(file, chunkID, chunkSize);

		if (chunkSize % 2 != 0)
			file.seekg(1, std::ios::cur);
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
	if (!file)
		throw std::runtime_error("Error while reading fmt chunk");
	_format = determineFormat();
	if (chunkSize > 16)
		file.seekg(chunkSize - 16, std::ios::cur);
	std::cout << "fmt  chunk acquisition completed" << std::endl;
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

void Wav::readIXMLChunk(std::ifstream& file, uint32_t chunkSize)
{
	_ixml.resize(chunkSize);

	file.read(_ixml.data(), chunkSize);

	if (!file)
		throw std::runtime_error("Error while reading iXML chunk");
	std::cout << "iXML chunk acquisition completed" << std::endl;
}

void Wav::readBextChunk(std::ifstream& file, uint32_t chunkSize)
{
	if (chunkSize < 602)
		throw std::runtime_error("Unsupported bext chunk size");

	_bext.description.resize(256);
	file.read(_bext.description.data(), 256);
	_bext.originator.resize(32);
	file.read(_bext.originator.data(), 32);
	_bext.originatorReference.resize(32);
	file.read(_bext.originatorReference.data(), 32);
	_bext.originationDate.resize(10);
	file.read(_bext.originationDate.data(), 10);
	_bext.originationTime.resize(8);
	file.read(_bext.originationTime.data(), 8);
	file.read(reinterpret_cast<char*>(&_bext.timeReferenceLow), sizeof(_bext.timeReferenceLow));
	file.read(reinterpret_cast<char*>(&_bext.timeReferenceHigh), sizeof(_bext.timeReferenceHigh));
	file.read(reinterpret_cast<char*>(&_bext.version), sizeof(_bext.version));

	_bext.umid.resize(64);
	file.read(reinterpret_cast<char*>(_bext.umid.data()), 64);

	file.read(reinterpret_cast<char*>(&_bext.LoudnessValue), sizeof(_bext.LoudnessValue));
	file.read(reinterpret_cast<char*>(&_bext.LoudnessRange), sizeof(_bext.LoudnessRange));
	file.read(reinterpret_cast<char*>(&_bext.MaxTruePeakLevel), sizeof(_bext.MaxTruePeakLevel));
	file.read(reinterpret_cast<char*>(&_bext.MaxMomentaryLoudness), sizeof(_bext.MaxMomentaryLoudness));
	file.read(reinterpret_cast<char*>(&_bext.MaxShortTermLoudness), sizeof(_bext.MaxShortTermLoudness));

	_bext.reserved.resize(180);
	file.read(reinterpret_cast<char*>(_bext.reserved.data()), 180);

	std::size_t codingHistorySize = chunkSize - 602;
	_bext.codingHistory.resize(codingHistorySize);
	file.read(_bext.codingHistory.data(), codingHistorySize);

	if (!file)
		throw std::runtime_error("Error while reading bext chunk");

	std::cout << "bext chunk acquisition completed" << std::endl;
	_bext.exists = true;
}

void Wav::readOtherChunk(std::ifstream& file, char *chunkID, uint32_t chunkSize)
{
	std::vector<std::byte> data(chunkSize);

	file.read(reinterpret_cast<char*>(data.data()), chunkSize);
	if (!file)
		throw std::runtime_error("Error while reading Other chunk");

	std::cout.write(chunkID, 4);
	std::cout << " chunk (" << chunkSize << " bytes):" << std::endl;

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
