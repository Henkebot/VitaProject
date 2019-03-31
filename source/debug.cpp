#include "debug.h"

#include <psp2/io/fcntl.h>

const unsigned int FILE_TEMP_BUFFER_SIZE = 256;

static SceUID fileHandle = 0;
static char pBuffer[FILE_TEMP_BUFFER_SIZE];

///////////////////////////////////////////
void InitDebugFile()
{	
	// Only Init once
	if(0 != fileHandle)
		return;

	fileHandle = sceIoOpen("ux0:data/debugfile.txt", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	
// If the file open fails, there is no way to get this information formally..
	if(fileHandle < 0)
		return;	

	// Reset the buffer
	for(auto& c : pBuffer)
	{
		c = '\0';
	}


}

///////////////////////////////////////////
void PrintToFile(const char* _pMsg)
{
	if(_pMsg == nullptr)
		return;

	unsigned int i = 0;	

	for(; 
	i < FILE_TEMP_BUFFER_SIZE && 
	_pMsg[i] != '\0'; i++)
	{
		pBuffer[i] = _pMsg[i];
	}
	pBuffer[i] = '\0';
	
	sceIoWrite(fileHandle,reinterpret_cast<const void*>(pBuffer),i);
}
__attribute__((__format__ (__printf__,1,2)))
void PrintError(const char* _pFormat,...)
{
	char pBuffer[256];

	va_list opt;
	va_start(opt,_pFormat);
	vsnprintf(pBuffer,sizeof(pBuffer),_pFormat,opt);
	PrintToFile(pBuffer);
	va_end(opt);
}




