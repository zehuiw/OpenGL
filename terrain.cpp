// 用途: 绘制地面
// 作者: 何孟宁

#include <fstream>
#include <cmath>

#include "gl\glut.h"
#include "Bitmap.h"
#include "vector3.h"
#include "Terrain.h"
#include <iostream>

using namespace std;
//------------------------------------------------------------
void CTerrain::Free()
{
	glDeleteLists(m_List, m_Number);
	delete[] m_buffer;
	delete[] m_Block;
}
//------------------------------------------------------------
bool CTerrain::Create(char* filename)
{
	// 从BMP读取,注意宽一定要是4的倍数

	ifstream file(filename, ios::binary | ios::in);
	if (!file)
	{
		cout << "无法打开 " << filename << endl;
		return false;
	}
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
	file.read((char*)&bmfh, sizeof(bmfh));
	file.read((char*)&bmih, sizeof(bmih));
	if (bmih.biBitCount != 8)
	{
		cout << filename << "不是8位灰度图 " << bmih.biBitCount << endl;
		return false;
	}
	file.seekg(bmfh.bfOffBits);
	int size = bmih.biHeight * bmih.biWidth;
	m_buffer = new unsigned char[size];
	file.read((char *)m_buffer, size);
	file.close();

	m_Width = bmih.biWidth;
	cout  <<'\t'<< m_Width << endl;

	m_Height = bmih.biHeight;
	cout << m_Height << endl;

	m_Step = 2.0f;
	m_minZ = -60.0f;
	m_Scale = 0.54f;

	makelist();
	return true;
}
//------------------------------------------------------------
void CTerrain::makelist()
{
	// 顶点
	vector3 *Vertice = new vector3[m_Width * m_Height];
	// 纹理
	Coord *TexCoord = new Coord[m_Width * m_Height];
	// 法向量
	vector3 *Normal = new vector3[m_Width * m_Height];

	for (int i = 0; i < m_Height * m_Width; i++)
	{
		Normal[i].zero();
	}

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			int i = y * m_Width + x;
			// 顶点坐标
			Vertice[i].x = m_Step * x;
			Vertice[i].z = m_Step * y;
			Vertice[i].y = (m_minZ + m_Scale * m_buffer[i]);
			// 纹理坐标
			TexCoord[i].x = 11.0f / m_Width * x;
			TexCoord[i].y = 11.0f / m_Height * y;
		}
	}
	// 计算法向量
	for (int y = 0; y < m_Height - 1; y++)
	{
		for (int x = 0; x < m_Width - 1; x++)
		{
			int i = y * m_Width + x;

			vector3 &v1 = Vertice[i];
			vector3 &v3 = Vertice[i + m_Width];
			vector3 &v2 = Vertice[i + 1];
			vector3 &v4 = Vertice[i + m_Width + 1];
			vector3 &n1 = Normal[i];
			vector3 &n3 = Normal[i + m_Width];
			vector3 &n2 = Normal[i + 1];
			vector3 &n4 = Normal[i + m_Width + 1];

			vector3 nor1 = vector3::GetTriangleNormal(v1, v2, v3);
			vector3 nor2 = vector3::GetTriangleNormal(v3, v2, v4);
			n1 += nor1;
			n2 += nor1;
			n3 += nor1;
			n2 += nor2;
			n3 += nor2;
			n4 += nor2;
		}
	}
	// 计算单位法向量
	for (int i = 0; i < m_Height * m_Width; i++)
	{
		Normal[i].normalize();
	}
	// 
	const int size = 32;
	int number_x = m_Width / size;
	int number_y = m_Height / size;
	m_Number = number_x * number_y;

	m_List = glGenLists(m_Number);
	m_Block = new TerrainBlock[m_Number];

	for (int y = 0; y < number_y; y++)
	{
		// 遍历每一个地形块，并计算大小，创建显示列表
		int min_y = y * size;
		int max_y;
		if (y == number_y - 1)
		{
			max_y = m_Height - 1;
		}
		else
		{
			max_y = min_y + size;
		}
		for (int x = 0; x < number_x; x++)
		{
			int min_x = x * size;
			int max_x;
			if (x == number_x - 1)
			{
				max_x = m_Height - 1;
			}
			else
			{
				max_x = min_x + size;
			}
			// 开始创建一个地形块
			TerrainBlock &Block_now = m_Block[x + y * number_x];
			Block_now.List = m_List + x + y * number_x;
			// 计算坐标最值
			Block_now.Min = Vertice[min_x + min_y * m_Width];
			Block_now.Max = Vertice[max_x + max_y * m_Width];
			for (int y2 = min_y; y2 <= max_y; y2++)
			{
				for (int x2 = min_x; x2 <= max_x; x2++)
				{
					float z = Vertice[x2 + y2 * m_Width].z;
					if (Block_now.Min.z > z) Block_now.Min.z = z;
					if (Block_now.Max.z < z) Block_now.Max.z = z;
				}
			}
			// 创建显示列表
			glNewList(Block_now.List, GL_COMPILE);
			for (int y2 = min_y; y2 < max_y; y2++)
			{
				glBegin(GL_TRIANGLE_STRIP);
				for (int x2 = min_x; x2 <= max_x; x2++)
				{
					//(x,y+1)(x,y)(x+1,y+1)(x+1,y)...
					int p1 = x2 + (y2 + 1) * m_Width;
					glTexCoord2fv(TexCoord[p1].v);
					glNormal3fv(Normal[p1].v);
					glVertex3fv(Vertice[p1].v);
					int p2 = x2 + y2 * m_Width;
					glTexCoord2fv(TexCoord[p2].v);
					glNormal3fv(Normal[p2].v);
					glVertex3fv(Vertice[p2].v);
				}
				glEnd();
			}
			glEndList();
		}
	}

	delete[] Vertice;
	delete[] TexCoord;
	delete[] Normal;
}
//------------------------------------------------------------

void CTerrain::Draw()
{
	// 绘制模型
	
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	for (int i = 0; i<m_Number; i++)
	{

		glCallList(m_Block[i].List);
	}

}
//------------------------------------------------------------
float CTerrain::GetHeight(float x, float y)
{
	// x,y处高度
	int w = (int)(x / m_Step);
	int h = (int)(y / m_Step);
	if (w >= 0 && h >= 0 && w<m_Width - 1 && h<m_Height - 1)
	{
		float h00 = m_minZ + m_Scale * m_buffer[h * m_Width + w];
		float h01 = m_minZ + m_Scale * m_buffer[h * m_Width + w + 1];
		float h11 = m_minZ + m_Scale * m_buffer[(h + 1) * m_Width + w + 1];
		float h10 = m_minZ + m_Scale * m_buffer[(h + 1) * m_Width + w];

		float tx = (x - w * m_Step) / m_Step;
		float ty = (y - h * m_Step) / m_Step;
		float txty = tx * ty;
		// 插值
		float height = h00*(1.0f - ty - tx + txty) + h01*(tx - txty) + h11*txty + h10*(ty - txty);
		if (height < 0.0f)
		{
			height = 0.0f;
		}
		return height;
	}
	else
	{
		return (float)0xffffffff;
	}

}
//------------------------------------------------------------