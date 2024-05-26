#include "Render.h"
#include <sstream>
#include <iostream>
#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include "MyOGL.h"
#include "Camera.h"
#include "Light.h"
#include "Primitives.h"
#include "GUItextRectangle.h"

bool textureMode = true;
bool lightMode = true;
bool textureReplace = true;		
bool Circle1 = false;	
bool Kvadrat = false;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
	if (key == 'E')											
	{
		textureReplace = !textureReplace;
	}
	if (key == 'C')											
	{
		Circle1 = !Circle1;
	}
	if (key == 'Q')
	{
		Kvadrat= !Kvadrat;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}

GLuint texId;
GLuint texId3;
GLuint texId4;
GLuint texId2;									

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	{//������ ����������� ���������  (R G B)																						
		RGBTRIPLE* texarray;

		//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
		char* texCharArray;
		int texW, texH;
		OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);					
		OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);


		//���������� �� ��� ��������
		glGenTextures(1, &texId);
		//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
		glBindTexture(GL_TEXTURE_2D, texId);

		//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);


		//�������� ������
		free(texCharArray);
		free(texarray);

		//������� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);																
	}

	{//������ ����������� ���������  (R G B)
	RGBTRIPLE* texarray2;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char* texCharArray2;
	int texW2, texH2;
	OpenGL::LoadBMP("texture1.bmp", &texW2, &texH2, &texarray2);
	OpenGL::RGBtoChar(texarray2, texW2, texH2, &texCharArray2);


	//���������� �� ��� ��������
	glGenTextures(1, &texId2);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId2);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW2, texH2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray2);


	//�������� ������
	free(texCharArray2);
	free(texarray2);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	{RGBTRIPLE* texarray3;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char* texCharArray3;
	int texW3, texH3;
	OpenGL::LoadBMP("texture2.bmp", &texW3, &texH3, &texarray3);
	OpenGL::RGBtoChar(texarray3, texW3, texH3, &texCharArray3);


	//���������� �� ��� ��������
	glGenTextures(1, &texId3);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId3);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW3, texH3, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray3);


	//�������� ������
	free(texCharArray3);
	free(texarray3);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	{RGBTRIPLE* texarray4;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char* texCharArray4;
	int texW4, texH4;
	OpenGL::LoadBMP("texture3.bmp", &texW4, &texH4, &texarray4);
	OpenGL::RGBtoChar(texarray4, texW4, texH4, &texCharArray4);


	//���������� �� ��� ��������
	glGenTextures(1, &texId4);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId4);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW4, texH4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray4);


	//�������� ������
	free(texCharArray4);
	free(texarray4);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}





