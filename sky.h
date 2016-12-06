#ifndef _SKY_H
#define _SKY_H

#include <gl/glut.h>

class Sky
{
public:
	Sky();

	//��ʼ����յ�λ�ã��뾶������
	void InitSky(float skyX, float skyY, float skyZ, float skyR, unsigned int texture);

	//��ʾ���
	void ShowSky(void);

	//�����յ�����
	float GetSkyX(void);
	float GetSkyY(void);
	float GetSkyZ(void);
	float GetSkyR(void);
	unsigned int GetSkyTexture(void);

public:
	float X;
	float Y;
	float Z;
	float R;
	unsigned int T;
	GLUquadricObj *qobj;
};

#endif