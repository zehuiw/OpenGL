#include "model.h"
#include "gl/glut.h"
#include <string>
#include "vector3.h"
#include <fstream>
#include <iostream>

void CModel::Create( string& mesh, vector3& Pos, float ang)
{

	m_Pos = Pos;
	m_Loaded = Load(mesh);
	angle = ang;
	init = false;
}

void CModel::setPosition(vector3 position,float ang)
{
	m_Pos = position;
	angle = ang;
}

CModel::~CModel(void)
{
	// �ͷ��ڴ�
	if (m_Loaded)
	{
		delete[] m_Point;
		delete[] m_Index;
	}
}

bool CModel::Load(string m_Name)
{
	ifstream file(("img\\" + m_Name).c_str(), ios::binary | ios::in);
	if (!file)
	{
		cout << "�޷��� " << m_Name << endl;
		return false;
	}

	file.read((char*)&m_NumPoint, 4);	// ��ȡ�������m_NumPoint
	file.read((char*)&m_NumFace, 4);	// ��ȡ����m_NumFace

	// �����ڴ�
	m_Point = new POINT_TNV[m_NumPoint];
	m_Index = new unsigned short[m_NumFace * 3];

	// ��ȡ����
	for (unsigned int i = 0; i<m_NumPoint; i++)
	{
		file.read((char*)m_Point[i].tex.v, sizeof(Coord));

		m_Point[i].nor.zero();
		file.read((char*)m_Point[i].ver.v, sizeof(vector3));
		m_Point[i].ver.exchange();
	}
	file.read((char*)m_Index, m_NumFace*sizeof(unsigned short) * 3);
	file.close();

	// ��������������棬���㷨����
	for (unsigned int i = 0; i<m_NumFace; i++)
	{
		unsigned int p1, p2, p3;
		p1 = m_Index[i * 3];
		p2 = m_Index[i * 3 + 1];
		p3 = m_Index[i * 3 + 2];

		vector3 nor = vector3::GetTriangleNormal(m_Point[p1].ver, m_Point[p2].ver, m_Point[p3].ver);

		m_Point[p1].nor += nor;
		m_Point[p2].nor += nor;
		m_Point[p3].nor += nor;
	}

	// ������������ж��㣬��λ������
	for (unsigned int i = 0; i<m_NumPoint; i++)
	{
		m_Point[i].nor.normalize();
	}

	return 1;
}


void CModel::Render(bool move)
{
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glPushMatrix();	
	if (!init)
	{
		init = true;
		glTranslatef(m_Pos.x, m_Pos.y, m_Pos.z);
		Draw();
		glGetFloatv(GL_PROJECTION_MATRIX, mat);
	}
	else{
		glLoadMatrixf(mat);
		if (move){
			glTranslatef(m_Pos.x, m_Pos.y, m_Pos.z);
			glRotatef(angle, 0., 1., 0.);
			glGetFloatv(GL_PROJECTION_MATRIX, mat);
		}
		Draw();
	}

	glPopMatrix();	
}

void CModel::Draw(void)
{
	glInterleavedArrays(GL_T2F_N3F_V3F, 0, m_Point);
	glDrawElements(GL_TRIANGLES, m_NumFace * 3, GL_UNSIGNED_SHORT, m_Index);
}