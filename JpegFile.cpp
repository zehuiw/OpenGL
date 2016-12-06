// 用途: 1.加载JPEG 2.保存JPEG
// 作者: 何孟宁

#include <stdio.h>
#include "JpegFile.h"
#include <iostream>
using namespace std;
//----------------------------------------------------------------
// 错误处理
void CJpegFile::my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	// 输出错误信息
	char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message) (cinfo, buffer);
	cout << "libjpeg: " << buffer << endl;
	longjmp(myerr->setjmp_buffer, 1);
}

//----------------------------------------------------------------
// 读取jpeg
bool CJpegFile::Load(const char *file, unsigned char **dataBuf,
	unsigned int *width, unsigned int *height, int *components)
{
	// 打开文件
	FILE *infile = fopen(file, "rb");
	if (!infile)
	{
		cout << "无法打开 " << file << endl;
		return false;
	}
	// 使用的结构
	jpeg_decompress_struct cinfo;
	my_error_mgr jerr;
	// 错误处理
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		cout << "无法解压 " << file << endl;
		return false;
	}
	// 初始化
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	// 图像大小
	(*width) = cinfo.output_width;
	(*height) = cinfo.output_height;
	int row_stride = cinfo.output_width * cinfo.output_components;
	(*components) = cinfo.output_components;
	// 分配内存,需在使用后自行删除
	(*dataBuf) = new unsigned char[cinfo.output_height * row_stride];
	// 读取数据
	while (cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char* row = (*dataBuf) + row_stride *
			(cinfo.output_height - cinfo.output_scanline - 1);
		jpeg_read_scanlines(&cinfo, &row, 1);
	}
	// 解压结束
	jpeg_finish_decompress(&cinfo);
	fclose(infile);
	jpeg_destroy_decompress(&cinfo);
	return true;
}

//----------------------------------------------------------------
// 保存为jpeg
bool CJpegFile::Save(const char *file, unsigned char *dataBuf,
	unsigned int Width, unsigned int Height, int quality)
{
	// 创建文件
	FILE *outfile = fopen(file, "wb");
	if (!outfile)
	{
		cout << "无法创建 " << file << endl;
		return false;
	}
	// 使用的结构
	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;
	// 错误处理
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_compress(&cinfo);
		fclose(outfile);
		cout << "无法压缩 " << file << endl;
		return false;
	}
	// 初始化
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
	// 设置参数
	cinfo.image_width = Width;
	cinfo.image_height = Height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	// 开始压缩
	jpeg_start_compress(&cinfo, TRUE);
	// 写入数据
	int row_stride = cinfo.image_width * 3;
	while (cinfo.next_scanline < cinfo.image_height)
	{
		unsigned char* row = dataBuf + row_stride *
			(cinfo.image_height - cinfo.next_scanline - 1);
		jpeg_write_scanlines(&cinfo, &row, 1);
	}
	// 结束压缩,dataBuf 须自行释放
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return true;
}