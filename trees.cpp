// ��;: �����ʾ��ľ
// ����: ������

#include "gl\glut.h"
#include "trees.h"
#include "vector3.h"


//------------------------------------------------------------
void CTree::Init(float height, int number, CTerrain& ground)
{

	num = number;
	h = height;
	tree = new TreeInfo[num];
	for (int i = 0; i<num;)
	{
		// 256.0Ϊ��������
		float x = 256.0f * (float)rand() / (float)RAND_MAX;
		float y = 256.0f * (float)rand() / (float)RAND_MAX;
		float z = ground.GetHeight(x, y);
		if (z != 0.0f)// ���в�����
		{
			tree[i].x = x;
			tree[i].y = z;
			tree[i].z = y;
			i++;
		}
	}
}
//------------------------------------------------------------
void CTree::Free(void)
{
	delete[] tree;
}
//------------------------------------------------------------
void CTree::Draw(void)
{
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	vector3 X(mat[0], mat[4], mat[8]);
	vector3 Z(mat[1], mat[5], mat[9]);

	glEnable(GL_ALPHA_TEST);// ͸����ͼ
	glAlphaFunc(GL_GREATER, 0.6f);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	for (int i = 0; i<num; i++)
	{
		vector3 pos(tree[i].x, tree[i].y+h, tree[i].z ); 
		glTexCoord2f(0.0, 0.0); glVertex3fv((pos + (X + Z)*-h).v);//���µ�
		glTexCoord2f(1.0, 0.0); glVertex3fv((pos + (X - Z)* h).v);//���µ�
		glTexCoord2f(1.0, 1.0); glVertex3fv((pos + (X + Z)* h).v);//���ϵ�
		glTexCoord2f(0.0, 1.0); glVertex3fv((pos + (Z - X)* h).v);//���ϵ�
	}
	glEnd();
	glEnable(GL_LIGHTING);

	glDisable(GL_ALPHA_TEST);
	//glDisable(GL_BLEND);
}
//------------------------------------------------------------