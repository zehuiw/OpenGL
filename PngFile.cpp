// 用途: 读写PNG
// 作者: 何孟宁
#include "PngFile.h"
#include <stdio.h>
#include <iostream>
using namespace std;
void CPngFile::user_error_fn(png_structp png_ptr, png_const_charp error_message)
{
	cout << "libpng error: " << error_message << endl;
	if (png_ptr)
	{
		longjmp(png_ptr->jmpbuf, 1);
	}
}

void CPngFile::user_warning_fn(png_structp png_ptr, png_const_charp warning_message)
{
	cout << "libpng warning: " << warning_message << endl;
}

bool CPngFile::Load(const char *file, png_bytepp dataBuf, unsigned int *img_width,
	unsigned int *img_height, int *components)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type;
	// 打开文件	
	if ((fp = fopen(file, "rb")) == NULL)
	{
		cout << "无法打开 " << file << endl;
		return false;
	}
	// 创建并初始化read_struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, user_error_fn, user_warning_fn);

	if (png_ptr == NULL)
	{
		fclose(fp);
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return false;
	}
	// 错误处理
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(fp);
		cout << "无法读取 " << file << endl;
		return false;
	}
	// 开始读
	png_init_io(png_ptr, fp);

	png_set_sig_bytes(png_ptr, sig_read);

	png_read_info(png_ptr, info_ptr);
	// 文件信息
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		int_p_NULL, int_p_NULL, int_p_NULL);
	(*img_width) = width;
	(*img_height) = height;
	if (color_type == PNG_COLOR_TYPE_GRAY)
	{
		(*components) = 1;
	}
	else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		(*components) = 2;
	}
	else if (color_type == PNG_COLOR_TYPE_RGB)
	{
		(*components) = 3;
	}
	else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		(*components) = 4;
	}
	// 转换格式
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
		(*components) = 3;
	}
	else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
		(*components) = 4;
	}
	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);
	}
	else if (bit_depth < 8)
	{
		png_set_packing(png_ptr);
	}
	// 分配内存,需在使用后自行删除
	png_uint_32 rowbytes = width * (*components);
	(*dataBuf) = new png_byte[rowbytes * height];
	// 读取数据
	for (png_uint_32 y = 0; y < height; y++)
	{
		png_bytep row = (*dataBuf) + rowbytes * (height - y - 1);
		png_read_rows(png_ptr, &row, png_bytepp_NULL, 1);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);

	return true;
}

bool CPngFile::Save(const char *file, png_bytep dataBuf, unsigned int width, unsigned int height)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;

	/* open the file */
	fp = fopen(file, "wb");
	if (fp == NULL)
	{
		cout << "无法打开 " << file << endl;
		return false;
	}

	// 创建并初始化write_struct
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
		NULL, user_error_fn, user_warning_fn);
	if (png_ptr == NULL)
	{
		fclose(fp);
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, png_infopp_NULL);
		return false;
	}

	// 错误处理
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		cout << "无法读取 " << file << endl;
		return false;
	}

	// 开始写
	png_init_io(png_ptr, fp);

	// 文件信息
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	// 写入数据
	for (png_uint_32 y = 0; y < height; y++)
	{
		png_bytep row = dataBuf + 3 * width * (height - y - 1);
		png_write_rows(png_ptr, &row, 1);
	}

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);

	return true;
}