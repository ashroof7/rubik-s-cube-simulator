//============================================================================
// Name        : rubics.cpp
// Author      : Ashraf Saleh
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "utils.h"
#include "cube.h"
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

GLfloat screen_width = 600;
GLfloat screen_height = 600;

// Basic colors
const vec4 WHITE = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 RED = vec4(1.0f, 0.0f, 0.0f, 1.0f);
const vec4 GREEN = vec4(0.0f, 1.0f, 0.0f, 1.0f);
const vec4 BLUE = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 YELLOW = vec4(1.0f, 1.0f, 0.0f, 1.0f);

mat4 I_mat = mat4(1.0); // Identity matrix
mat4 M_mat = mat4(1.0); // Model matrix
mat4 V_mat = mat4(1.0); // View matrix
mat4 S_mat = mat4(1.0); // Shape matrix (rotation of cube by mouse)
mat4 P_mat = mat4(1.0); // projection matrix

GLuint P_loc, M_loc, V_loc, S_loc; // locations pointers of P, M, V, S matrices in shader

GLuint program;

GLuint vao, attribute_v_color; // vao

cube *c;
cube *rubik[3][3][3];

vec3 rot[3][3][3];// amount of rotation in each direction
vec3 temp[3][3]; // buffer the face that is going to rotate
int cube_map[3][3][3][3]; //[i][j][k][i,j,k] contains the indices of cube[i][j][k]
//                      in the currently displayed rubik
int temp_map[3][3][3];

int rubic[3][3][3];// contains the cube index [0..26]
vec3 angle[27];

vec3 eye(5,5,-10);

//mouse state variables
int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;

void init_buffers(){
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//	for (int i = 0; i < 3; ++i) {
	//		for (int j = 0; j < 3; ++j) {
	//			for (int k = 0; k < 3; ++k) {
	//				rubik[i][i][k] = new cube(program, i/2.0, j/2.0, k/2.0 , i*3*3+j*3+k);
	//			}
	//		}
	//	}
	c = new cube(program, 0 );

	// Initializing rotation matrix
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; ++k){
				rot[i][j][k]=vec3(0.0f,0.0f,0.0f);
				//				cube_map[i][j][k][0] = i;
				//				cube_map[i][j][k][1] = j;
				//				cube_map[i][j][k][2] = k;
				rubic[i][j][k] = i*9 + j*3 + k;
				angle[rubic[i][j][k]] = vec3(0.0f,0.0f,0.0f);
			}


}


void init() {
	// Load shaders and use the resulting shader program
	program = init_shaders("vshader.glsl", "fshader.glsl");
	glUseProgram(program);



	init_buffers();

	//	M = translate(vec3(0,0,10));
	//		M = Scale(vec3(.5,.5,.5));

	V_mat = lookAt(eye, vec3(0,0,0), vec3(0,1,0));
	//	P = Ortho(-2,2,-2,2,0,10);
	P_mat = perspective(45.0f, 1.0f*screen_width/screen_height, 1.0f, 100.0f);


	// matrices location in shader
	P_loc = glGetUniformLocation(program, "P");
	V_loc = glGetUniformLocation(program, "V");
	M_loc = glGetUniformLocation(program, "M");
	S_loc = glGetUniformLocation(program, "S");

	glUniformMatrix4fv(P_loc, 1, false, value_ptr(P_mat));
	glUniformMatrix4fv(V_loc, 1, false, value_ptr(V_mat));
	glUniformMatrix4fv(M_loc, 1, false, value_ptr(M_mat));
	glUniformMatrix4fv(S_loc, 1, false, value_ptr(S_mat));

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

}



