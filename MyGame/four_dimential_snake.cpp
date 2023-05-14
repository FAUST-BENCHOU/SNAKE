#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>//windows编程头文件
#include <time.h>
#include <conio.h>//控制台输入输出头文件
#include <GL/glut.h>
#include <GL/freeglut.h>
#
using namespace std;

#define GL_CLAMP_TO_EDGE_fourdim 0x812F //将超出纹理坐标范围的部分取最近的边界像素值作为纹理的采样值。
#define TO_RAD_fourdim (3.14159265/180.0); 

GLuint texId_fourdim[7];
enum Texture_fourdim { SKYBOX_BACK_S, SKYBOX_TOP_S, SKYBOX_LEFT_S, SKYBOX_RIGHT_S, SKYBOX_FRONT_S, SKYBOX_DOWN_S };

struct Vertex_fourdim {
	float x;
	float y;
	float z;
};



struct MovingObject_fourdim {
	float x;
	float y;
	float z;
	float angle;
} cam_fourdim;

void loadTGA_fourdim(const char* filename)
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

void vertex_fourdim(Vertex_fourdim* v) {
	glVertex3f(v->x, v->y, v->z);
}

void loadGLTextures_fourdim() {
	glGenTextures(7, texId_fourdim); 		// 生成纹理对象
	//back
	glBindTexture(GL_TEXTURE_2D, texId_fourdim[SKYBOX_BACK_S]);
	loadTGA_fourdim("textures2/1.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_fourdim);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_fourdim);

	// left
	glBindTexture(GL_TEXTURE_2D, texId_fourdim[SKYBOX_LEFT_S]);
	loadTGA_fourdim("textures2/2.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_fourdim);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_fourdim);

	// right
	glBindTexture(GL_TEXTURE_2D, texId_fourdim[SKYBOX_RIGHT_S]);
	loadTGA_fourdim("textures2/3.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_fourdim);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_fourdim);
	// front
	glBindTexture(GL_TEXTURE_2D, texId_fourdim[SKYBOX_FRONT_S]);
	loadTGA_fourdim("textures2/4.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_fourdim);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_fourdim);
	// down
	glBindTexture(GL_TEXTURE_2D, texId_fourdim[SKYBOX_DOWN_S]);
	loadTGA_fourdim("textures2/holl.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_fourdim);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_fourdim);

	glBindTexture(GL_TEXTURE_2D, texId_fourdim[SKYBOX_TOP_S]);
	loadTGA_fourdim("textures2/sky.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_fourdim);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_fourdim);

}

void drawSkybox_fourdim() {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	for (int i = 1; i <= 40; i = i + 1) {

		////////////////////// LEFT WALL ///////////////////////
		glBindTexture(GL_TEXTURE_2D, texId_fourdim[i % 6]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0); glVertex3f(-i, 0, i);
		glTexCoord2f(0.0, 0.0); glVertex3f(-i, 0, -i);
		glTexCoord2f(0.0, 1.0); glVertex3f(-i, 1.2 * i, -i);
		glTexCoord2f(1.0, 1.0); glVertex3f(-i, 1.2 * i, i);
		glEnd();

		////////////////////// FRONT WALL ///////////////////////
		glBindTexture(GL_TEXTURE_2D, texId_fourdim[i % 6]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0); glVertex3f(-i, 0, -i);
		glTexCoord2f(0.0, 0.0); glVertex3f(i, 0, -i);
		glTexCoord2f(0.0, 1.0); glVertex3f(i, 1.2 * i, -i);
		glTexCoord2f(1.0, 1.0); glVertex3f(-i, 1.2 * i, -i);
		glEnd();

		////////////////////// RIGHT WALL ///////////////////////
		glBindTexture(GL_TEXTURE_2D, texId_fourdim[i % 6]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0); glVertex3f(i, 0, -i);
		glTexCoord2f(0.0, 0.0); glVertex3f(i, 0, i);
		glTexCoord2f(0.0, 1.0); glVertex3f(i, 1.2 * i, i);
		glTexCoord2f(1.0, 1.0); glVertex3f(i, 1.2 * i, -i);
		glEnd();


		////////////////////// REAR WALL ////////////////////////
		glBindTexture(GL_TEXTURE_2D, texId_fourdim[i % 6]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0); glVertex3f(i, 0, i);
		glTexCoord2f(0.0, 0.0); glVertex3f(-i, 0, i);
		glTexCoord2f(0.0, 1.0); glVertex3f(-i, 1.2 * i, i);
		glTexCoord2f(1.0, 1.0); glVertex3f(i, 1.2 * i, i);
		glEnd();

		/////////////////////// TOP //////////////////////////
		glBindTexture(GL_TEXTURE_2D, texId_fourdim[i % 6]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0); glVertex3f(-i, 1.2 * i, -i);
		glTexCoord2f(1.0, 0.0); glVertex3f(i, 1.2 * i, -i);
		glTexCoord2f(0.0, 0.0); glVertex3f(i, 1.2 * i, i);
		glTexCoord2f(0.0, 1.0); glVertex3f(-i, 1.2 * i, i);
		glEnd();

		/////////////////////// FLOOR //////////////////////////
		glBindTexture(GL_TEXTURE_2D, texId_fourdim[i % 6]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-i, 0, i);
		glTexCoord2f(1.0, 0.0); glVertex3f(i, 0, i);
		glTexCoord2f(1.0, 1.0); glVertex3f(i, 0, -i);
		glTexCoord2f(0.0, 1.0); glVertex3f(-i, 0, -i);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}
int count_time_fourdim = 0;
void display_fourdim() {
	// Check if 80 seconds have elapsed
	if (count_time_fourdim==500)
	{
		glutLeaveMainLoop();
		// Exit the loop

	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			 //GL_LINE = Wireframe;   GL_FILL = Solid //设置多边形的绘制模式为填充模式，即将多边形内部填充。
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 //空颜色缓冲区和深度缓冲区
	glMatrixMode(GL_MODELVIEW);							 //模型视图矩阵

	glLoadIdentity();									 //用于将当前矩阵重置为单位矩阵，相当于将当前变换的矩阵清空，变为默认状态

	gluLookAt(cam_fourdim.x, cam_fourdim.y, cam_fourdim.z, cam_fourdim.x + cos(cam_fourdim.angle), cam_fourdim.y, cam_fourdim.z + sin(cam_fourdim.angle), 0, 1, 0);



	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(5, 4, 5);
	drawSkybox_fourdim();
	glPopMatrix();

	glFlush();
}

void initialize_fourdim() {
	loadGLTextures_fourdim();											//加载纹理贴图。

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);			//启用颜色材质功能，并指定使用当前前表面的环境光和散射光颜色来作为物体的颜色。
	glEnable(GL_COLOR_MATERIAL);								//启用颜色材质跟踪

	glClearColor(0, 0, 0, 0);

	glEnable(GL_DEPTH_TEST);									//启用深度测试，以便正确地渲染远近不同的物体
	glEnable(GL_NORMALIZE);										//法向量归一化

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(150, 1, 1, 10000);  //相机视图体积

	// Set camera's initial position & angle //
	cam_fourdim.x = 0;
	cam_fourdim.y = 9;
	cam_fourdim.z = 0;
	cam_fourdim.angle = 0.0 * TO_RAD_fourdim

}

void changeCamX_fourdim(float amount) {
	cam_fourdim.x += amount;
}
void changeCamZ_fourdim(float amount) {
	cam_fourdim.z += amount;
}

void special_fourdim(int key, int x, int y) {
	const float CHANGE_VIEW_ANGLE = 8.0;
	const float MOVE_DISTANCE = 8.0;
	count_time_fourdim++;
	switch (key) {
	case GLUT_KEY_LEFT:

		cam_fourdim.angle -= CHANGE_VIEW_ANGLE * TO_RAD_fourdim;

		break;
	case GLUT_KEY_RIGHT:

		cam_fourdim.angle += CHANGE_VIEW_ANGLE * TO_RAD_fourdim;

		break;
	case GLUT_KEY_UP:

		changeCamX_fourdim(MOVE_DISTANCE * cos(cam_fourdim.angle));
		changeCamZ_fourdim(MOVE_DISTANCE * sin(cam_fourdim.angle));

		break;
	case GLUT_KEY_DOWN:

		changeCamX_fourdim(-MOVE_DISTANCE * cos(cam_fourdim.angle));
		changeCamZ_fourdim(-MOVE_DISTANCE * sin(cam_fourdim.angle));

		break;

	}
	glutPostRedisplay();
}

void count_time_() {
	count_time_fourdim++;
}

void timer_fourdim(int value) {
	count_time_();
	glutPostRedisplay();
	glutTimerFunc(25, timer_fourdim, 0);
}


void four_dimential_snake() {

	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(1500, 850);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("SNAKE_FOUR_DIMENTIAL");
	initialize_fourdim();
	glutSpecialFunc(special_fourdim);
	glutTimerFunc(25, timer_fourdim, 0);
	glutDisplayFunc(display_fourdim);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();
	
}
