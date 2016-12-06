// 用途: 绘制水面

#ifndef WATER_H
#define WATER_H

#include "gl\glut.h"

class CWater
{
public:
	void Draw(void);
	void Init();
	GLuint m_Texture;// 纹理
	float m_offset;		// 偏移

};

#endif // WATER_H