void rotate_face(int axis, int col, int direction){
	// axis 0 = x	1 = y	z = 2
	// col indicates the col number to be rotated {0, 2} // cant take value 1
	// direction denotes the angle 1 up 	 0 = down

	cout<<"rotate face "<<axis<<" "<<col<<" "<<direction<<endl;

	int i=0, j=0; // loop indices
	int *a,*b,*c; // rotation matrix indices

	switch (axis) {
	case 0:	//x axis
		a = &i;
		b = &col;
		c = &j;
		break;
	case 1: //y axis
		a = &col;
		b = &i;
		c = &j;
		break;
	case 2: //z axis
		a = &i;
		b = &j;
		c = &col;
		break;
	default : return;
	}

	cout <<">>>"<<*b << " "<<*a<<" "<<*c<<endl;

	int face[3][3]; // current face to rotate
	// cubes to rotate
	for ( i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j){
			face[i][j] = rubic[*a][*b][*c];
			//			cout<<face[i][j]<<" "<<endl;
		}
	//	cout<<endl;

	for (j = 2; j >= 0; --j){
		for ( i = 2; i >=0; --i){
			cout<<rubic[*a][*b][*c]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	if (direction){
		// direction up
		for ( i = 0; i < 3; ++i)
			for ( j = 0; j < 3; ++j){
				//				cout<<"Replace "<<rubic[*a][*b][*c]<<" with "<<face[j][2-i]<<endl;
				angle[rubic[*a][*b][*c]][axis] +=(angle[rubic[*a][*b][*c]][axis]>270?-270.0f : 90.0f);
				rubic[*a][*b][*c] = face[j][2-i];
				//				swap(face[i][j], face[2-j][i]);
			}

		//rot[i][col][j][0] += (rot[i][col][j][0]>350?-350.0f : 10.0f);
		//rot[i][col][j][0] += (rot[i][col][j][0]>270?-270.0f : 90.0f);

	} else {
		// direction down
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j){
				//				cout<<"Replace "<<rubic[*a][*b][*c]<<" with "<<face[j][2-i]<<endl;
				angle[rubic[*a][*b][*c]][axis] -=(angle[rubic[*a][*b][*c]][axis]<90?-270.0f : 90.0f);
				rubic[*a][*b][*c] = face[2-j][i];
				//				swap(face[i][j], face[2-j][i]);
			}
	}

	for (j = 2; j >= 0; --j){
		for ( i = 2; i >=0; --i){
			cout<<rubic[*a][*b][*c]<<" ";
		}
		cout<<endl;
	}
}


void display(){

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear the window//
	//	glClear(GL_COLOR_BUFFER_BIT); // clear the window//

	glEnableVertexAttribArray(vao);
	glBindVertexArray(vao);

	// test on 1 cube
	//	M_mat = translate(0*1.2f, 0*1.2f , 0*1.2f);
	//	M_mat = rotate(M_mat, -90.0f, vec3(1,0,0));
	//	glUniformMatrix4fv(M_loc, 1, false, value_ptr(M_mat));
	//	glDrawArrays(GL_TRIANGLES, 0,36);


	for (int j = 0; j < 3; ++j){
		for (int i = 0; i < 3; ++i){
			for (int k = 0; k < 3; ++k){
				// FIXME IN one CALL
				mat4 rx  = rotate(angle[rubic[i][j][k]].x, vec3(1,0,0));
				mat4 ry  = rotate(angle[rubic[i][j][k]].y, vec3(0,1,0));
				mat4 rz  = rotate(angle[rubic[i][j][k]].z, vec3(0,0,1));
				mat4 t   = translate((j-1)*1.2f, (i-1)*1.2f, (k-1)*1.2f);
				M_mat = t*rz*ry*rx;


				glUniformMatrix4fv(M_loc, 1, false, value_ptr(M_mat));
				glDrawArrays(GL_TRIANGLES, 0,36);
			}
		}
	}


	/* Push each element in buffer_vertices to the vertex shader */
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	//	int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	//	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(vao);
	glutSwapBuffers();
}


void keyboard_special(int key, int x, int y) {

	switch (key) {

	}
}


void free_buffers(){
	glDeleteProgram(program);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033:
		cout << "exit" << endl;
		exit(EXIT_SUCCESS);
		break;


		// rotate around x
	case 'q':
		rotate_face(0,2,1);
		glutPostRedisplay();
		break;
	case 'w':
		rotate_face(0,1,1);
		glutPostRedisplay();
		break;
	case 'e':
		rotate_face(0,0,1);
		glutPostRedisplay();
		break;
	case 'a':
		rotate_face(0,2,0);
		glutPostRedisplay();
		break;
	case 's':
		rotate_face(0,1,0);
		glutPostRedisplay();
		break;
	case 'd':
		rotate_face(0,0,0);
		glutPostRedisplay();
		break;


		// rotate around y
	case 'r':
		rotate_face(1,2,1);
		glutPostRedisplay();
		break;
	case 't':
		rotate_face(1,1,1);
		glutPostRedisplay();
		break;
	case 'y':
		rotate_face(1,0,1);
		glutPostRedisplay();
		break;
	case 'f':
		rotate_face(1,2,0);
		glutPostRedisplay();
		break;
	case 'g':
		rotate_face(1,1,0);
		glutPostRedisplay();
		break;
	case 'h':
		rotate_face(1,0,0);
		glutPostRedisplay();
		break;


		// rotate around z
	case 'u':
		rotate_face(2,2,1);
		glutPostRedisplay();
		break;
	case 'i':
		rotate_face(2,1,1);
		glutPostRedisplay();
		break;
	case 'o':
		rotate_face(2,0,1);
		glutPostRedisplay();
		break;
	case 'j':
		rotate_face(2,2,0);
		glutPostRedisplay();
		break;
	case 'k':
		rotate_face(2,1,0);
		glutPostRedisplay();
		break;
	case 'l':
		rotate_face(2,0,0);
		glutPostRedisplay();
		break;


	}




}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}



