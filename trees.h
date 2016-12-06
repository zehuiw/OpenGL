// ��;: �����ʾ��ľ

#ifndef TREE_H
#define TREE_H

#include "gl\glut.h"
#include "terrain.h"

class CTree
{
public:
	void Init(float height, int number, CTerrain& ground);
	void Free(void);
	void Draw(void);
	GLuint m_Texture;// ����

private:
	struct TreeInfo
	{
		float x, y, z;
	};
	TreeInfo *tree;// ÿһ��������Ϣ
	int num;// ��������
	float h;// ���ĸ߶�

};

#endif // TREE_H