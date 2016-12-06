#include "PngFile.h"
#include "JpegFile.h"
#include "sky.h"
#include "terrain.h"
#include <GL/glut.h>
#include <iostream>  
#include <fstream>  
#include "lake.h"
#include "trees.h"
#include "stdlib.h"
#include "stdafx.h"
#include "model.h"
#include <sstream>

using namespace std;

float angle = 120.;
float z = 70.,y = 15.;
float x = 70.;

float loader_x = 159, loader_z = 104, loader_angle = 0;


GLuint g_sky;

CTerrain ground;
Sky sky1;
CWater lake;
CTree trees;
CModel loader;

//CLoad3DS g_Load3ds;
//t3DModel g_3DModel;

void timer(int id);
void reshape(int w, int h);
void disp();
void init();
void LoadFromFile(string m_Name, GLuint &m_Texture);
void Create(GLint components, GLint width, GLint height, const void *data, GLuint &m_Texture);

void init()
{
	float radius = 300.;
	LoadFromFile("sky5.jpg", g_sky);
	LoadFromFile("grass2.jpg", ground.m_Texture);
	LoadFromFile("water1.jpg", lake.m_Texture);
	LoadFromFile("tree.png", trees.m_Texture);
	LoadFromFile("loader.png", loader.m_Texture);
//	cout << "loader:::"<< loader.m_Texture;

	ground.Create("img\\ground.bmp");

	glEnable(GL_TEXTURE_2D);							// ��������ӳ��
	glShadeModel(GL_SMOOTH);							// ������Ӱƽ��
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// ����ɫ
	glClearDepth(1.0f);									// ������Ȼ���
	glEnable(GL_DEPTH_TEST);							// ������Ȳ���
	glDepthFunc(GL_LEQUAL);								// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ������ϸ��͸������

	const GLfloat LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };// ���������
	const GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };// ��������
	const GLfloat Lightspecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// ���淴���
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);// ���û�����
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);// ���������
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lightspecular);// ���þ��淴���
	glEnable(GL_LIGHT0);// ����Դ����Ӧ��
	glEnable(GL_LIGHTING);

	sky1.InitSky(128, 0, 128, 130, g_sky);
	lake.Init();
	trees.Init(5.0f, 50, ground);
	loader.Create(string("loader.3D"), vector3(loader_x, ground.GetHeight(loader_x,loader_z), loader_z),0.);
}

void timer(int id)
{
	lake.m_offset += 0.01;
	glutPostRedisplay();
	glutTimerFunc(40, timer, 1);
}


void Key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		ground.Free();
		trees.Free();
		
		break;
	default:
		break;
	}
}

void SpecialKey(int key, int xx, int yy)
{
	float _x = x, _z = z;

	float angle_C = (angle - 90.) * 3.14 / 180.;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle -= 1.0;
		break;
	case GLUT_KEY_RIGHT:
		angle += 1.0;
		break;
	case GLUT_KEY_UP:
		_x += cos(angle_C);
		_z += sin(angle_C);
		if ((_x - 128)*(_x - 128) + (_z - 128)*(_z - 128) < 15500){
			x = _x;
			z = _z;
		}
		break;
	case GLUT_KEY_DOWN:
		_x -= cos(angle_C);
		_z -= sin(angle_C);

		if ((_x - 128)*(_x - 128) + (_z - 128)*(_z - 128) < 15500){
			x = _x;
			z = _z;
		}
		break;

	case GLUT_KEY_F1:
		loader_z += sin((loader_angle)*3.14/180);
		loader_x += cos((loader_angle)*3.14/180);
		cout << "loader_x:" << loader_x << '\t' << "loader_y:" << loader_z << endl;
		break;

	case GLUT_KEY_F2:
		loader_z -= sin((loader_angle)*3.14/180);
		loader_x -= cos((loader_angle)*3.14/180);
		cout << "loader_x:" << loader_x << '\t' << "loader_y:" << loader_z << endl;

		break;

	case GLUT_KEY_F3:
		loader_angle += 1.0;
		cout << "angle:" << loader_angle << '\t' << sin((loader_angle)*3.14 / 180) << cos((loader_angle)*3.14 / 180) << endl;
		break;

	case GLUT_KEY_F4:
		loader_angle -= 1.0;
		cout << "angle:" << loader_angle << '\t' << sin((loader_angle)*3.14 / 180) << cos((loader_angle)*3.14 / 180) <<endl;
		break;

	}

	
	float height;
	height = ground.GetHeight(x, z);
	y += (height+5 - y) / 5.0f;
//	cout << 'x' << x << '\t' << 'z' << z << endl;

	loader.setPosition(vector3(loader_x, ground.GetHeight(loader_x,loader_z), loader_z),loader_angle);

	disp();
}

void LoadFromFile(string m_Name, GLuint &m_Texture)
{
	unsigned int width, height;
	int components;
	unsigned char *dataBuf = NULL;
	bool success = false;
	// ������չ���ж�����
	if (m_Name.rfind(".jpg") != string::npos)
	{
		success = CJpegFile::Load(("img\\" + m_Name).c_str(), &dataBuf, &width, &height, &components);
		cout << success;
	}
	else if (m_Name.rfind(".png") != string::npos)
	{
		success = CPngFile::Load(("img\\" + m_Name).c_str(), &dataBuf, &width, &height, &components);
	}
	// �����ȡ�ɹ�
	if (success)
	{
		Create(components, width, height, dataBuf, m_Texture);
	}
	delete[] dataBuf;
	// �����Ƿ�ɹ��������Ϊ�Ѽ���
}

void Create(GLint components, GLint width, GLint height, const void *data, GLuint &m_Texture)
{
	GLenum format;
	switch (components)
	{
	case 1:
		format = GL_LUMINANCE;
		break;
	case 2:
		format = GL_LUMINANCE_ALPHA;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, width, height, format, GL_UNSIGNED_BYTE, data);
}



void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float R = (float)w / h;
	gluPerspective(45.0f,R, 0.2f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void disp()
{
//	float height;
	float angle_C = (angle + 90) * 3.14 / 180.;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			 // ����ˢ�±���ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();
	gluLookAt(x, y, z,
		x - 10.*cos(angle_C), y, z - 10.*sin(angle_C),
		0, 10, 0
		);

	const GLfloat light_position[] = { -1.0f, 4.0f, 1.0f, 0.0f };// ��Դλ��
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);// ���ù�Դλ��
	const GLfloat mat_diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat mat_ambient1[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//	const GLfloat mat_ambient2[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1); // ����
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);

	sky1.ShowSky();
	ground.Draw();

	lake.Draw();
	trees.Draw();
	loader.Render();

	glutSwapBuffers();
}

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 700);
	glutCreateWindow("final");
	init();
	glutReshapeFunc(reshape);

	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);

	glutDisplayFunc(disp);
	glutTimerFunc(40, timer, 1);
	glutMainLoop();

	return 0;
}

