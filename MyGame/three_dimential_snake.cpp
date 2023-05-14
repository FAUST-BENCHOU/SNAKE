#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>//windows编程头文件
#include <time.h>
#include <conio.h>//控制台输入输出头文件
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <thread>
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F //将超出纹理坐标范围的部分取最近的边界像素值作为纹理的采样值。
#define TO_RAD (3.14159265/180.0); 

const float GREY_gl[4] = { 0.2, 0.2, 0.2, 1.0 },
WHITE_gl[4] = { 1.0, 1.0, 1.0, 1.0 },
BLACK_gl[4] = { 0.0, 0.0, 0.0, 1.0 },
GOLD_gl[4] = { 0.8, 0.7, 0.3, 1 },
BROWN_gl[4] = { 0.5, 0.3, 0.1, 1 },
YELLOW_gl[4] = { 0.675, 0.566, 0.324, 1 };

GLuint texId[7];
enum Texture { SKYBOX_BACK, SKYBOX_TOP, STONE, BRICK, BRICK_SNAKE_FRONT, BRICK_SNAKE_FRONT_S, BRICK_SNAKE_BACK };

struct Vertex {
	float x;
	float y;
	float z;
};

struct Door {
	bool open = false;
	bool opening = false;
	bool closing = false;
	float angle = 0;
} door;

struct MovingObject {
	float x;
	float y;
	float z;
	float angle;
} cam, snake_gl;

bool light1Enabled = true;

int change_colour = 0;

void loadTGA(const char* filename)
{
	char id, cmap, imgtype, bpp, c_garb;
	char* imageData, temp;
	short int s_garb, wid, hgt;
	int nbytes, size, indx;
	ifstream file(filename, ios::in | ios::binary);
	if (!file)
	{
		cout << "*** Error opening image file: " << filename << endl;
		exit(1);
	}
	file.read(&id, 1);
	file.read(&cmap, 1);
	file.read(&imgtype, 1);
	if (imgtype != 2 && imgtype != 3)   //2= colour (uncompressed),  3 = greyscale (uncompressed)
	{
		cout << "*** Incompatible image type: " << (int)imgtype << endl;
		exit(1);
	}
	//Color map specification
	file.read((char*)&s_garb, 2);
	file.read((char*)&s_garb, 2);
	file.read(&c_garb, 1);
	//Image specification
	file.read((char*)&s_garb, 2);  //x origin
	file.read((char*)&s_garb, 2);  //y origin
	file.read((char*)&wid, 2);     //image width								    
	file.read((char*)&hgt, 2);     //image height
	file.read(&bpp, 1);     //bits per pixel
	file.read(&c_garb, 1);  //img descriptor
	nbytes = bpp / 8;           //No. of bytes per pixels
	size = wid * hgt * nbytes;  //Total number of bytes to be read
	imageData = new char[size];
	file.read(imageData, size);
	if (nbytes > 2)   //swap R and B
	{
		for (int i = 0; i < wid * hgt; i++)
		{
			indx = i * nbytes;
			temp = imageData[indx];
			imageData[indx] = imageData[indx + 2];
			imageData[indx + 2] = temp;
		}
	}

	switch (nbytes)
	{
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, 1, wid, hgt, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
		break;
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, 3, wid, hgt, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		break;
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, 4, wid, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		break;
	}
	delete imageData;
}

void vertex(Vertex* v) {
	glVertex3f(v->x, v->y, v->z);
}

void normal(Vertex* v1, Vertex* v2, Vertex* v3, bool reversed) {
	float x1 = v1->x, y1 = v1->y, z1 = v1->z;
	float x2 = v2->x, y2 = v2->y, z2 = v2->z;
	float x3 = v3->x, y3 = v3->y, z3 = v3->z;

	float nx, ny, nz;
	nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

	if (reversed) {
		glNormal3f(nx, ny, nz);
	}
	else {
		glNormal3f(-nx, -ny, -nz);
	}
}

