#include "..\twogd.h"

void filer::CloseStream()
{
	if (f_Stream != NULL)
	{
		fclose(f_Stream);
	}
}

uint8_t filer::OpenStream(LPSTR c_StreamName)
{
	if (f_Stream == NULL)
	{
		f_Stream = fopen(c_StreamName, "r");

		if (f_Stream == NULL)
		{
			return GD_FILE_FAILED;
		}
		return GD_TASK_OKAY;
	}
	return GD_FILE_FAILED;
}