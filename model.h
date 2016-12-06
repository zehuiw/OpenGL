// 用途: 绘制模型

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
	GLuint m_Texture;	// 纹理

private:
	vector3 m_Pos;			// 位置
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
	unsigned int m_NumPoint;// 顶点个数
	unsigned int m_NumFace;	// 面数
	POINT_TNV *m_Point;		// 顶点数据, GL_T2F_N3F_V3F格式
	unsigned short *m_Index;// 索引
	int m_Count;			// 引用数

	// 名称
};

#endif // MODEL_H