void loadGLTextures() {
	glGenTextures(7, texId); 		// 生成纹理对象
	//back
	glBindTexture(GL_TEXTURE_2D, texId[SKYBOX_BACK]);
	loadTGA("textures/skybox/back.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// top
	glBindTexture(GL_TEXTURE_2D, texId[SKYBOX_TOP]);
	loadTGA("textures/skybox/top.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, texId[STONE]);
	loadTGA("textures/stone.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[BRICK]);
	loadTGA("textures/brick.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[BRICK_SNAKE_FRONT]);
	loadTGA("textures/brick_snake_front.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[BRICK_SNAKE_FRONT_S]);
	loadTGA("textures/brick_snake_front_s.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[BRICK_SNAKE_BACK]);
	loadTGA("textures/brick_snake_back.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawSkybox() {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	////////////////////// ELSE WALL ////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[SKYBOX_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(1000, 0, 1000);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1000, 0, 1000);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1000, 1000, 1000);
	glTexCoord2f(1.0, 1.0); glVertex3f(1000, 1000, 1000);

	glTexCoord2f(1.0, 0.0); glVertex3f(-1000, 0, 1000);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1000, 0, -1000);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1000, 1000, -1000);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1000, 1000, 1000);

	glTexCoord2f(0.0, 0.0); glVertex3f(-1000, 0, 1000);
	glTexCoord2f(1.0, 0.0); glVertex3f(1000, 0, 1000);
	glTexCoord2f(1.0, 1.0); glVertex3f(1000, 0, -1000);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1000, 0, -1000);

	glTexCoord2f(1.0, 0.0); glVertex3f(1000, 0, -1000);
	glTexCoord2f(0.0, 0.0); glVertex3f(1000, 0, 1000);
	glTexCoord2f(0.0, 1.0); glVertex3f(1000, 1000, 1000);
	glTexCoord2f(1.0, 1.0); glVertex3f(1000, 1000, -1000);

	glTexCoord2f(1.0, 0.0); glVertex3f(-1000, 0, -1000);
	glTexCoord2f(0.0, 0.0); glVertex3f(1000, 0, -1000);
	glTexCoord2f(0.0, 1.0); glVertex3f(1000, 1000, -1000);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1000, 1000, -1000);
	glEnd();

	/////////////////////// TOP //////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[SKYBOX_TOP]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1000, 1000, -1000);
	glTexCoord2f(1.0, 0.0); glVertex3f(1000, 1000, -1000);
	glTexCoord2f(0.0, 0.0); glVertex3f(1000, 1000, 1000);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1000, 1000, 1000);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawGround() {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[STONE]);

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (int x = -400; x <= 400; x += 20) {
		for (int z = -400; z <= 400; z += 20) {
			glTexCoord2f(0.0, 0.0); glVertex3f(x, 0, z);
			glTexCoord2f(0.0, 1.0); glVertex3f(x, 0, z + 20);
			glTexCoord2f(1.0, 1.0); glVertex3f(x + 20, 0, z + 20);
			glTexCoord2f(1.0, 0.0); glVertex3f(x + 20, 0, z);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawDoor1() {


	glTranslatef(0.15, 0, 2.5);
	glRotatef(door.angle, 0, 1, 0);
	glTranslatef(-0.15, 0, 0);

	glPushMatrix();
	glColor4fv(GOLD_gl);
	glScalef(0.15, 0.5, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLACK_gl);
	glColor4f(0.15, 0.15, 0.15, 1);
	glScalef(0.3, 1, 0.02);
	glutSolidCube(1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE_gl);
	glPopMatrix();

	glColor4f(1, 1, 1, 1);

}
void drawDoor2() {


	glTranslatef(0.15, 0, 2.5);
	glRotatef(door.angle, 0, -1, 0);
	glTranslatef(-0.15, 0, 0);

	glPushMatrix();
	glColor4fv(GOLD_gl);
	glScalef(0.15, 0.5, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLACK_gl);
	glColor4f(0.15, 0.15, 0.15, 1);
	glScalef(0.3, 1, 0.02);
	glutSolidCube(1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE_gl);
	glPopMatrix();

	glColor4f(1, 1, 1, 1);

}

//画整个地图
void drawMaze(bool reversed) {
	Vertex v[20];
	// Front Face Bottom
	v[1] = { 1.2, 0, 6 };
	v[2] = { 1.8, 0, 5 };

	v[3] = { 7.2, 0, 6 };
	v[4] = { 6.6, 0, 5 };

	v[5] = { 7.2, 0, 0 };
	v[6] = { 6.6, 0, 1 };

	v[7] = { 1.2, 0, 0 };
	v[8] = { 1.8, 0, 1 };
	// Front Face Mid

	v[9] = { 1.2, 1, 6 };
	v[10] = { 1.8, 1, 5 };

	v[11] = { 7.2, 1, 6 };
	v[12] = { 6.6, 1, 5 };

	v[13] = { 7.2, 1, 0 };
	v[14] = { 6.6, 1, 1 };

	v[15] = { 1.2, 1, 0 };
	v[16] = { 1.8, 1, 1 };
	// Entrance Bottom

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[BRICK]);

	glBegin(GL_QUADS);

	//1 big wall //
	normal(&v[1], &v[7], &v[15], reversed);
	glTexCoord2f(v[1].z, v[1].y); glVertex3f(v[1].x, v[1].y, v[1].z);
	glTexCoord2f(v[7].z, v[7].y); glVertex3f(v[7].x, v[7].y, v[7].z);
	glTexCoord2f(v[15].z, v[15].y); glVertex3f(v[15].x, v[15].y, v[15].z);
	glTexCoord2f(v[9].z, v[9].y); glVertex3f(v[9].x, v[9].y, v[9].z);

	//1 small wall //
	normal(&v[2], &v[8], &v[16], reversed);
	glTexCoord2f(v[2].z, v[2].y); glVertex3f(v[2].x, v[2].y, v[2].z);
	glTexCoord2f(v[8].z, v[8].y); glVertex3f(v[8].x, v[8].y, v[8].z);
	glTexCoord2f(v[16].z, v[16].y); glVertex3f(v[16].x, v[16].y, v[16].z);
	glTexCoord2f(v[10].z, v[10].y); glVertex3f(v[10].x, v[10].y, v[10].z);


	//3 big wall //
	normal(&v[11], &v[3], &v[5], reversed);
	glTexCoord2f(v[11].z, v[11].y); glVertex3f(v[11].x, v[11].y, v[11].z);
	glTexCoord2f(v[3].z, v[3].y); glVertex3f(v[3].x, v[3].y, v[3].z);
	glTexCoord2f(v[5].z, v[5].y); glVertex3f(v[5].x, v[5].y, v[5].z);
	glTexCoord2f(v[13].z, v[13].y); glVertex3f(v[13].x, v[13].y, v[13].z);

	//3 small wall //
	normal(&v[12], &v[4], &v[6], reversed);
	glTexCoord2f(v[12].z, v[12].y); glVertex3f(v[12].x, v[12].y, v[12].z);
	glTexCoord2f(v[4].z, v[4].y); glVertex3f(v[4].x, v[4].y, v[4].z);
	glTexCoord2f(v[6].z, v[6].y); glVertex3f(v[6].x, v[6].y, v[6].z);
	glTexCoord2f(v[14].z, v[14].y); glVertex3f(v[14].x, v[14].y, v[14].z);

	glEnd();
	glDisable(GL_TEXTURE_2D);


	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[BRICK_SNAKE_BACK]);
	glBegin(GL_QUADS);
	//2 big wall //
	normal(&v[5], &v[7], &v[15], reversed);
	glTexCoord2f(v[5].x, v[5].y); glVertex3f(v[5].x, v[5].y, v[5].z);
	glTexCoord2f(v[7].x, v[7].y); glVertex3f(v[7].x, v[7].y, v[7].z);
	glTexCoord2f(v[15].x, v[15].y); glVertex3f(v[15].x, v[15].y, v[15].z);
	glTexCoord2f(v[13].x, v[13].y); glVertex3f(v[13].x, v[13].y, v[13].z);

	//2 small wall //
	normal(&v[6], &v[8], &v[16], reversed);
	glTexCoord2f(v[6].x, v[6].y); glVertex3f(v[6].x, v[6].y, v[6].z);
	glTexCoord2f(v[8].x, v[8].y); glVertex3f(v[8].x, v[8].y, v[8].z);
	glTexCoord2f(v[16].x, v[16].y); glVertex3f(v[16].x, v[16].y, v[16].z);
	glTexCoord2f(v[14].x, v[14].y); glVertex3f(v[14].x, v[14].y, v[14].z);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[BRICK_SNAKE_FRONT_S]);
	glBegin(GL_QUADS);

	//4 big wall //
	normal(&v[3], &v[1], &v[9], reversed);
	glTexCoord2f(v[3].x, v[3].y); glVertex3f(v[3].x, v[3].y, v[3].z);
	glTexCoord2f(v[1].x, v[1].y); glVertex3f(v[1].x, v[1].y, v[1].z);
	glTexCoord2f(v[9].x, v[9].y); glVertex3f(v[9].x, v[9].y, v[9].z);
	glTexCoord2f(v[11].x, v[11].y); glVertex3f(v[11].x, v[11].y, v[11].z);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[BRICK_SNAKE_FRONT]);
	glBegin(GL_QUADS);
	//4 small wall //
	normal(&v[4], &v[2], &v[10], reversed);
	glTexCoord2f(v[4].x, v[4].y); glVertex3f(v[4].x, v[4].y, v[4].z);
	glTexCoord2f(v[2].x, v[2].y); glVertex3f(v[2].x, v[2].y, v[2].z);
	glTexCoord2f(v[10].x, v[10].y); glVertex3f(v[10].x, v[10].y, v[10].z);
	glTexCoord2f(v[12].x, v[12].y); glVertex3f(v[12].x, v[12].y, v[12].z);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawCompleteMaze() {
	glTranslatef(100, 0, -100);
	glScalef(50, 50, 50);
	glRotatef(90, 0, 1, 0);
	glTranslatef(-1.5, 0, -1.5); // to centre the pyramid

	// Left Door
	glPushMatrix();
	glTranslatef(1.35 + 0.001, 0.5 - 0.001, -0.235);
	glScalef(-1, 1, 1);
	drawDoor1();
	glPopMatrix();

	// Right Door
	glPushMatrix();
	glTranslatef(1.65 - 0.001, 0.5 - 0.001, -0.235);
	drawDoor1();
	glPopMatrix();

	// Left Door
	glPushMatrix();
	glTranslatef(6.63 + 0.001, 0.5 - 0.001, -0.235);
	glScalef(-1, 1, 1);
	drawDoor2();
	glPopMatrix();

	// Right Door
	glPushMatrix();
	glTranslatef(6.93 - 0.001, 0.5 - 0.001, -0.235);
	drawDoor2();
	glPopMatrix();

	glDisable(GL_LIGHT0);
	if (light1Enabled) {
		glEnable(GL_LIGHT1);
	}

	glPushMatrix();
	glTranslatef(0.03, 0, 0.01);
	glScalef(0.98, 0.98, 0.98);
	drawMaze(true);
	glPopMatrix();

	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);

}

