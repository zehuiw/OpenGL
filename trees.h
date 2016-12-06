// 用途: 随机显示树木

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
	GLuint m_Texture;// 纹理

private:
	struct TreeInfo
	{
		float x, y, z;
	};
	TreeInfo *tree;// 每一棵树的信息
	int num;// 树的数量
	float h;// 树的高度

};

#endif // TREE_H