#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include "mesh-parser.h"

void menu(int value);
int menuDraw;

enum TRANSFORM_STATE{
	ROTATE,
	TRANSLATE,
	SCALE,
};

enum DRAW_STATE{
	SMOOTH,
	FLAT
};

TRANSFORM_STATE tranState = ROTATE;
DRAW_STATE drawState = FLAT;

int window;

float windowX = 500.0;
float windowY = 500.0;

bool fullscreen = false;
bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

float xT = 0.0;
float yT = 0.0; 

float xTdiff = 0.0;
float yTdiff = 0.0;

float scale = 1.0;
float scaleDiff = 0.0;

float xCenter;
float yCenter;
float zCenter;

float lightPosition[] = {50.0f, 30.0f, 100.0f};

GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat yellow[]  = {1.0, 1.0, 0.0, 1.0};

bool light0 = true;
bool light1 = false;

bool drawSMD = false;

float light1Position[] = {-20.0f, -10.0f, 50.0f};


SurFaceMesh* objectMesh;
std::vector<TRIANGLE*> SMDmesh;

int count = 0;

void calculateNormal(FLTVECT a, FLTVECT b, FLTVECT c, float* normal){
	
	float U[] = {b.x - a.x, b.y - a.y, b.z - a.z};
	float V[] = {c.x - a.x, c.y - a.y, c.z - a.z};
//0 = x, 1 = y, 2 = z
	normal[0] = (U[1] * V[2]) - (U[2] * V[1]);
	normal[1] = (U[2] * V[0]) - (U[0] * V[2]);
	normal[2] = (U[0] * V[1]) - (U[1] * V[0]);

	float norm = sqrt((normal[0]*normal[0]) + (normal[1]*normal[1]) + (normal[2]*normal[2]));

	normal[0] = normal[0]/norm;
	normal[1] = normal[1]/norm;
	normal[2] = normal[2]/norm;

}

void lightingFunction(){

	GLfloat light_diffuse[]={0.8, 0.8, 0.8, 1.0};
	GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat light_model_ambient[]={0.2, 0.2, 0.2, 1.0};

	GLfloat light1_diffuse[]={0.1, 0.2, 0.2, 1.0};
	GLfloat light1_specular[]={0.5, 0.5, 0.5, 1.0};
	GLfloat light1_model_ambient[]={0.6, 0.2, 0.8, 1.0};


	if(light0 == true){
   		glEnable(GL_LIGHT0);
   		glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
		glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	}
	else{
		glDisable(GL_LIGHT0);
	}

	if(light1 == true){
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE,light1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR,light1_specular);
		glLightfv(GL_LIGHT1, GL_AMBIENT, light1_model_ambient);
		glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
	}
	else{
		glDisable(GL_LIGHT1);
	}
}


bool init(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
	//glDepthRange(0.0,1.0);
	glDepthMask(GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glMatrixMode(GL_MODELVIEW);

	objectMesh = readMeshFile();
	SMDmesh = read_SMD_mesh("power_armor_reference.smd");

	xCenter = (objectMesh->max_x + objectMesh->min_x)/2.0;
	yCenter = (objectMesh->max_y + objectMesh->min_y)/2.0;
	zCenter = (objectMesh->max_z + objectMesh->min_z)/2.0;

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0f, 0.0f, 150.0f, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);

	lightingFunction();

   	glShadeModel(GL_FLAT);
   	glEnable(GL_CULL_FACE);

	return true;
}

void transformFunc(){

	glTranslatef(xCenter + (xT/2.0), yCenter + (yT/2.0), zCenter);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glScalef(scale, scale, scale);
	glTranslatef(-xCenter - (xT/2.0), -yCenter - (yT/2.0), -zCenter);

	//for some reason, is seems to translate ~2x the change in mouse position; divide in half to make it more accurate
	glTranslatef(xT/2.0, yT/2.0, 0);

}