//画蛇
void drawSnake_gl() {

	glPushMatrix();

	// 绘制蛇头

	glTranslatef(0.0f, 0.0f, -10.0f);
	if (change_colour == 0) {
		glColor3f(0.0196, 0.4, 0.0392);
	}
	else if (change_colour == 1) {
		glColor3f(0.176, 0.176, 0.176);
	}
	else if (change_colour == 2) {
		glColor3f(0.263, 0.129, 0.055);
	}
	else if (change_colour == 3) {
		glColor3f(0.8039, 0.7451, 0.4392);
	}
	glScalef(0.8, 0.5, 1);
	glutSolidSphere(1.5f, 20, 20);

	glScalef(1.25, 2, 1);

	// 绘制眼睛
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(-1.0f, 0.7f, 2.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(0.3f, 20, 20);
	glPopMatrix();


	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(1.0f, 0.7f, 2.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(0.3f, 20, 20);
	glPopMatrix();

	glScalef(1, 1, 2);
	glTranslatef(0.0f, 0.0f, -2.8);

	// 绘制蛇舌
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0);
	glColor3f(0.9f, 0.1f, 0.1f);
	glBegin(GL_QUADS);

	glVertex3f(-0.1f, -0.1f, 3.0f);
	glVertex3f(0.1f, -0.1f, 3.0f);
	glVertex3f(0.1f, -0.1f, 4.0f);
	glVertex3f(-0.1f, -0.1f, 4.0f);

	glVertex3f(0.1f, -0.1f, 3.0f);
	glVertex3f(0.1f, -0.1f, 4.0f);
	glVertex3f(0.1f, -0.2f, 4.0f);
	glVertex3f(0.1f, -0.2f, 3.0f);

	glVertex3f(-0.1f, -0.1f, 3.0f);
	glVertex3f(-0.1f, -0.1f, 4.0f);
	glVertex3f(-0.1f, -0.2f, 4.0f);
	glVertex3f(-0.1f, -0.2f, 3.0f);


	glVertex3f(0.1f, -0.1f, 4.0f);
	glVertex3f(0.1f, -0.2f, 4.0f);
	glVertex3f(0.2f, -0.2f, 4.5f);
	glVertex3f(0.2f, -0.1f, 4.5f);

	glVertex3f(-0.1f, -0.1f, 4.0f);
	glVertex3f(-0.1f, -0.2f, 4.0f);
	glVertex3f(-0.2f, -0.2f, 4.5f);
	glVertex3f(-0.2f, -0.1f, 4.5f);

	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(0, -0.1f, 4);
	glVertex3f(0.1f, -0.1f, 4);
	glVertex3f(0.2f, -0.1f, 4.5f);

	glVertex3f(0, -0.1f, 4.0f);
	glVertex3f(-0.2f, -0.1f, 4.5f);
	glVertex3f(-0.1f, -0.1f, 4.0f);

	glEnd();
	glPopMatrix();

	//蛇身
	for (int i = 0; i < 18; i++) {
		glPushMatrix();
		if (change_colour == 0) {
			glColor3f(0.0196, 0.4, 0.0392);
		}
		else if (change_colour == 1) {
			glColor3f(0.176, 0.176, 0.176);
		}
		else if (change_colour == 2) {
			glColor3f(0.263, 0.129, 0.055);
		}
		else if (change_colour == 3) {
			glColor3f(0.8039, 0.7451, 0.4392);
		}
		glRotatef(i * 10.0f - 90, 0.0f, 1.0f, 0.0f);
		glTranslatef(2.0f, 0.0f, 0);
		glutSolidCube(0.5);
		glPopMatrix();
	}
	glTranslatef(0.0f, 0.0f, -4.0);
	for (int i = 0; i < 18; i++) {
		glPushMatrix();
		if (change_colour == 0) {
			glColor3f(0.0196, 0.4, 0.0392);
		}
		else if (change_colour == 1) {
			glColor3f(0.176, 0.176, 0.176);
		}
		else if (change_colour == 2) {
			glColor3f(0.263, 0.129, 0.055);
		}
		else if (change_colour == 3) {
			glColor3f(0.8039, 0.7451, 0.4392);
		}
		glRotatef(-i * 10.0f - 90, 0.0f, 1.0f, 0.0f);
		glTranslatef(2.0f, 0.0f, 0);
		glutSolidCube(0.5);
		glPopMatrix();
	}

	glPopMatrix();
	glColor4fv(WHITE_gl);
	glDisable(GL_TEXTURE_2D);
}

void timer_callback() {
	glutLeaveMainLoop();
}

int count_time = 0;
void display() {
	// Check if 80 seconds have elapsed
	if (count_time==3200)
	{
		glutLeaveMainLoop();
		// Exit the loop

	}
	float lpos[4] = { -1000.0, 1000.0, 1000.0, 1.0 };  //light's position
	float shadowMat[16] = {
			lpos[1], 0, 0, 0,	-lpos[0], 0, -lpos[2], -1,
			0, 0, lpos[1], 0,	0, 0, 0, lpos[1]
	};
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			 //GL_LINE = Wireframe;   GL_FILL = Solid //设置多边形的绘制模式为填充模式，即将多边形内部填充。
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 //空颜色缓冲区和深度缓冲区
	glMatrixMode(GL_MODELVIEW);							 //模型视图矩阵

	glLoadIdentity();									 //用于将当前矩阵重置为单位矩阵，相当于将当前变换的矩阵清空，变为默认状态

	gluLookAt(cam.x, cam.y, cam.z, cam.x + cos(cam.angle), cam.y, cam.z + sin(cam.angle), 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position

	glPushMatrix();
	glTranslatef(0, -2000, 0);
	glScalef(5, 4, 5);
	drawSkybox();
	glPopMatrix();

	// NORMAL LIGHTING //
	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	drawGround();
	glPopMatrix();

	glPushMatrix();
	drawCompleteMaze();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(snake_gl.x, snake_gl.y, snake_gl.z);
	glScalef(10, 10, 10);
	drawSnake_gl();
	glPopMatrix();

	int x_1[10] = { 22,12,30,15,3,35,29,19,9,3 }, y_1[10] = { 19, 27, 33, 30, 10, 23, 35, 14, 37, 22 }, z_1[10] = { 31,76,220,165,73,22,150,98,200,180 };
	int x_2[10] = { 9, 35, 7, 11, 36, 16, 38, 23, 14, 20 }, y_2[10] = { 6, 29, 8, 21, 35, 36, 13, 22, 38, 14 }, z_2[10] = { 183, 107, 39, 155, 28, 201, 79, 130, 14, 181 };
	int x_3[10] = { 19, 4, 15, 6, 12, 22, 30, 7, 39, 33 }, y_3[10] = { 34, 28, 16, 7, 37, 20, 12, 11, 24, 31 }, z_3[10] = { 77, 222, 71, 46, 234, 58, 154, 56, 221, 210 };
	int x_4[10] = { 31, 36, 27, 9, 9, 35, 6, 38, 30, 14 }, y_4[10] = { 15, 39, 31, 5, 25, 18, 35, 18, 14, 10 }, z_4[10] = { 85, 24, 233, 218, 112, 81, 237, 210, 41, 200 };
	for (int i = 0; i < 10; i++) {
		//x:back_s=70   back_b=30         left_s=-350  right_s=-110 
		glPushMatrix();
		glTranslatef(30 + x_1[i], y_1[i], -110 - z_1[i]);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		drawSnake_gl();
		glPopMatrix();
	}
	change_colour = 1;
	for (int i = 0; i < 10; i++) {
		//x:back_s=70   back_b=30       left_s=-350  right_s=-110 
		glPushMatrix();
		glTranslatef(30 + x_2[i], y_2[i], -110 - z_2[i]);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		drawSnake_gl();
		glPopMatrix();
	}
	change_colour = 2;
	for (int i = 0; i < 10; i++) {
		//x:back_s=70   back_b=30        left_s=-350  right_s=-110 
		glPushMatrix();
		glTranslatef(30 + x_3[i], y_3[i], -110 - z_3[i]);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		drawSnake_gl();
		glPopMatrix();
	}
	change_colour = 3;
	for (int i = 0; i < 10; i++) {
		//x:back_s=70   back_b=30          left_s=-350  right_s=-110 
		glPushMatrix();
		glTranslatef(30 + x_4[i], y_4[i], -110 - z_4[i]);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		drawSnake_gl();
		glPopMatrix();
	}
	change_colour = 0;
	glDisable(GL_LIGHT0);
	if (light1Enabled) {
		glEnable(GL_LIGHT1);
	}

	// NORMAL LIGHTING //
	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);

	glFlush();
}

void initialize() {
	loadGLTextures();											//加载纹理贴图。

	glEnable(GL_LIGHTING);										//启用光照功能
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);			//启用颜色材质功能，并指定使用当前前表面的环境光和散射光颜色来作为物体的颜色。
	glEnable(GL_COLOR_MATERIAL);								//启用颜色材质跟踪

	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE_gl);				//设置材质的镜面反射特性
	glMaterialf(GL_FRONT, GL_SHININESS, 50);					//设置材质的光泽度50

	glLightfv(GL_LIGHT0, GL_AMBIENT, GREY_gl);					//设置光源0的环境光颜色
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE_gl);					//设置光源0的散射光颜色
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE_gl);				//设置光源0的镜面反射光颜色

	glLightfv(GL_LIGHT1, GL_AMBIENT, GREY_gl);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, WHITE_gl);
	glLightfv(GL_LIGHT1, GL_SPECULAR, WHITE_gl);

	glClearColor(0, 0, 0, 0);

	glEnable(GL_DEPTH_TEST);									//启用深度测试，以便正确地渲染远近不同的物体
	glEnable(GL_NORMALIZE);										//法向量归一化

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 1, 10000);  //相机视图体积

	// Set camera's initial position & angle //
	cam.x = 55;
	cam.y = 30;
	cam.z = -100;
	cam.angle = 0.0 * TO_RAD

		// Set snake's initial position
		snake_gl.x = 295;	//x:back_s=70   back_b=30  front_s=275   front_b=315
	snake_gl.y = 10;
	snake_gl.z = -80;	//left_s=-350  lsft_b=-375   right_s=-110   right_b=-90

}

