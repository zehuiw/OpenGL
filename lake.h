// ��;: ����ˮ��

#ifndef WATER_H
#define WATER_H

#include "gl\glut.h"

class CWater
{
public:
	void Draw(void);
	void Init();
	GLuint m_Texture;// ����
	float m_offset;		// ƫ��

};

#endif // WATER_H