void Render(OpenGL* ogl)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	/*��������������*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glDisable(GL_BLEND);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	//������ �����
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���						


	//������ ��������� ���������� ��������
	double A[] = { 1,3,0 };
	double B[] = { 0,9,0 };
	double C[] = { 7,8,0 };
	double D[] = { 11,14,0 };
	double E[] = { 10,8,0 };
	double F[] = { 14,5,0 };
	double G[] = { 11,0,0 };
	double H[] = { 6,5,0 };

	double A1[] = { 1,3,5 };
	double B1[] = { 0,9,5 };
	double C1[] = { 7,8,5 };
	double D1[] = { 11,14,5 };
	double E1[] = { 10,8,5 };
	double F1[] = { 14,5,5 };
	double G1[] = { 11,0,5 };
	double H1[] = { 6,5,5 };

	if (!Kvadrat)
	{

		if (!Circle1)
		{
			if (textureReplace == false)
			{
				{
					double x0 = 0;
					double y0 = 9;

					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					for (double i = -1.25; i <= -0.55; i += 0.0001)
					{
						double x = 1.5 + 6.08276253 / 2 * cos(i * 3.141593);
						double y = 6 + 6.08276253 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3dv(A1);
						glVertex3d(x0, y0, 5);
						glVertex3d(x, y, 5);
						glEnd();

						x0 = x;
						y0 = y;
					}

					x0 = 14;
					y0 = 5;

					for (double i = 0.406; i <= 0.85; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(10.69, 7.483, 5);
						glVertex3d(x0, y0, 5);
						glVertex3d(x, y, 5);
						glEnd();

						x0 = x;
						y0 = y;
					}
					for (double i = 0.8; i <= 1.25; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(8, 3, 5);
						glVertex3d(x0, y0, 5);
						glVertex3d(x, y, 5);
						glEnd();

						x0 = x;
						y0 = y;
					}
					glDisable(GL_BLEND);
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(14.0 / 20.0, 14.0 / 20.0);
						glVertex3d(8, 3, 5);
						glTexCoord2d(49.0 / 80.0, 16.0 / 20.0);
						glVertex3d(10.69, 7.483, 5);
						glTexCoord2d(51.0 / 80.0, 73.0 / 100.0);
						glVertex3d(13.125 + 6.0104076 / 2 * cos(0.8 * 3.141593), 2.125 + 6.0104076 / 2 * sin(0.8 * 3.141593), 5);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(75.0 / 80.0, 86.0 / 100.0);
						glVertex3dv(B1);
						glTexCoord2d(58.0 / 80.0, 83.0 / 100.0);
						glVertex3dv(C1);
						glTexCoord2d(15.0 / 20.0, 15.0 / 20.0);
						glVertex3dv(H1);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(58.0 / 80.0, 83.0 / 100.0);
						glVertex3dv(C1);
						glTexCoord2d(12.0 / 20.0, 1);
						glVertex3dv(D1);
						glTexCoord2d(49.0 / 80.0, 83.0 / 100.0);
						glVertex3dv(E1);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(15.0 / 20.0, 15.0 / 20.0);
						glVertex3dv(H1);
						glTexCoord2d(49.0 / 80.0, 83.0 / 100.0);
						glVertex3dv(E1);
						glTexCoord2d(49.0 / 80.0, 16.0 / 20.0);
						glVertex3d(10.69, 7.483, 5);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(58.0 / 80.0, 83.0 / 100.0);
						glVertex3dv(C1);
						glTexCoord2d(51.0 / 80.0, 83.0 / 100.0);
						glVertex3dv(E1);
						glTexCoord2d(15.0 / 20.0, 15.0 / 20.0);
						glVertex3dv(H1);

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(49.0 / 80.0, 16.0 / 20.0);
						glVertex3d(10.69, 7.483, 5);
						glTexCoord2d(12.0 / 20.0, 14.0 / 20.0);
						glVertex3d(8, 3, 5);
						glTexCoord2d(15.0 / 20.0, 15.0 / 20.0);
						glVertex3dv(H1);

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(73.0 / 80.0, 69.0 / 100.0);
						glVertex3dv(A1);
						glTexCoord2d(75.0 / 80.0, 86.0 / 100.0);
						glVertex3dv(B1);
						glTexCoord2d(15.0 / 20.0, 15.0 / 20.0);
						glVertex3dv(H1);
						glEnd();

						glDisable(GL_BLEND);
					}
					if (textureReplace)
						glBindTexture(GL_TEXTURE_2D, texId);
					else
						glBindTexture(GL_TEXTURE_2D, texId2);
					x0 = 0;
					y0 = 9;

					for (double i = -1.25; i <= -0.55; i += 0.0001)
					{
						double x = 1.5 + 6.08276253 / 2 * cos(i * 3.141593);
						double y = 6 + 6.08276253 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3dv(A);
						glVertex3d(x0, y0, 0);
						glVertex3d(x, y, 0);
						glEnd();

						x0 = x;
						y0 = y;
					}

					x0 = 14;
					y0 = 5;

					for (double i = 0.406; i <= 0.85; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(10.69, 7.483, 0);
						glVertex3d(x0, y0, 0);
						glVertex3d(x, y, 0);
						glEnd();

						x0 = x;
						y0 = y;
					}

					for (double i = 0.8; i <= 1.25; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(8, 3, 0);
						glVertex3d(x0, y0, 0);
						glVertex3d(x, y, 0);
						glEnd();

						x0 = x;
						y0 = y;
					}

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(6.0 / 20.0, 14.0 / 20.0);
					glVertex3d(8, 3, 0);
					glTexCoord2d(31.0 / 80.0, 16.0 / 20.0);
					glVertex3d(10.69, 7.483, 0);
					glTexCoord2d(29.0 / 80.0, 73.0 / 100.0);
					glVertex3d(13.125 + 6.0104076 / 2 * cos(0.8 * 3.141593), 2.125 + 6.0104076 / 2 * sin(0.8 * 3.141593), 0);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(5.0 / 80.0, 86.0 / 100.0);
					glVertex3dv(B);
					glTexCoord2d(22.0 / 80.0, 83.0 / 100.0);
					glVertex3dv(C);
					glTexCoord2d(5.0 / 20.0, 15.0 / 20.0);
					glVertex3dv(H);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(22.0 / 80.0, 83.0 / 100.0);
					glVertex3dv(C);
					glTexCoord2d(8.0 / 20.0, 1);
					glVertex3dv(D);
					glTexCoord2d(31.0 / 80.0, 83.0 / 100.0);
					glVertex3dv(E);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(5.0 / 20.0, 15.0 / 20.0);
					glVertex3dv(H);
					glTexCoord2d(31.0 / 80.0, 83.0 / 100.0);
					glVertex3dv(E);
					glTexCoord2d(31.0 / 80.0, 16.0 / 20.0);
					glVertex3d(10.69, 7.483, 0);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(22.0 / 80.0, 83.0 / 100.0);
					glVertex3dv(C);
					glTexCoord2d(29.0 / 80.0, 83.0 / 100.0);
					glVertex3dv(E);
					glTexCoord2d(5.0 / 20.0, 15.0 / 20.0);
					glVertex3dv(H);

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(31.0 / 80.0, 16.0 / 20.0);
					glVertex3d(10.69, 7.483, 0);
					glTexCoord2d(6.0 / 20.0, 14.0 / 20.0);
					glVertex3d(8, 3, 0);
					glTexCoord2d(5.0 / 20.0, 15.0 / 20.0);
					glVertex3dv(H);

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(7.0 / 80.0, 69.0 / 100.0);
					glVertex3dv(A);
					glTexCoord2d(5.0 / 80.0, 86.0 / 100.0);
					glVertex3dv(B);
					glTexCoord2d(5.0 / 20.0, 15.0 / 20.0);
					glVertex3dv(H);
					glEnd();
				}

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(0.1428, 1, 0);
				glTexCoord2d(0, 1.0 / 20.0);
				glVertex3dv(B);
				glTexCoord2d(0, 5.0 / 20.0);
				glVertex3dv(B1);
				glTexCoord2d(4.0 / 20.0, 5.0 / 20.0);
				glVertex3dv(C1);
				glTexCoord2d(4.0 / 20.0, 1.0 / 20.0);
				glVertex3dv(C);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(-0.83, 0.6, 0);
				glTexCoord2d(5.0 / 20.0, 1.0 / 20.0);
				glVertex3dv(C);
				glTexCoord2d(5.0 / 20.0, 5.0 / 20.0);
				glVertex3dv(C1);
				glTexCoord2d(9.0 / 20.0, 5.0 / 20.0);
				glVertex3dv(D1);
				glTexCoord2d(9.0 / 20.0, 1.0 / 20.0);
				glVertex3dv(D);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(1, -0.17, 0);
				glTexCoord2d(10.0 / 20.0, 1.0 / 20.0);
				glVertex3dv(D);
				glTexCoord2d(10.0 / 20.0, 5.0 / 20.0);
				glVertex3dv(D1);
				glTexCoord2d(14.0 / 20.0, 5.0 / 20.0);
				glVertex3dv(E1);
				glTexCoord2d(14.0 / 20.0, 1.0 / 20.0);
				glVertex3dv(E);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(0.6, 0.8, 0);
				glTexCoord2d(0, 6.0 / 20.0);
				glVertex3dv(E);
				glTexCoord2d(0, 10.0 / 20.0);
				glVertex3dv(E1);
				glTexCoord2d(4.0 / 20.0, 10.0 / 20.0);
				glVertex3dv(F1);
				glTexCoord2d(4.0 / 20.0, 6.0 / 20.0);
				glVertex3dv(F);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(-0.71, -0.71, 0);
				glTexCoord2d(5.0 / 20.0, 6.0 / 20.0);
				glVertex3dv(G);
				glTexCoord2d(5.0 / 20.0, 10.0 / 20.0);
				glVertex3dv(G1);
				glTexCoord2d(9.0 / 20.0, 10.0 / 20.0);
				glVertex3dv(H1);
				glTexCoord2d(9.0 / 20.0, 6.0 / 20.0);
				glVertex3dv(H);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(0.37, -0.93, 0);
				glTexCoord2d(10.0 / 20.0, 6.0 / 20.0);
				glVertex3dv(A);
				glTexCoord2d(10.0 / 20.0, 10.0 / 20.0);
				glVertex3dv(A1);
				glTexCoord2d(14.0 / 20.0, 10.0 / 20.0);
				glVertex3dv(H1);
				glTexCoord2d(14.0 / 20.0, 6.0 / 20.0);
				glVertex3dv(H);
				glEnd();

				double x0 = 0;
				double y0 = 9;

				double tx0 = 0;
				double ty0 = 0;

				double Det[] = { 0,0,0 };
				double Det1[] = { 0,0,5 };
				double N[] = { 0,0,0 };
				double Nl;

				for (double i = -1.25; i <= -0.55; i += 0.0001)
				{
					double x = 1.5 + 6.08276253 / 2 * cos(i * 3.141593);
					double y = 6 + 6.08276253 / 2 * sin(i * 3.141593);

					double tx = tx0 + 1.0 / (30000.0 / 3.0);
					double ty = ty0 + 1.0 / (30000.0 / 3.0);

					glBegin(GL_QUADS);
					glColor3d(0.3f, 0.3f, 0.3f);
					Det[0] = { x - (1.5 + 6.08276253 / 2 * cos((i - 0.0001) * 3.141593)) };
					Det[1] = { y - (6 + 6.08276253 / 2 * sin((i - 0.0001) * 3.141593)) };
					N[0] = { Det[1] * Det1[2] - Det1[1] * Det[2] };
					N[1] = { -Det[0] * Det1[2] + Det1[0] * Det[2] };
					N[2] = { Det[0] * Det1[1] - Det1[0] * Det[1] };
					Nl = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
					N[0] = { N[0] / Nl };
					N[1] = { N[1] / Nl };
					N[2] = { N[2] / Nl };
					glNormal3d(N[0], N[1], N[2]);

					glTexCoord2d(15.0 / 20.0, tx0);
					glVertex3d(x0, y0, 0);
					glTexCoord2d(15.0 / 20.0, tx);
					glVertex3d(x, y, 0);
					glTexCoord2d(19.0 / 20.0, ty);
					glVertex3d(x, y, 5);
					glTexCoord2d(19.0 / 20.0, ty0);
					glVertex3d(x0, y0, 5);
					glEnd();

					x0 = x;
					y0 = y;
					tx0 = tx;
					ty0 = ty;
				}

				x0 = 14;
				y0 = 5;

				tx0 = 7.0 / 20.0;
				ty0 = 7.0 / 20.0;

				for (double i = 0.406; i <= 1.25; i += 0.0001)
				{
					double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
					double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

					double tx = tx0 + 0.5 / (168800.0 / 13.0);
					double ty = ty0 + 0.5 / (168800.0 / 13.0);

					glBegin(GL_POLYGON);
					glColor3d(0.3f, 0.3f, 0.3f);
					Det[0] = { (13.125 + 6.0104076 / 2 * cos((i + 0.0001) * 3.141593)) - x };
					Det[1] = { (13.125 + 6.0104076 / 2 * sin((i + 0.0001) * 3.141593)) - y };
					N[0] = { Det[1] * Det1[2] - Det1[1] * Det[2] };
					N[1] = { -Det[0] * Det1[2] + Det1[0] * Det[2] };
					N[2] = { Det[0] * Det1[1] - Det1[0] * Det[1] };
					Nl = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
					N[0] = { N[0] / Nl };
					N[1] = { N[1] / Nl };
					N[2] = { N[2] / Nl };
					glNormal3d(N[0], N[1], N[2]);

					glTexCoord2d(15.0 / 20.0, tx0);
					glVertex3d(x0, y0, 0);
					glTexCoord2d(15.0 / 20.0, tx);
					glVertex3d(x, y, 0);
					glTexCoord2d(19.0 / 20.0, ty);
					glVertex3d(x, y, 5);
					glTexCoord2d(19.0 / 20.0, ty0);
					glVertex3d(x0, y0, 5);
					glEnd();

					x0 = x;
					y0 = y;
					tx0 = tx;
					ty0 = ty;
				}
			}
			else
			{
				

				{
					double x0 = 0;
					double y0 = 9;

					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					for (double i = -1.25; i <= -0.55; i += 0.0001)
					{
						double x = 1.5 + 6.08276253 / 2 * cos(i * 3.141593);
						double y = 6 + 6.08276253 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3dv(A1);
						glVertex3d(x0, y0, 5);
						glVertex3d(x, y, 5);
						glEnd();

						x0 = x;
						y0 = y;
					}

					x0 = 14;
					y0 = 5;

					for (double i = 0.406; i <= 0.85; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(10.69, 7.483, 5);
						glVertex3d(x0, y0, 5);
						glVertex3d(x, y, 5);
						glEnd();

						x0 = x;
						y0 = y;
					}
					for (double i = 0.8; i <= 1.25; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(8, 3, 5);
						glVertex3d(x0, y0, 5);
						glVertex3d(x, y, 5);
						glEnd();

						x0 = x;
						y0 = y;
					}
					glDisable(GL_BLEND);
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(14.0 / 5.0, 14.0 / 5.0);
						glVertex3d(8, 3, 5);
						glTexCoord2d(49.0 / 16.0, 16.0 / 5.0);
						glVertex3d(10.69, 7.483, 5);
						glTexCoord2d(51.0 / 16.0, 73.0 / 20.0);
						glVertex3d(13.125 + 6.0104076 / 2 * cos(0.8 * 3.141593), 2.125 + 6.0104076 / 2 * sin(0.8 * 3.141593), 5);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(75.0 / 16.0, 86.0 / 20.0);
						glVertex3dv(B1);
						glTexCoord2d(58.0 / 16.0, 83.0 / 20.0);
						glVertex3dv(C1);
						glTexCoord2d(15.0 / 5.0, 15.0 / 5.0);
						glVertex3dv(H1);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(58.0 / 16.0, 83.0 / 20.0);
						glVertex3dv(C1);
						glTexCoord2d(12.0 / 5.0, 1);
						glVertex3dv(D1);
						glTexCoord2d(49.0 / 16.0, 83.0 / 20.0);
						glVertex3dv(E1);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(15.0 / 5.0, 15.0 / 5.0);
						glVertex3dv(H1);
						glTexCoord2d(49.0 / 16.0, 83.0 / 20.0);
						glVertex3dv(E1);
						glTexCoord2d(49.0 / 16.0, 16.0 / 5.0);
						glVertex3d(10.69, 7.483, 5);
						glEnd();

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(58.0 / 16.0, 83.0 / 20.0);
						glVertex3dv(C1);
						glTexCoord2d(51.0 / 16.0, 83.0 / 20.0);
						glVertex3dv(E1);
						glTexCoord2d(15.0 / 5.0, 15.0 / 5.0);
						glVertex3dv(H1);

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(49.0 / 16.0, 16.0 / 5.0);
						glVertex3d(10.69, 7.483, 5);
						glTexCoord2d(12.0 / 5.0, 14.0 / 5.0);
						glVertex3d(8, 3, 5);
						glTexCoord2d(15.0 / 5.0, 15.0 / 5.0);
						glVertex3dv(H1);

						glBegin(GL_TRIANGLES);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glTexCoord2d(73.0 / 16.0, 69.0 / 20.0);
						glVertex3dv(A1);
						glTexCoord2d(75.0 / 16.0, 86.0 / 20.0);
						glVertex3dv(B1);
						glTexCoord2d(15.0 / 5.0, 15.0 / 5.0);
						glVertex3dv(H1);
						glEnd();

						glDisable(GL_BLEND);
					}
					if (textureReplace)
						glBindTexture(GL_TEXTURE_2D, texId);
					else
						glBindTexture(GL_TEXTURE_2D, texId2);
					x0 = 0;
					y0 = 9;

					for (double i = -1.25; i <= -0.55; i += 0.0001)
					{
						double x = 1.5 + 6.08276253 / 2 * cos(i * 3.141593);
						double y = 6 + 6.08276253 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3dv(A);
						glVertex3d(x0, y0, 0);
						glVertex3d(x, y, 0);
						glEnd();

						x0 = x;
						y0 = y;
					}

					x0 = 14;
					y0 = 5;

					for (double i = 0.406; i <= 0.85; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(10.69, 7.483, 0);
						glVertex3d(x0, y0, 0);
						glVertex3d(x, y, 0);
						glEnd();

						x0 = x;
						y0 = y;
					}

					for (double i = 0.8; i <= 1.25; i += 0.1)
					{
						double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
						double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

						glBegin(GL_POLYGON);
						glNormal3d(0, 0, -1);
						glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
						glVertex3d(8, 3, 0);
						glVertex3d(x0, y0, 0);
						glVertex3d(x, y, 0);
						glEnd();

						x0 = x;
						y0 = y;
					}

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(6.0 / 5.0, 14.0 / 5.0);
					glVertex3d(8, 3, 0);
					glTexCoord2d(31.0 / 16.0, 16.0 / 5.0);
					glVertex3d(10.69, 7.483, 0);
					glTexCoord2d(29.0 / 16.0, 73.0 / 20.0);
					glVertex3d(13.125 + 6.0104076 / 2 * cos(0.8 * 3.141593), 2.125 + 6.0104076 / 2 * sin(0.8 * 3.141593), 0);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(5.0 / 16.0, 86.0 / 20.0);
					glVertex3dv(B);
					glTexCoord2d(22.0 / 16.0, 83.0 / 20.0);
					glVertex3dv(C);
					glTexCoord2d(5.0 / 5.0, 15.0 / 5.0);
					glVertex3dv(H);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(22.0 / 16.0, 83.0 / 20.0);
					glVertex3dv(C);
					glTexCoord2d(8.0 / 5.0, 1);
					glVertex3dv(D);
					glTexCoord2d(31.0 / 16.0, 83.0 / 20.0);
					glVertex3dv(E);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(5.0 / 5.0, 15.0 / 5.0);
					glVertex3dv(H);
					glTexCoord2d(31.0 / 16.0, 83.0 / 20.0);
					glVertex3dv(E);
					glTexCoord2d(31.0 / 16.0, 16.0 / 5.0);
					glVertex3d(10.69, 7.483, 0);
					glEnd();

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(22.0 / 16.0, 83.0 / 20.0);
					glVertex3dv(C);
					glTexCoord2d(29.0 / 16.0, 83.0 / 20.0);
					glVertex3dv(E);
					glTexCoord2d(5.0 / 5.0, 15.0 / 5.0);
					glVertex3dv(H);

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(31.0 / 16.0, 16.0 / 5.0);
					glVertex3d(10.69, 7.483, 0);
					glTexCoord2d(6.0 / 5.0, 14.0 / 5.0);
					glVertex3d(8, 3, 0);
					glTexCoord2d(5.0 / 5.0, 15.0 / 5.0);
					glVertex3dv(H);

					glBegin(GL_TRIANGLES);
					glNormal3d(0, 0, -1);
					glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					glTexCoord2d(7.0 / 16.0, 69.0 / 20.0);
					glVertex3dv(A);
					glTexCoord2d(5.0 / 16.0, 86.0 / 20.0);
					glVertex3dv(B);
					glTexCoord2d(5.0 / 5.0, 15.0 / 5.0);
					glVertex3dv(H);
					glEnd();
				}

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(0.1428, 1, 0);
				glTexCoord2d(0, 1.0 / 5.0);
				glVertex3dv(B);
				glTexCoord2d(0, 5.0 / 5.0);
				glVertex3dv(B1);
				glTexCoord2d(4.0 / 5.0, 5.0 / 5.0);
				glVertex3dv(C1);
				glTexCoord2d(4.0 / 5.0, 1.0 / 5.0);
				glVertex3dv(C);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(-0.83, 0.6, 0);
				glTexCoord2d(5.0 / 5.0, 1.0 / 5.0);
				glVertex3dv(C);
				glTexCoord2d(5.0 / 5.0, 5.0 / 5.0);
				glVertex3dv(C1);
				glTexCoord2d(9.0 / 5.0, 5.0 / 5.0);
				glVertex3dv(D1);
				glTexCoord2d(9.0 / 5.0, 1.0 / 5.0);
				glVertex3dv(D);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(1, -0.17, 0);
				glTexCoord2d(10.0 / 5.0, 1.0 / 5.0);
				glVertex3dv(D);
				glTexCoord2d(10.0 / 5.0, 5.0 / 5.0);
				glVertex3dv(D1);
				glTexCoord2d(14.0 / 5.0, 5.0 / 5.0);
				glVertex3dv(E1);
				glTexCoord2d(14.0 / 5.0, 1.0 / 5.0);
				glVertex3dv(E);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(0.6, 0.8, 0);
				glTexCoord2d(0, 6.0 / 5.0);
				glVertex3dv(E);
				glTexCoord2d(0, 10.0 / 5.0);
				glVertex3dv(E1);
				glTexCoord2d(4.0 / 5.0, 10.0 / 5.0);
				glVertex3dv(F1);
				glTexCoord2d(4.0 / 5.0, 6.0 / 5.0);
				glVertex3dv(F);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(-0.71, -0.71, 0);
				glTexCoord2d(5.0 / 5.0, 6.0 / 5.0);
				glVertex3dv(G);
				glTexCoord2d(5.0 / 5.0, 10.0 / 5.0);
				glVertex3dv(G1);
				glTexCoord2d(9.0 / 5.0, 10.0 / 5.0);
				glVertex3dv(H1);
				glTexCoord2d(9.0 / 5.0, 6.0 / 5.0);
				glVertex3dv(H);
				glEnd();

				glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.3f);
				glNormal3d(0.37, -0.93, 0);
				glTexCoord2d(10.0 / 5.0, 6.0 / 5.0);
				glVertex3dv(A);
				glTexCoord2d(10.0 / 5.0, 10.0 / 5.0);
				glVertex3dv(A1);
				glTexCoord2d(14.0 / 5.0, 10.0 / 5.0);
				glVertex3dv(H1);
				glTexCoord2d(14.0 / 5.0, 6.0 / 5.0);
				glVertex3dv(H);
				glEnd();

				double x0 = 0;
				double y0 = 9;

				double tx0 = 0;
				double ty0 = 0;

				double Det[] = { 0, 0, 0 };
				double Det1[] = { 0, 0, 5 };
				double N[] = { 0, 0, 0 };
				double Nl;

				for (double i = -1.25; i <= -0.55; i += 0.0001)
				{
					double x = 1.5 + 6.08276253 / 2 * cos(i * 3.141593);
					double y = 6 + 6.08276253 / 2 * sin(i * 3.141593);

					double tx = tx0 + 1.0 / (30000.0 / 3.0);
					double ty = ty0 + 1.0 / (30000.0 / 3.0);

					glBegin(GL_QUADS);
					glColor3d(0.3f, 0.3f, 0.3f);
					Det[0] = { x - (1.5 + 6.08276253 / 2 * cos((i - 0.0001) * 3.141593)) };
					Det[1] = { y - (6 + 6.08276253 / 2 * sin((i - 0.0001) * 3.141593)) };
					N[0] = { Det[1] * Det1[2] - Det1[1] * Det[2] };
					N[1] = { -Det[0] * Det1[2] + Det1[0] * Det[2] };
					N[2] = { Det[0] * Det1[1] - Det1[0] * Det[1] };
					Nl = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
					N[0] = { N[0] / Nl };
					N[1] = { N[1] / Nl };
					N[2] = { N[2] / Nl };
					glNormal3d(N[0], N[1], N[2]);

					glTexCoord2d(15.0 / 5.0, tx0);
					glVertex3d(x0, y0, 0);
					glTexCoord2d(15.0 / 5.0, tx);
					glVertex3d(x, y, 0);
					glTexCoord2d(19.0 / 5.0, ty);
					glVertex3d(x, y, 5);
					glTexCoord2d(19.0 / 5.0, ty0);
					glVertex3d(x0, y0, 5);
					glEnd();

					x0 = x;
					y0 = y;
					tx0 = tx;
					ty0 = ty;
				}

				x0 = 14;
				y0 = 5;

				tx0 = 7.0 / 5.0;
				ty0 = 7.0 / 5.0;

				for (double i = 0.406; i <= 1.25; i += 0.0001)
				{
					double x = 13.125 + 6.0104076 / 2 * cos(i * 3.141593);
					double y = 2.125 + 6.0104076 / 2 * sin(i * 3.141593);

					double tx = tx0 + 0.5 / (168800.0 / 13.0);
					double ty = ty0 + 0.5 / (168800.0 / 13.0);

					glBegin(GL_POLYGON);
					glColor3d(0.3f, 0.3f, 0.3f);
					Det[0] = { (13.125 + 6.0104076 / 2 * cos((i + 0.0001) * 3.141593)) - x };
					Det[1] = { (13.125 + 6.0104076 / 2 * sin((i + 0.0001) * 3.141593)) - y };
					N[0] = { Det[1] * Det1[2] - Det1[1] * Det[2] };
					N[1] = { -Det[0] * Det1[2] + Det1[0] * Det[2] };
					N[2] = { Det[0] * Det1[1] - Det1[0] * Det[1] };
					Nl = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
					N[0] = { N[0] / Nl };
					N[1] = { N[1] / Nl };
					N[2] = { N[2] / Nl };
					glNormal3d(N[0], N[1], N[2]);

					glTexCoord2d(15.0 / 5.0, tx0);
					glVertex3d(x0, y0, 0);
					glTexCoord2d(15.0 / 5.0, tx);
					glVertex3d(x, y, 0);
					glTexCoord2d(19.0 / 5.0, ty);
					glVertex3d(x, y, 5);
					glTexCoord2d(19.0 / 5.0, ty0);
					glVertex3d(x0, y0, 5);
					glEnd();

					x0 = x;
					y0 = y;
					tx0 = tx;
					ty0 = ty;
				}

			}






		}



		else
		{

			glBindTexture(GL_TEXTURE_2D, texId);
			glBegin(GL_POLYGON);
			for (double i = 0; i <= 2; i += 0.01)
			{
				double x = 9 * cos(i * 3.141593);
				double y = 9 * sin(i * 3.141593);

				double tx = cos(i * 3.141593) * 0.5 + 0.5;
				double ty = sin(i * 3.141593) * 0.5 + 0.5;

				glColor3d(0.5f, 0.5f, 0.5f);
				glNormal3d(0, 0, 1);
				glTexCoord2d(tx, ty);
				glVertex3d(x, y, 0);

			}
			glEnd();

		}
	}
	else
	{
		//������ ��������� ���������� ��������

		double A2[2] = { -4, -4 };
		double B2[2] = { 4, -4 };
		double C2[2] = { 4, 4 };
		double D2[2] = { -4, 4 };

		glBindTexture(GL_TEXTURE_2D, texId);

		glColor3d(0.6, 0.6, 0.6);
		glBegin(GL_QUADS);
		glNormal3d(0, 0, 1);
		glTexCoord2d(0, 0);
		glVertex2dv(A2);
		glTexCoord2d(1, 0);
		glVertex2dv(B2);
		glTexCoord2d(1, 1);
		glVertex2dv(C2);
		glTexCoord2d(0, 1);
		glVertex2dv(D2);

		glEnd();

		////����� ��������� ���������� ��������
		}
		//��������� ������ ������


		glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
		//(���� ��������� ��������, ����� �� ������������.)
		glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
		glLoadIdentity();	  //��������� ��������� �������
		glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

		glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
		glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
		glLoadIdentity();		  //���������� �� � ������

		glDisable(GL_LIGHTING);



		GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
		rec.setSize(300, 200);
		rec.setPosition(10, ogl->getHeight() - 200 - 10);


		std::stringstream ss;
		ss << "T - ���/���� �������" << std::endl;
		ss << "E - ������������ �������" << std::endl;
		ss << "C - ����" << std::endl;
		ss << "Q - �������" << std::endl;
		ss << "L - ���/���� ���������" << std::endl;
		ss << "F - ���� �� ������" << std::endl;
		ss << "G - ������� ���� �� �����������" << std::endl;
		ss << "G+��� ������� ���� �� ���������" << std::endl;
		ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
		ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
		ss << "��������� ������: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;


		rec.setText(ss.str().c_str());
		rec.Draw();

		glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
		glPopMatrix();


		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	}