bool camWithinWalls() {
	int back_small = 70, back_big = 30, front_small = 275, front_big = 315, left_small = -355, left_big = -375, right_small = -110, right_big = -90;
	if ((cam.x >= back_big && cam.x <= back_small)) {
		if ((cam.z >= left_big && cam.z <= right_big)) {
			return 0;
		}
	}
	if ((cam.x >= front_small && cam.x <= front_big)) {
		if ((cam.z >= left_big && cam.z <= right_big)) {
			return 0;
		}
	}
	if ((cam.x >= back_big && cam.x <= front_big)) {
		if ((cam.z >= left_big && cam.z <= left_small)) {
			return 0;
		}
	}
	if ((cam.x >= back_big && cam.x <= front_big)) {
		if ((cam.z >= right_small && cam.z <= right_big)) {
			return 0;
		}
	}
	return 1;
}
bool camWithinDoorwayWalls() {
	return cam.x > 110 && cam.x < 170 && ((!door.open || door.closing));
}

void changeCamX(float amount) {
	cam.x += amount;
	if ((camWithinWalls()) || camWithinDoorwayWalls()) {
		cam.x -= amount;
	}
}
void changeCamZ(float amount) {
	cam.z += amount;
	if ((camWithinWalls()) || camWithinDoorwayWalls()) {
		cam.z -= amount;
	}
}