/**
 * copied from OpenGL wiki
 * http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
 *
 * Get a normalized vector from the center of the virtual ball O to a
 * point P on the virtual ball surface, such that P is aligned on
 * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
 * sphere, return the nearest point on the virtual ball surface.
 */
vec3 get_arcball_vector(int x, int y) {
	//	convert the x,y screen coordinates to [-1,1] coordinates (and reverse y coordinates)
	vec3 P = vec3(1.0*x/screen_width*2 - 1.0, 	1.0*y/screen_height*2 - 1.0, 	0);
	P.y = -P.y;

	float OP_squared = P.x * P.x + P.y * P.y;
	// use Pythagorean theorem to get P.z
	if (OP_squared <= 1*1)
		P.z = sqrt(1*1 - OP_squared);  // Pythagore
	else
		P = normalize(P);  // nearest point

	return P;
}


void onIdle(){
	if (cur_mx != last_mx || cur_my != last_my) {
		vec3 va = get_arcball_vector(last_mx, last_my);
		vec3 vb = get_arcball_vector( cur_mx,  cur_my);

		// dot(va,vb) = ||va||.||vb||.cos(angle)
		float angle = acos(  glm::min(1.0f, 1.0f*dot(va, vb)))*3;

		vec3 axis_in_camera_coord = cross(va, vb);
		mat3 camera2object = inverse(mat3(V_mat * S_mat));
		vec3 axis_in_obj_coord =  camera2object * axis_in_camera_coord;
		S_mat = rotate(S_mat, degrees(angle), axis_in_obj_coord);

		last_mx = cur_mx;
		last_my = cur_my;

		glUniformMatrix4fv(S_loc, 1, false, value_ptr(S_mat));
		glutPostRedisplay();
	}
}

void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		arcball_on = true;
		last_mx = cur_mx = x;
		last_my = cur_my = y;
	} else {
		arcball_on = false;
	}
}

void onMotion(int x, int y) {
	if (arcball_on) {  // if left button is pressed
		cur_mx = x;
		cur_my = y;
	}
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(screen_width, screen_height);

	//	----------------------------------------
	//	 If you are using freeglut, the next two lines will check if
	//	 the code is truly 3.2. Otherwise, comment them out
	//	glutInitContextVersion( 3, 2 );
	//	glutInitContextProfile( GLUT_CORE_PROFILE );
	//	----------------------------------------
	glutCreateWindow("Ashraf's Cube");


	glewInit();
	init();

	glutIdleFunc(onIdle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutReshapeFunc(onReshape);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	//	glutPassiveMotionFunc(onPassiveMotion);

	glutMainLoop();
	return 0;
}


