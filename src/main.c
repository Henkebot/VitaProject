/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/clib.h>

#include <vita2d.h>

#include "debugScreen.h"

const char* pfilepath = "app0:/res/sample.png";


#define ALIGN(x,a) (((x) + ((a) -1)) & ~((a)-1))

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define printf psvDebugScreenPrintf

#define PRINT_ERROR(msg) {psvDebugScreenPrintf("Error at %s: %s\n",AT,msg); for(;;);}



void* cpu_alloc(unsigned int _size, SceUID* _pUid)
{
	void* pMem;

	_size = ALIGN(_size,4*1024);

	*_pUid = sceKernelAllocMemBlock("mem_alloc",SCE_KERNEL_MEMBLOCK_TYPE_USER_RW,_size,NULL);

	if(*_pUid < 0)
	{
		PRINT_ERROR("Failed to sceKernelAllocMemBlock()");
		return NULL;
	}

	if(sceKernelGetMemBlockBase(*_pUid,(void**)&pMem) < 0)
	{
		PRINT_ERROR("Failed to sceKernelGetMemBlockBase()");
		return NULL;
	}

	return pMem;
	
}

int main()
{
	SceCtrlData pad;
	vita2d_pgf *pgf;
	vita2d_pvf *pvf;
	float rad = 0.0f;

	psvDebugScreenInit();

	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	pgf = vita2d_load_default_pgf();
	pvf = vita2d_load_default_pvf();

	SceUID fd = sceIoOpen(pfilepath, SCE_O_RDONLY, 0777);

	SceIoStat info;
	sceIoGetstat(pfilepath,&info);
	SceUID pPngBufMemHandle;
	unsigned char* pngBuffer = (unsigned char*)cpu_alloc(info.st_size, &pPngBufMemHandle);
	if(pngBuffer < 0)
		PRINT_ERROR("Failed to Allocate pngBuffer");
	
	
	sceIoRead(fd,pngBuffer,info.st_size);
	 vita2d_texture *image;
	 image = vita2d_load_PNG_buffer(pngBuffer);

	memset(&pad, 0, sizeof(pad));

	SceUInt64 curMicros = 0, deltaMicros = 0, lastMicros = 0;
	
	int frames = 0;

	char timeBuffer[256];

	for(;;)
	{
		sceCtrlPeekBufferPositive(0, &pad, 1);

		if (pad.buttons & SCE_CTRL_START)
			break;

		

		vita2d_start_drawing();
		vita2d_clear_screen();

	

		
		
		vita2d_pgf_draw_text(pgf,0,100,RGBA8(255,255,255,255),1.0f,timeBuffer); 

		vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);

		vita2d_end_drawing();
		vita2d_swap_buffers();
		frames++;
		
		curMicros = sceKernelGetProcessTimeWide();

		if(curMicros >= (lastMicros + 1000000))
		{
			deltaMicros = curMicros - lastMicros;
			lastMicros = curMicros;
			float fps = (frames/(double)deltaMicros) * 1000000.0;
			float frameTime = (1.0f / fps) *1000.0f;

			frames = 0;
			int writeSize =	sceClibSnprintf(timeBuffer,256,"Fps: %f\nFrame Time: %f\n",fps,frameTime);	
			timeBuffer[writeSize] = '\0';                                           	
	

		}



		rad += 0.001f;
	}

	/*
	 * vita2d_fini() waits until the GPU has finished rendering,
	 * then we can free the assets freely.
	 */
	vita2d_fini();
	vita2d_free_texture(image);
	vita2d_free_pgf(pgf);
	vita2d_free_pvf(pvf);

	sceKernelFreeMemBlock(pPngBufMemHandle);
	sceIoClose(fd);

	sceKernelExitProcess(0);
	return 0;
}
