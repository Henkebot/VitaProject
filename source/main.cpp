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

//#include <gfx.h>

#include "debugScreen.h"
//#include "utility.h"

const char* pfilepath = "app0:/res/sample.png";


#define printf psvDebugScreenPrintf

#define PRINT_ERROR(msg) {psvDebugScreenPrintf("Error at %s: %s\n",AT,msg); for(;;);}



#include "debug.h"
#define RGBA(r,g,b,a)((r << 24)|(g<<16)|(b<<8)|(a))
#define FRAMEBUFFER_SIZE (2*1024*1024)
//const unsigned int SCREEN_HEIGHT = 960;
//const unsigned int SCREEN_WIDTH = 544;
int main()
{
	// Create a debug file that we can output errors to
	InitDebugFile();
	PrintError("This is a fucking test\n");

	// Testing Start
	SceUID displayBlock;
	void* pFramebuffer = NULL;

	displayBlock = sceKernelAllocMemBlock("disp",SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,FRAMEBUFFER_SIZE, NULL);
	if(displayBlock < 0)
		PrintError("Failed to allocate displayblock\n");
	sceKernelGetMemBlockBase(displayBlock,(void**)&pFramebuffer);
	SceDisplayFrameBuf frameDesc;
	frameDesc.base 		= pFramebuffer;
	frameDesc.height 	= SCREEN_HEIGHT;
	frameDesc.width 	= SCREEN_WIDTH;
	frameDesc.pitch 	= SCREEN_WIDTH;
	frameDesc.size 		= sizeof(frameDesc);
	frameDesc.pixelformat	= 0; 	
	
	int error = sceDisplaySetFrameBuf(&frameDesc,SCE_DISPLAY_SETBUF_NEXTFRAME);
	if(error < 0)
		PrintError("Failed To SetDisplayFrameBuf()\n");
	if(NULL == pFramebuffer)
		PrintError("Failed to get a ptr\n");

	for(unsigned int x = 0; x < SCREEN_WIDTH;x++)
	{
		for(unsigned int y = 0; y < SCREEN_HEIGHT;y++)
		{
			unsigned int* pPixel = ((unsigned int*)pFramebuffer) + x + (y * SCREEN_WIDTH);
			unsigned char r = x;
			unsigned char g = y;
			unsigned char b = x + y;

			*pPixel =RGBA(255,0,0,255); 			
		}
	}
	for(;;);
return 0;

//	// Testing End
//	return 0;
//	gfx_init();
//	SceCtrlData pad;
//	vita2d_pgf *pgf;
//	vita2d_pvf *pvf;
//	float rad = 0.0f;
//
//	psvDebugScreenInit();
//
//	vita2d_init();
//	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
//
//	pgf = vita2d_load_default_pgf();
//	pvf = vita2d_load_default_pvf();
//
//	SceUID fd = sceIoOpen(pfilepath, SCE_O_RDONLY, 0777);
//
//	SceIoStat info;
//	sceIoGetstat(pfilepath,&info);
//	SceUID pPngBufMemHandle;
//	unsigned char* pngBuffer = (unsigned char*)cpu_alloc(info.st_size, &pPngBufMemHandle);
//	if(pngBuffer < 0)
//		PRINT_ERROR("Failed to Allocate pngBuffer");
//	
//	
//	sceIoRead(fd,pngBuffer,info.st_size);
//	 vita2d_texture *image;
//	 image = vita2d_load_PNG_buffer(pngBuffer);
//
//	memset(&pad, 0, sizeof(pad));
//
//	SceUInt64 curMicros = 0, deltaMicros = 0, lastMicros = 0;
//	
//	int frames = 0;
//
//	char timeBuffer[256];
//
//	for(;;)
//	{
//		sceCtrlPeekBufferPositive(0, &pad, 1);
//
//		if (pad.buttons & SCE_CTRL_START)
//			break;
//
//		
//
//		vita2d_start_drawing();
//		vita2d_clear_screen();
//
//	
//
//		
//		
//		vita2d_pgf_draw_text(pgf,0,100,RGBA8(255,255,255,255),1.0f,timeBuffer); 
//
//		vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
//
//		vita2d_end_drawing();
//		vita2d_swap_buffers();
//		frames++;
//		
//		curMicros = sceKernelGetProcessTimeWide();
//
//		if(curMicros >= (lastMicros + 1000000))
//		{
//			deltaMicros = curMicros - lastMicros;
//			lastMicros = curMicros;
//			float fps = (frames/(double)deltaMicros) * 1000000.0;
//			float frameTime = (1.0f / fps) *1000.0f;
//
//			frames = 0;
//			int writeSize =	sceClibSnprintf(timeBuffer,256,"Fps: %f\nFrame Time: %f\n",fps,frameTime);	
//			timeBuffer[writeSize] = '\0';                                           	
//	
//
//		}
//
//
//
//		rad += 0.001f;
//	}
//
//	/*
//	 * vita2d_fini() waits until the GPU has finished rendering,
//	 * then we can free the assets freely.
//	 */
//	vita2d_fini();
//	vita2d_free_texture(image);
//	vita2d_free_pgf(pgf);
//	vita2d_free_pvf(pvf);
//
//	sceKernelFreeMemBlock(pPngBufMemHandle);
//	sceIoClose(fd);
//
//	sceKernelExitProcess(0);
//	return 0;
}