void special(int key, int x, int y) {
	const float CHANGE_VIEW_ANGLE = 2.0;
	const float MOVE_DISTANCE = 2.0;

	switch (key) {
	case GLUT_KEY_LEFT:

		cam.angle -= CHANGE_VIEW_ANGLE * TO_RAD;

		break;
	case GLUT_KEY_RIGHT:

		cam.angle += CHANGE_VIEW_ANGLE * TO_RAD;

		break;
	case GLUT_KEY_UP:

		changeCamX(MOVE_DISTANCE * cos(cam.angle));
		changeCamZ(MOVE_DISTANCE * sin(cam.angle));

		break;
	case GLUT_KEY_DOWN:

		changeCamX(-MOVE_DISTANCE * cos(cam.angle));
		changeCamZ(-MOVE_DISTANCE * sin(cam.angle));

		break;

	}
	glutPostRedisplay();
}

void moveDoors() {
	const float DETECT_X_LOWER = 100, DETECT_X_UPPER = 180;
	const float OPEN_ANGLE = 80, CLOSED_ANGLE = 0;
	count_time++;
	if (door.opening) {
		door.angle += 1;
		if (door.angle >= OPEN_ANGLE) {
			door.opening = false;
			door.open = true;
		}
	}
	else if (door.closing) {
		door.angle -= 1;
		if (door.angle <= CLOSED_ANGLE) {
			door.closing = false;
			door.open = false;
		}
	}
	else if (cam.x > DETECT_X_LOWER && cam.x < DETECT_X_UPPER) {
		if (!door.open) {
			door.opening = true;
		}
	}
	else if (door.open) {
		door.closing = true;
	}
};

void timer(int value) {
	moveDoors();
	glutPostRedisplay();
	glutTimerFunc(25, timer, 0);
	
}

void three_dimential_snake() {
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(1500, 850);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("SNAKE");
	initialize();

	glutSpecialFunc(special);

	glutTimerFunc(25, timer, 0);

	glutDisplayFunc(display);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutMainLoop();
	
	return;

}
