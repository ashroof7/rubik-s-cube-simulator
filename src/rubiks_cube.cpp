//============================================================================
// Name        : rubics.cpp
// Author      : Ashraf Saleh
// Version     :
// Copyright   :
// Description : rubik's cube simulator in C++, Ansi-style
//============================================================================


#include "utils.h"
#include "cube.h"

#include <time.h>
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

mat4 I_mat = mat4(1.0); // Identity matrix
mat4 M_mat = mat4(1.0); // Model matrix
mat4 V_mat = mat4(1.0); // View matrix
mat4 W_mat = mat4(1.0); // World matrix (rotation of cube by mouse)
mat4 P_mat = mat4(1.0); // projection matrix

GLuint P_loc, M_loc, V_loc, W_loc; // locations pointers of P, M, V, W matrices in shader

GLuint program;
GLuint vao; // vao


cube *c;
int rubic[3][3][3];// contains the cube index [0..26]
vec3 relative_angle[27]; // angle of rotation of cube around face center
mat4 self_rot[27]; // self rotation matrix for each cube

vec3 eye(5,5,-10);

//mouse state variables
int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;

// animation related variables
GLfloat anim_angle = 5; // animation angle starts with 0 and ends with 90
int anim_angle_cnt = 0; // increment value of the animation angle

int cur_col = 0, cur_axis = 0, cur_direction = 0;
bool in_animation = false;


void rotate_face(int axis, int col, int direction);

bool win(){
		for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					for (int k = 0; k < 3; ++k)
						if(rubic[i][j][k] != i*9 + j*3 + k)
							return false;
	return true;
}

void shuffle(){
	in_animation = false;
	rotate_face(rand()%3,rand()%3,rand()&1);
	glutPostRedisplay();
}



void init_buffers(){
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	c = new cube(program, 0 );

	// Initializing rotation matrix
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; ++k){
				rubic[i][j][k] = i*9 + j*3 + k;
				self_rot[rubic[i][j][k]] = mat4(1.0);
				relative_angle[rubic[i][j][k]] = vec3(0.0f,0.0f,0.0f);
			}

}


void init() {
	// Load shaders and use the resulting shader program
	program = init_shaders("vshader.glsl", "fshader.glsl");
	glUseProgram(program);



	init_buffers();

	V_mat = lookAt(eye, vec3(0,0,0), vec3(0,1,0));
	P_mat = perspective(45.0f, 1.0f*screen_width/screen_height, 1.0f, 100.0f);

	// matrices location in shader
	P_loc = glGetUniformLocation(program, "P");
	V_loc = glGetUniformLocation(program, "V");
	M_loc = glGetUniformLocation(program, "M");
	W_loc = glGetUniformLocation(program, "W");

	glUniformMatrix4fv(P_loc, 1, false, value_ptr(P_mat));
	glUniformMatrix4fv(V_loc, 1, false, value_ptr(V_mat));
	glUniformMatrix4fv(M_loc, 1, false, value_ptr(M_mat));
	glUniformMatrix4fv(W_loc, 1, false, value_ptr(W_mat));

	glClearColor(0.9, 0.9, 0.9, 1.0); // white background

	glEnable(GL_DEPTH_TEST);// Enable depth test
	glDepthFunc(GL_LESS);// Accept fragment if it closer to the camera than the former one


}



void animation(int i){
	cout<<"a7ee a7ee"<<endl;
	rotate_face(cur_axis, cur_col, cur_direction);
	glutPostRedisplay();
	if (i > 0)
		glutTimerFunc(10, animation, i-1);
	else
		in_animation = false;
}


bool first_animation = true;

