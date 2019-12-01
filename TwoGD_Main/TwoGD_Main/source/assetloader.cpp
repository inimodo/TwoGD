#include "twogd.h"

__STATUS __WAY
mapformat::CloseStream() 
{
	if (this->f_Stream != NULL) 
	{
		fclose(this->f_Stream);
	}
}
__STATUS __WAY
mapformat::OpenStream(const char* c_StreamName)
{
	if (this->f_Stream == NULL) 
	{
		this->f_Stream = fopen(c_StreamName, "r");
		if (this->f_Stream == NULL)
		{
			return GD_FILE_FAILED;
		}
		return GD_TASK_OKAY;
	}
	return GD_FILE_FAILED;
}