// ��;: ���Ƶ���
// ����: ������
#ifndef TERRAIN_H
#define TERRAIN_H

#include "gl\glut.h"
#include "vector3.h"
#include <iostream>
#include <string>

using namespace std;

class CTerrain
{
public:
	bool Create(char* filename);
	void Draw(); // ����
	float GetHeight(float x, float y);// ���(x,y)���߶�
	void Free();
	GLuint m_Texture;// ����
private:
	void makelist();


private:
	union Coord
	{
		struct
		{
			float x, y;
		};
		float v[2];
	};
	struct TerrainBlock
	{
		GLuint List;// �洢��ʾ�б�
		vector3 Max, Min;// �ߴ�
	};

private:
	GLuint m_List;	// ��һ����ʾ�б�
	unsigned char *m_buffer;// �洢�߶�ͼ
	TerrainBlock* m_Block;// ���ο�
public:
	int m_Width;	// �߶�ͼ���
	int m_Height;	// �߶�ͼ�߶�
	float m_Step;	// XY����
	float m_minZ;	// ��͸߶�
	float m_Scale;	// �߶�����
	int m_Number;	// ���ο���Ŀ
};

#endif // TERRAIN_H