// ��;: ����ģ��

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include "vector3.h"
#include "gl/glut.h"

using namespace std;

class CModel
{
public:
	~CModel();
	void Create(string& mesh, vector3& Pos, float ang);

	void Render(bool move);
	GLuint m_Texture;	// ����

private:
	vector3 m_Pos;			// λ��
	int m_Loaded;
	float angle;

public:
	bool Load(string m_Name);
	bool init;
	void setPosition(vector3 position, float ang);

private:
	float mat[16];
	void Draw(void);
	union Coord
	{
		struct
		{
			float x, y;
		};
		float v[2];
	};
	struct POINT_TNV
	{
		Coord tex;
		vector3 nor;
		vector3 ver;
	};
	unsigned int m_NumPoint;// �������
	unsigned int m_NumFace;	// ����
	POINT_TNV *m_Point;		// ��������, GL_T2F_N3F_V3F��ʽ
	unsigned short *m_Index;// ����
	int m_Count;			// ������

	// ����
};

#endif // MODEL_H