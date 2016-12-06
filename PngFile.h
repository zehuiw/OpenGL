// ”√Õæ: ∂¡–¥PNG
#ifndef PNGFILE_H
#define PNGFILE_H

#include "png.h"

class CPngFile
{
private:
	static void user_error_fn(png_structp png_ptr, png_const_charp error_message);
	static void user_warning_fn(png_structp png_ptr, png_const_charp warning_message);
public:
	static bool Load(const char *file, png_bytepp dataBuf,
		unsigned int *img_width, unsigned int *img_height, int *components);
	static bool Save(const char *file, png_bytep dataBuf,
		unsigned int width, unsigned int height);
};

#endif // PNGFILE_H