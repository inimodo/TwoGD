#include "..\twogd.h"

using namespace std::chrono;

perlog::perlog()
{

}

perlog::perlog(int i_BufferSize_)
{
	i_BufferSize = i_BufferSize_;
	i_Buffer = (int*)malloc(sizeof(int) * i_BufferSize);
	if (i_Buffer == NULL)
	{
		i_BufferSize = 0;
		return;
	}
	for (int i_Index = 0; i_Index < i_BufferSize; i_Index++)
	{
		i_Buffer[i_Index] = 0;
	}
}

void perlog::Start()
{
	a_Start = high_resolution_clock::now();
}

void perlog::Stop()
{
	a_Stop = high_resolution_clock::now();
	memmove(i_Buffer + 1, i_Buffer, (i_BufferSize - 1) * sizeof(long long));
	i_Buffer[0] = (int)duration_cast<milliseconds>(a_Stop - a_Start).count();
}

float perlog::GetDelta()
{
	int i_Avr = 0;
	for (int i_Index = 0; i_Index < i_BufferSize; i_Index++)
	{
		i_Avr += i_Buffer[i_Index];
	}
	return (float)i_Avr / (float)i_BufferSize;
}

uint8_t perlog::Dispose()
{
	if (i_Buffer != NULL)
	{
		free(i_Buffer);
	}
	return GD_TASK_OKAY;
}