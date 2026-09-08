
#include <iostream>
#include <string>
#include <filesystem>
#include "Wav.hpp"

#ifdef _WIN32
# include <windows.h>
#endif

void print_banner(void)
{
	std::cout << std::endl;
	std::cout << "╔═══════════════════════════════════════════════════════════════════════╗" << std::endl;
	std::cout << "║                                                                       ║" << std::endl;
	std::cout << "║  ██╗    ██╗ █████╗ ██╗   ██╗       ██████╗ ██████╗ ██╗   ██╗██████╗   ║" << std::endl;
	std::cout << "║  ██║    ██║██╔══██╗██║   ██║      ██╔════╝ ██╔══██╗██║   ██║██╔══██╗  ║" << std::endl;
	std::cout << "║  ██║ █╗ ██║███████║██║   ██║█████╗██║  ███╗██████╔╝██║   ██║██████╔╝  ║" << std::endl;
	std::cout << "║  ██║███╗██║██╔══██║╚██╗ ██╔╝╚════╝██║   ██║██╔══██╗██║   ██║██╔══██╗  ║" << std::endl;
	std::cout << "║  ╚███╔███╔╝██║  ██║ ╚████╔╝       ╚██████╔╝██║  ██║╚██████╔╝██████╔╝  ║" << std::endl;
	std::cout << "║   ╚══╝╚══╝ ╚═╝  ╚═╝  ╚═══╝         ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═════╝   ║" << std::endl;
	std::cout << "║                                                                       ║" << std::endl;
	std::cout << "╚═══════════════════════════════════════════════════════════════════════╝" << std::endl << std::endl;
}

void print_menu()
{
	std::cout << std::endl;
	std::cout << "╔══════════════════════════════════════╗" << std::endl;
	std::cout << "║               WAV_GRUB               ║" << std::endl;
	std::cout << "╠══════════════════════════════════════╣" << std::endl;
	std::cout << "║  0 - Quit                            ║" << std::endl;
	std::cout << "║  1 - Format info                     ║" << std::endl;
	std::cout << "║  2 - iXML info                       ║" << std::endl;
	std::cout << "║  3 - Bext info                       ║" << std::endl;
	std::cout << "║  4 - Time info                       ║" << std::endl;
	std::cout << "║  5 - Print brut data                 ║" << std::endl;
	std::cout << "╚══════════════════════════════════════╝" << std::endl;
}

bool	is_valid_path_wav(std::string const & input)
{
	std::filesystem::path path = input;

	if (!std::filesystem::exists(path))
	{
		std::cout << "The path does not exist." << std::endl;
		return false;
	}
	if (!std::filesystem::is_regular_file(path))
	{
		std::cout << "The path is not a file." << std::endl;
		return false;
	}
	if (!input.ends_with(".wav"))
	{
		std::cout << "Input is not a .wav file" << std::endl;
		return false;
	}
	return true;
}

bool set_filename(std::string & filename, int argc, char **argv)
{
	if (argc == 2)
	{
		if (is_valid_path_wav(argv[1]))
		{
			filename = argv[1];
			return true;
		}
	}

	while (filename.empty())
	{
		std::string input;
		std::cout << "Enter .wav path to grub (q to quit): ";
		std::cin >> input;

		if (input == "q" || input == "Q")
			return false;
		if (is_valid_path_wav(input))
			filename = input;
	}
	return true;
}

int get_menu_choice()
{
	std::string input;

	while (true)
	{
		print_menu();
		std::cout << "Choice: ";
		if (!(std::cin >> input))
		{
			std::cin.clear();
			return 0;
		}

		if (input == "0")
			return 0;
		if (input == "1")
			return 1;
		if (input == "2")
			return 2;
		if (input == "3")
			return 3;
		if (input == "4")
			return 4;
		if (input == "5")
			return 5;

		std::cout << "Invalid choice.\n";
	}
}

void menu(Wav const & wav)
{
	while (true)
	{
		int choice = get_menu_choice();

		switch (choice)
		{
			case 0:
				return ;

			case 1:
				wav.print_fmt_info();
				break;

			case 2:
				wav.print_iXML_info();
				break;

			case 3:
				wav.printBextInfo();
				break;

			case 4:
				wav.print_time_info();
				break;

			case 5:
				wav.take_a_look_to_data();
				break;
		}

		std::cout << "\nPress Enter to return to menu...";
		std::cin.ignore();
		std::cin.get();
	}
}

int	main(int argc, char **argv)
{
	#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	#endif
	
	if (argc > 2)
	{
		std::cout	<< "Wrong number of argument." << std::endl
					<< "Usage 1: ./wav_viewer" << std::endl
					<< "Usage 2: ./wav_viewer <wav_file>" << std::endl;
		return (1);
	}
	print_banner();

	std::string filename;
	if (!set_filename(filename, argc, argv))
		return (0);
	try
	{
		Wav wav(filename);
		std::cout << std::endl << filename << " size : " << wav.fileSize() << std::endl;
		menu(wav);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