void rotate_face(int axis, int col, int direction){
	in_animation = true;

	cur_col = col;
	cur_axis = axis;
	cur_direction = direction;
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

	//	cout <<">>>"<<*b << " "<<*a<<" "<<*c<<endl;

	int face[3][3]; // current face to rotate
	// cubes to rotate
	for ( i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j){
			face[i][j] = rubic[*a][*b][*c];
		}

		for (j = 2; j >= 0; --j){
			for ( i = 2; i >=0; --i){
//				cout<<rubic[*a][*b][*c]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;


	GLfloat thres = 360-anim_angle;
	bool last_animation = false;
	anim_angle_cnt++;

	if (direction){
			// direction up
			for ( i = 0; i < 3; ++i)
				for ( j = 0; j < 3; ++j){
					relative_angle[rubic[*a][*b][*c]][axis] +=(relative_angle[rubic[*a][*b][*c]][axis]>thres?-thres : anim_angle);
					// FIXME change condition to check on relative angle
					if ( anim_angle > (90-anim_angle*anim_angle_cnt)){
						last_animation = true;
						cout<<"up"<<endl;
						relative_angle[rubic[*a][*b][*c]][axis] = 0;
							if (axis==0){
								rubic[*a][*b][*c] = face[j][2-i];
								self_rot[rubic[*a][*b][*c]] = rotate(90.0f, vec3(1,0,0))*self_rot[rubic[*a][*b][*c]];
							}else if (axis==1){
														rubic[*a][*b][*c] = face[2-j][i];
														self_rot[rubic[*a][*b][*c]] = rotate(90.0f, vec3(0,1,0))*self_rot[rubic[*a][*b][*c]];

							}else if (axis ==2){
							rubic[*a][*b][*c] = face[2-j][i];
							self_rot[rubic[*a][*b][*c]] = rotate(90.0f, vec3(0,0,1))*self_rot[rubic[*a][*b][*c]];

							}
					}
				}

		} else {
			// direction down
			for (i = 0; i < 3; ++i)
				for (j = 0; j < 3; ++j){
					relative_angle[rubic[*a][*b][*c]][axis] -=(relative_angle[rubic[*a][*b][*c]][axis]<anim_angle?-thres : anim_angle);
					if ( anim_angle > (90-anim_angle*anim_angle_cnt)){
						last_animation = true;
						cout<<"down"<<endl;
						relative_angle[rubic[*a][*b][*c]][axis] = 0;

							if (axis==0){
								rubic[*a][*b][*c] = face[2-j][i];
								self_rot[rubic[*a][*b][*c]] = rotate(-90.0f, vec3(1,0,0))*self_rot[rubic[*a][*b][*c]];
							}else if (axis==1){
								rubic[*a][*b][*c] = face[j][2-i];
								self_rot[rubic[*a][*b][*c]] = rotate(-90.0f, vec3(0,1,0))*self_rot[rubic[*a][*b][*c]];
							}else if (axis ==2){
								rubic[*a][*b][*c] = face[j][2-i];
								self_rot[rubic[*a][*b][*c]] = rotate(-90.0f, vec3(0,0,1))*self_rot[rubic[*a][*b][*c]];
							}
					}
				}
		}

	if (last_animation){
				cout<<"after "<<endl;
				for (j = 2; j >= 0; --j){
					for ( i = 2; i >=0; --i){
//						cout<<rubic[*a][*b][*c]<<" ";
					}
					cout<<endl;
				}

		cout<<"================"<<endl;
		anim_angle_cnt = 0;
		in_animation = false;
		last_animation = false;
		if(win())
			cout<<"###### win #####"<<endl;
	}

	if (first_animation){
		first_animation = false;
		glutTimerFunc(10, animation, 16);
	}

}




void display(){

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear the window//

	glEnableVertexAttribArray(vao);
	glBindVertexArray(vao);

	for (int j = 0; j < 3; ++j){
		for (int i = 0; i < 3; ++i){
			for (int k = 0; k < 3; ++k){
				// FIXME IN one CALL
				mat4 t = mat4(1.0);

				// translate to center of face
				if (cur_axis == 0)
					t   = translate((j-1)*1.0f, 0.0f, (k-1)*1.0f);
				else if (cur_axis == 1)
					t   = translate(0.0f, (i-1)*1.0f, (k-1)*1.0f);
				else
					t   = translate((j-1)*1.0f, (i-1)*1.0f, 0.0f);


				//t   = translate((j-1)*1.0f, (i-1)*1.0f, (k-1)*1.0f);
				mat4 cube_rotation = self_rot[rubic[i][j][k]];
				M_mat = t*cube_rotation;

				mat4 rx  = rotate(relative_angle[rubic[i][j][k]].x, vec3(1,0,0));
				mat4 ry  = rotate(relative_angle[rubic[i][j][k]].y, vec3(0,1,0));
				mat4 rz  = rotate(relative_angle[rubic[i][j][k]].z, vec3(0,0,1));

				// translate to position in face
				if (cur_axis == 0)
					t   = translate(0.0f, (i-1)*1.0f, 0.0f);
				else if (cur_axis == 1)
					t   = translate((j-1)*1.0f, 0.0f, 0.0f);
				else
					t   = translate(0.0f, 0.0f, (k-1)*1.0f);


				M_mat = rz*ry*rx*t*M_mat;
				glUniformMatrix4fv(M_loc, 1, false, value_ptr(M_mat));
				c->draw();
			}
		}
	}


	glDisableVertexAttribArray(vao);
	glutSwapBuffers();
}


void keyboard_special(int key, int x, int y) {

}


void free_buffers(){
	glDeleteProgram(program);
}

void keyboard(unsigned char key, int x, int y) {
	if (in_animation)
		return;
	first_animation = true;
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

	case '`':
		shuffle();
		break;

	}

}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
	P_mat = perspective(45.0f, 1.0f*screen_width/screen_height, 1.0f, 100.0f);
	glUniformMatrix4fv(P_loc, 1, false, value_ptr(P_mat));

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
		mat3 camera2object = inverse(mat3(V_mat * W_mat));
		vec3 axis_in_obj_coord =  camera2object * axis_in_camera_coord;
		W_mat = rotate(W_mat, degrees(angle), axis_in_obj_coord);

		last_mx = cur_mx;
		last_my = cur_my;

		glUniformMatrix4fv(W_loc, 1, false, value_ptr(W_mat));
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
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
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


