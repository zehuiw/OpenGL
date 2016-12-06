// ”√Õæ: 1.º”‘ÿJPEG 2.±£¥ÊJPEG

#ifndef JPEGFILE_H
#define JPEGFILE_H

#ifdef _WINDOWS_
#define XMD_H
#undef FAR
#endif //_WINDOWS_

extern "C"
{
#include <setjmp.h>
#include "jpeglib.h"
}

class CJpegFile
{
public:
	static bool Load(const char *file, unsigned char **dataBuf,
		unsigned int *width, unsigned int *height, int *components);
	static bool Save(const char *file, unsigned char *dataBuf,
		unsigned int Width, unsigned int Height, int quality);
private:
	typedef struct my_error_mgr
	{
		jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	} *my_error_ptr;

	static void my_error_exit(j_common_ptr cinfo);
};

#endif // JPEGFILE_H