void createMenu(){
	menuDraw = glutCreateMenu(menu);

	glutAddMenuEntry("Smooth Shading", 1);
	glutAddMenuEntry("Flat Shading", 2);
	glutAddMenuEntry("Light 1", 3);
	glutAddMenuEntry("Light 2", 4);
	glutAddMenuEntry("Draw T-45", 5);
	glutAddMenuEntry("EXIT", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int value){
	switch(value){
		case 1:
			drawSMD = false;
			drawState = SMOOTH;
			glShadeModel(GL_SMOOTH);
		break;

		case 2:
			drawSMD = false;
			drawState = FLAT;
			glShadeModel(GL_FLAT);
		break;

		case 3:
			light0 = light0 ^ true;
		break;

		case 4:
			light1 = light1 ^ true;
		break;

		case 5:
			drawSMD = true;
			glShadeModel(GL_SMOOTH);
			//glFrontFace(GL_CCW);
		break;

		case 0:
			glutDestroyWindow(window);
			for(std::vector<TRIANGLE*>::iterator it = SMDmesh.begin(); it != SMDmesh.end(); ++it){
				free((*it)->a);
				free((*it)->b);
				free((*it)->c);
				free((*it));
			}
			exit(0);
		break;
	}

	glutPostRedisplay();
}

void drawNormal(FLTVECT a, FLTVECT b, FLTVECT c, float normalx, float normaly, float normalz){

	float centerX = (a.x + b.x + c.x)/3;
	float centerY = (a.y + b.y + c.y)/3;
	float centerZ = (a.z + b.z + c.z)/3;

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(centerX, centerY, centerZ);
	glVertex3f(centerX + (normalx*2), centerY + (normaly*2), centerZ + (normalz*2));
	glEnd();

}

void drawFunction(){
	glPushMatrix();
	transformFunc();

	float mat_diffuse[]={0.0,0.5f,0.7f,0.0f};
	float mat_ambient[]={0.0,0.1f,0.0f,0.0f};

	if(drawState == FLAT){

		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

		for(int i = 0; i < objectMesh->nf; ++i){

			int a = objectMesh->face[i].a;
			int b = objectMesh->face[i].b;
			int c = objectMesh->face[i].c;

			glNormal3f(objectMesh->face[i].normal_x, objectMesh->face[i].normal_y, objectMesh->face[i].normal_z);
			glBegin(GL_TRIANGLES);
			
			glVertex3f(objectMesh->vertex[a].x, objectMesh->vertex[a].y, objectMesh->vertex[a].z);
			glVertex3f(objectMesh->vertex[b].x, objectMesh->vertex[b].y, objectMesh->vertex[b].z);
			glVertex3f(objectMesh->vertex[c].x, objectMesh->vertex[c].y, objectMesh->vertex[c].z);
			glEnd();

			//drawNormal(objectMesh->vertex[a], objectMesh->vertex[b], objectMesh->vertex[c], 
			//	objectMesh->face[i].normal_x, objectMesh->face[i].normal_y, objectMesh->face[i].normal_z);

		}
	}

	if(drawState == SMOOTH){
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

		for(int i = 0; i < objectMesh->nf; ++i){

			int a = objectMesh->face[i].a;
			int b = objectMesh->face[i].b;
			int c = objectMesh->face[i].c;

			
			glBegin(GL_TRIANGLES);
			glNormal3f(objectMesh->vertex[a].normal_x, objectMesh->vertex[a].normal_y, objectMesh->vertex[a].normal_z);
			glVertex3f(objectMesh->vertex[a].x, objectMesh->vertex[a].y, objectMesh->vertex[a].z);
			glNormal3f(objectMesh->vertex[b].normal_x, objectMesh->vertex[b].normal_y, objectMesh->vertex[b].normal_z);
			glVertex3f(objectMesh->vertex[b].x, objectMesh->vertex[b].y, objectMesh->vertex[b].z);
			glNormal3f(objectMesh->vertex[c].normal_x, objectMesh->vertex[c].normal_y, objectMesh->vertex[c].normal_z);
			glVertex3f(objectMesh->vertex[c].x, objectMesh->vertex[c].y, objectMesh->vertex[c].z);
			glEnd();

		}
	}	

	glPopMatrix();
}

void drawModelSMD(){

	glPushMatrix();
	transformFunc();

	float mat_diffuse[]={0.0,0.5f,0.7f,0.0f};
	float mat_ambient[]={0.0,0.1f,0.0f,0.0f};

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

	for(std::vector<TRIANGLE*>::iterator it = SMDmesh.begin(); it != SMDmesh.end(); ++it){
		glBegin(GL_TRIANGLES);

		glNormal3f((*it)->a->normal_x, (*it)->a->normal_y, (*it)->a->normal_z);
		glVertex3f((*it)->a->x, (*it)->a->y, (*it)->a->z);
		//printf("%f %f %f, ", (*it)->a->y, (*it)->a->y, (*it)->a->z);

		glNormal3f((*it)->b->normal_x, (*it)->b->normal_y, (*it)->b->normal_z);
		glVertex3f((*it)->b->x, (*it)->b->y, (*it)->b->z);
		//printf("%f %f %f, ", (*it)->b->y, (*it)->b->y, (*it)->b->z);

		glNormal3f((*it)->c->normal_x, (*it)->c->normal_y, (*it)->c->normal_z);
		glVertex3f((*it)->c->x, (*it)->c->y, (*it)->c->z);
		//printf("%f %f %f\n", (*it)->c->y, (*it)->c->y, (*it)->c->z);

		glEnd();
	}

	glPopMatrix();

}

void display(){

	//glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMaterialfv(GL_FRONT, GL_EMISSION, white); 
	glPushMatrix();
    glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);
    glTranslatef(5,1,0);
    glutSolidSphere(1, 10, 8);
   	glPopMatrix();
   	
   	lightingFunction();

   	glMaterialfv(GL_FRONT, GL_EMISSION, black); 

   	if(drawSMD)
   		drawModelSMD();
   	
   	else
		drawFunction();

	glFlush();
	glutSwapBuffers();

}

void mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseDown = true;

		xTdiff = x - xT;
		yTdiff = -(windowY-y) + yT;

		scaleDiff = x - y;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	
	else
		mouseDown = false;
}

void mouseMotion(int x, int y){
	
	if (mouseDown){

		switch(tranState){

			case ROTATE:
				yrot = x - xdiff;
				xrot = y + ydiff;
			break;

			case TRANSLATE:
				xT = x - xTdiff;
				yT = (windowY-y) + yTdiff;
			break;

			case SCALE:
				scale = (x - y) / scaleDiff;
			break;
		}

		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 27 : 
			exit(1); break;
		case 'r':
			tranState = ROTATE;
			break;

		case 't':
			tranState = TRANSLATE;
			//xT = 0.0;
			//yT = 0.0;

			xTdiff = 0.0;
			yTdiff = 0.0;
			break;
		case 's':
			tranState = SCALE;

			break;
	}
}

void specialKeyboard(int key, int x, int y){
	if (key == GLUT_KEY_F1){
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		
		else{
			glutReshapeWindow(windowX, windowY);
			glutPositionWindow(50, 50);
		}
	}
}

void reshape(int w, int h)
{
   glViewport(0, 0,w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, windowX / windowY, 1.0, 175.0);

   glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(windowX, windowY);

	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);

	window = glutCreateWindow("Lighting");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(reshape); 

	createMenu();

	if (!init())
	return 1;

	glutMainLoop();

	return 0;
}
