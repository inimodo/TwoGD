#include "..\twogd.h"

using namespace std::chrono;

perlog::perlog()
{

}

perlog::perlog(int i_BufferSize)
{
	this->i_BufferSize = i_BufferSize;
	this->i_Buffer = (int*)malloc(sizeof(int)*i_BufferSize);
	if (this->i_Buffer == NULL) 
	{
		this->i_BufferSize = 0;
		return;
	}
	for (int i_Index = 0; i_Index < this->i_BufferSize; i_Index++)
	{
		this->i_Buffer[i_Index] = 0;
	}
}

void perlog::Start()
{
	a_Start = high_resolution_clock::now();
}

void perlog::Stop()
{
	a_Stop = high_resolution_clock::now();
	memmove(this->i_Buffer+1, this->i_Buffer, (this->i_BufferSize - 1)*sizeof(long long));
	this->i_Buffer[0] = (int)duration_cast<milliseconds>(a_Stop - a_Start).count();
}

float perlog::GetDelta()
{
	int i_Avr = 0;
	for (int i_Index = 0; i_Index < this->i_BufferSize; i_Index++)
	{
		i_Avr += this->i_Buffer[i_Index];
	}
	return (float)i_Avr / (float)this->i_BufferSize;
}

UCHAR perlog::Dispose() 
{
	if (this->i_Buffer != NULL) 
	{
		free(this->i_Buffer);
	}
	return GD_TASK_OKAY;
}