#include "wav_viewer.hpp"

void	exit_prog(int status, int free_type)
{
	if (!free_type)
		exit(status);
	else
	{
		//free...
		exit(status);
	}
}
