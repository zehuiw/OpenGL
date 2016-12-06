// 用途: 绘制地面
// 作者: 何孟宁
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
	void Draw(); // 绘制
	float GetHeight(float x, float y);// 获得(x,y)处高度
	void Free();
	GLuint m_Texture;// 纹理
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
		GLuint List;// 存储显示列表
		vector3 Max, Min;// 尺寸
	};

private:
	GLuint m_List;	// 第一个显示列表
	unsigned char *m_buffer;// 存储高度图
	TerrainBlock* m_Block;// 地形块
public:
	int m_Width;	// 高度图宽度
	int m_Height;	// 高度图高度
	float m_Step;	// XY缩放
	float m_minZ;	// 最低高度
	float m_Scale;	// 高度缩放
	int m_Number;	// 地形块数目
};

#endif // TERRAIN_H