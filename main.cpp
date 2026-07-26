#include <fstream>
#include <iostream>

bool checkRiff(std::ifstream& file)
{
	char id[4];

	file.read(id, 4);
	if (!file)
		return false;
	if (id[0] == 'R' && id[1] == 'I' && id[2] == 'F' && id[3] == 'F')
		return true;
	else
		return false;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong number of argument. Usage: execute the binary with the wav file name" << std::endl;
		return (0);
	}
	std::ifstream file(argv[1], std::iostream::binary);
	if (!file)
	{
		std::cerr << "wav file can't be opened" << std::endl;
		return 1;
	}
	if (checkRiff(file))
		std::cout << "RIFF OK" << std::endl;
	else
		std::cout << "RIFF not OK" << std::endl;
}
