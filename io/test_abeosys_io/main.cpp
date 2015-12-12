#include "../abeosys_io.h"
#include <GLFW/glfw3.h>
#include <Windows.h>
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#define dir_path0 L"G:\\ProjectsData\\MapData\\35913044\\*"
#define shp_path0 "G:\\ProjectsData\\MapData\\35913044\\b0010000_polygon.shp"
#define dbf_path0 "G:\\ProjectsData\\MapData\\35913044\\b0010000_polygon.dbf"
#define shp_path1 "G:\\ProjectsData\\MapData\\35913044\\f0010000_line.shp"
#define dbf_path1 "G:\\ProjectsData\\MapData\\35913044\\f0010000_line.dbf"


float zf = 1;
float xo = 0, yo = 0;
float rx = 75, ry =0, rz = 45;
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_I)//zoom in
		zf = zf*1.1;
	else if (key == GLFW_KEY_O)//zoom out
		zf = zf*0.9;
	else if (key == GLFW_KEY_LEFT)//zoom out
		xo += 10;
	else if (key == GLFW_KEY_RIGHT)//zoom out
		xo -= 10;
	else if (key == GLFW_KEY_UP)//zoom out
		yo -= 10;
	else if (key == GLFW_KEY_DOWN)//zoom out
		yo += 10;
	else if (key == GLFW_KEY_X)//zoom out
		rx += 5;
	else if (key == GLFW_KEY_Y)//zoom out
		ry += 5;
	else if (key == GLFW_KEY_Z)//zoom out
		rz += 5;

	cout << "rx=" << rx << ", ry=" << ry << ", rz=" << rz << endl;
}


int main(){
	WIN32_FIND_DATA search_data;

	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));

	HANDLE handle = FindFirstFile(dir_path0, &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{

		wcout << "\n" << search_data.cFileName;

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	//Close the handle after use or memory/resource leak
	FindClose(handle);

	shp_file shp[2];
	dbf_file dbf[2];
	cout << "Start loading...";
#define _load(k) shp[k].load(shp_path##k);dbf[k].load(dbf_path##k);
	_load(0);
	_load(1);
	wcout << dbf[1].get_string(0, 2).c_str()<<"\n";
	//cout << "DONE" << endl;
	//	

	//cout << "Start writing...";
	//fstream sw;
	//sw.open("G:\\data.txt",ios::out);
	//sw << "# Format" << endl;
	//sw << "# First line: xmin, xmax, ymin, ymax" << endl;
	//sw << "# Data line: shape type;number of points;height data;bound0,bound1,bound2,bound3;x0,y0,x1,y1...." << endl;
	////write minmax info
	//sw  << shp[0].header().xmin << ','
	//	<< shp[0].header().xmax << ','
	//	<< shp[0].header().ymin << ','
	//	<< shp[0].header().ymax << endl;
	//for (int i = 0; i < shp[0].record_count(); i++){
	//	sw << shp[0].record(i)->shape_type << ';';							// write shape type
	//	shp::polygon* polygon = (shp::polygon*)shp[0].record(i)->entity;	//	
	//	sw << polygon->point_count << ';'									// point count
	//		<< dbf[0].get_double(i, 5) << ';';								// height=story count*3m
	//	sw << polygon->bound[0] << ','										// bound
	//		<< polygon->bound[1] << ','
	//		<< polygon->bound[2] << ','
	//		<< polygon->bound[3] << ';';
	//	for (int k = 0; k < polygon->point_count; k++){
	//		shp::point& p = polygon->points[k];								// point data
	//		sw << p.x << ',' << p.y << ',';
	//	}
	//	sw << endl;
	//}
	//sw.close();
	//cout << "DONE" << endl;
	
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1280, 960, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	float ww = shp[0].header().xmax - shp[0].header().xmin;
	float hh = shp[0].header().ymax - shp[0].header().ymin;
	float xoff = shp[0].header().xmin;
	float yoff = shp[0].header().ymin;
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
	
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);


		static const GLfloat lightPos[4] = { 500, 500, 0, 1.0f };		
		GLfloat mShininess[] = { 1 };	
		GLfloat whiteLight[] = { 0.5, 0.5, 0.5 };

		glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);

		GLfloat materialColor[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat materialSpecular[] = { 0.3, 0.3, 0.3, 1.0 };
		GLfloat materialEmission[] = { 0.02, 0.02, 0.02, 1.0 };

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 5);
		

		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);

		glEnable(GL_NORMALIZE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-width/2,width/2, height/2, -height/2, -100000.f, 100000.f);
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		

		//draw mouse cross hair
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		glDisable(GL_LIGHTING);
		glTranslatef(-width / 2, -height / 2, 0);
		glColor3f(0, 1, 0);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		glBegin(GL_LINES);
		glVertex3f(xpos,0,0);
		glVertex3f(xpos, height,0);
		glVertex3d(0, ypos,0);
		glVertex3d(width, ypos,0);
		glEnd();


		

		glLoadIdentity();
		glRotatef(rx,1,0,0);
		glRotatef(ry, 0, 1, 0);
		glRotatef(glfwGetTime()*0.5+rz, 0, 0, 1);
		

		glScalef(zf,zf,zf);
		glTranslatef(-xoff- ww / 2+xo, -yoff - hh / 2+yo, 0);

		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glEnable(GL_LIGHTING);
		//glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(0.5, 0, 0);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
			glVertex3f(shp[0].header().xmin, shp[0].header().ymin, 0);
			glVertex3f(shp[0].header().xmin, shp[0].header().ymax, 0);
			glVertex3f(shp[0].header().xmax, shp[0].header().ymax, 0);
			glVertex3f(shp[0].header().xmax, shp[0].header().ymin, 0);	
			
		glEnd();

		//glPolygonMode(GL_FRONT, GL_FILL);
		//glPolygonMode(GL_BACK, GL_FILL);
		for (int i = 0; i < shp[0].record_count(); i++){
			shp::polygon* polygon = (shp::polygon*)shp[0].record(i)->entity;	
			float z = dbf[0].get_double(i, 5)*3;
			glColor3f(0.5, 0.5,0.5);
			glBegin(GL_QUADS);
			for (int k = 0; k < polygon->point_count-1; k++){
				float x0, x1, y0, y1;
				x0 = polygon->points[k].x; x1 = polygon->points[k+1].x;
				y0 = polygon->points[k].y; y1 = polygon->points[k + 1].y;
				glNormal3f((y1 - y0)*z, -(x1 - x0)*z, 0);
				glVertex3f(x0,y0,0);
				glVertex3f(x1,y1,0);
				glVertex3f(x1, y1, z);
				glVertex3f(x0, y0, z);

			}
			glEnd();
		}

		for (int i = 0; i < shp[1].record_count(); i++){
			shp::polygon* polygon = (shp::polygon*)shp[1].record(i)->entity;
			float z = dbf[1].get_double(i, 1) ;
			glColor3f(0, 0.5, 0);
			glBegin(GL_QUADS);
			for (int k = 0; k < polygon->point_count - 1; k++){
				float x0, x1, y0, y1;
				x0 = polygon->points[k].x; x1 = polygon->points[k + 1].x;
				y0 = polygon->points[k].y; y1 = polygon->points[k + 1].y;
				glNormal3f((y1 - y0)*z, -(x1 - x0)*z, 0);
				glVertex3f(x0, y0, z-5);
				glVertex3f(x1, y1, z-5);
				glVertex3f(x1, y1, z);
				glVertex3f(x0, y0, z);

			}
			glEnd();
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Press any key to exit" << endl;
	getchar();
	return 0;
}