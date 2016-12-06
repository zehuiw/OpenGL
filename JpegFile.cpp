// ��;: 1.����JPEG 2.����JPEG
// ����: ������

#include <stdio.h>
#include "JpegFile.h"
#include <iostream>
using namespace std;
//----------------------------------------------------------------
// ������
void CJpegFile::my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	// ���������Ϣ
	char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message) (cinfo, buffer);
	cout << "libjpeg: " << buffer << endl;
	longjmp(myerr->setjmp_buffer, 1);
}

//----------------------------------------------------------------
// ��ȡjpeg
bool CJpegFile::Load(const char *file, unsigned char **dataBuf,
	unsigned int *width, unsigned int *height, int *components)
{
	// ���ļ�
	FILE *infile = fopen(file, "rb");
	if (!infile)
	{
		cout << "�޷��� " << file << endl;
		return false;
	}
	// ʹ�õĽṹ
	jpeg_decompress_struct cinfo;
	my_error_mgr jerr;
	// ������
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		cout << "�޷���ѹ " << file << endl;
		return false;
	}
	// ��ʼ��
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	// ͼ���С
	(*width) = cinfo.output_width;
	(*height) = cinfo.output_height;
	int row_stride = cinfo.output_width * cinfo.output_components;
	(*components) = cinfo.output_components;
	// �����ڴ�,����ʹ�ú�����ɾ��
	(*dataBuf) = new unsigned char[cinfo.output_height * row_stride];
	// ��ȡ����
	while (cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char* row = (*dataBuf) + row_stride *
			(cinfo.output_height - cinfo.output_scanline - 1);
		jpeg_read_scanlines(&cinfo, &row, 1);
	}
	// ��ѹ����
	jpeg_finish_decompress(&cinfo);
	fclose(infile);
	jpeg_destroy_decompress(&cinfo);
	return true;
}

//----------------------------------------------------------------
// ����Ϊjpeg
bool CJpegFile::Save(const char *file, unsigned char *dataBuf,
	unsigned int Width, unsigned int Height, int quality)
{
	// �����ļ�
	FILE *outfile = fopen(file, "wb");
	if (!outfile)
	{
		cout << "�޷����� " << file << endl;
		return false;
	}
	// ʹ�õĽṹ
	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;
	// ������
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_compress(&cinfo);
		fclose(outfile);
		cout << "�޷�ѹ�� " << file << endl;
		return false;
	}
	// ��ʼ��
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
	// ���ò���
	cinfo.image_width = Width;
	cinfo.image_height = Height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	// ��ʼѹ��
	jpeg_start_compress(&cinfo, TRUE);
	// д������
	int row_stride = cinfo.image_width * 3;
	while (cinfo.next_scanline < cinfo.image_height)
	{
		unsigned char* row = dataBuf + row_stride *
			(cinfo.image_height - cinfo.next_scanline - 1);
		jpeg_write_scanlines(&cinfo, &row, 1);
	}
	// ����ѹ��,dataBuf �������ͷ�
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return true;
}