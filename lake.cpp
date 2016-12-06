// 用途: 绘制水面

#include "gl/glut.h"
#include "lake.h"

void CWater::Draw(void)
{

	const GLfloat mat_diffuse2[] = { 1.0f, 1.0f, 1.0f, 0.8f };
	const GLfloat mat_ambient2[] = { 0.4f, 0.4f, 0.4f, 0.8f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);

	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f); // 法线向上
	glTexCoord2f(0.0f + m_offset, 20.0f + m_offset); glVertex3f(256.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f + m_offset, 0.0f + m_offset); glVertex3f(256.0f, 0.0f, 256.0f);
	glTexCoord2f(20.0f + m_offset, 0.0f + m_offset); glVertex3f(0.0f, 0.0f, 256.0f);
	glTexCoord2f(20.0f + m_offset, 20.0f + m_offset); glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glDisable(GL_BLEND);

//	m_offset += 0.002f;
}

void CWater::Init()
{
	m_offset = 0.0f;
}
