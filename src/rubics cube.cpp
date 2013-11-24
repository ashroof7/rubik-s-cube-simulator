//============================================================================
// Name        : rubics.cpp
// Author      : Ashraf Saleh
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "include/Angel.h"


using namespace std;
using namespace Angel;

GLfloat screen_width = 600;
GLfloat screen_height = 600;

// Basic colors
const vec4 WHITE = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 RED = vec4(1.0f, 0.0f, 0.0f, 1.0f);
const vec4 GREEN = vec4(0.0f, 1.0f, 0.0f, 1.0f);
const vec4 BLUE = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 YELLOW = vec4(1.0f, 1.0f, 0.0f, 1.0f);


mat4 I = mat4(1.0); // Identity matrix
mat4 M = mat4(1.0); // Model matrix
mat4 V = mat4(1.0); // View matrix
mat4 P = mat4(1.0); // projection matrix
GLuint P_loc, M_loc, V_loc; // locations pointers of P, M, V matrices in shader

GLuint program;

GLuint vbo_cube_vertices, vbo_cube_colors; // vbo
GLuint ibo_cube_elements; // element buffer
GLuint vao, attribute_v_color; // vao

static const GLfloat cube_vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,

		-1.0f, -1.0f, -0.5f,
		1.0f, -1.0f, -0.5f,
		1.0f,  1.0f, -0.5f,
		1.0f,  1.0f, -0.5f,
		-1.0f,  1.0f, -0.5f,
		-1.0f, -1.0f, -0.5f,
};

static const GLfloat cube_colors[] = {
		0.0, 1.0, 0.0,  //blue
		0.0, 1.0, 0.0,  //blue
		0.0, 1.0, 0.0,  //blue
		0.0, 1.0, 0.0,  //blue
		0.0, 1.0, 0.0,  //blue
		0.0, 1.0, 0.0,  //blue

		1.0, 0.0, 0.0,  //red
		1.0, 0.0, 0.0,  //red
		1.0, 0.0, 0.0,  //red
		1.0, 0.0, 0.0,  //red
		1.0, 0.0, 0.0,  //red
		1.0, 0.0, 0.0,  //red

		1.0, 1.0, 0.0,  //yellow
		1.0, 1.0, 0.0,  //yellow
		1.0, 1.0, 0.0,  //yellow
		1.0, 1.0, 0.0,  //yellow
		1.0, 1.0, 0.0,  //yellow
		1.0, 1.0, 0.0,  //yellow

		0.0, 0.0, 0.0,  //black
		0.0, 0.0, 0.0,  //black
		0.0, 0.0, 0.0,  //black
		0.0, 0.0, 0.0,  //black
		0.0, 0.0, 0.0,  //black
		0.0, 0.0, 0.0,  //black





		0.0, 0.0, 1.0,  //green
		0.0, 0.0, 1.0,  //green
		0.0, 0.0, 1.0,  //green
		0.0, 0.0, 1.0,  //green
		0.0, 0.0, 1.0,  //green
		0.0, 0.0, 1.0,  //green

		1.0, 0.0, 1.0,  //magenta
		1.0, 0.0, 1.0,  //magenta
		1.0, 0.0, 1.0,  //magenta
		1.0, 0.0, 1.0,  //magenta
		1.0, 0.0, 1.0,  //magenta
		1.0, 0.0, 1.0,  //magenta
};

GLushort cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		3, 2, 6,
		6, 7, 3,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// left
		4, 0, 3,
		3, 7, 4,
		// right
		1, 5, 6,
		6, 2, 1,
};

void init_buffers(){

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	GLint posAttrib = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	GLint colAttrib = glGetAttribLocation(program, "in_Color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

}


void init() {
	// Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);


	init_buffers();

	M = Translate(vec3(0,0,3));
	//	M = Scale(vec3(.5,.5,.5));

	V = LookAt(vec3(1,1,-2), vec3(0,0,0), vec3(0,1,0));
//	P = Ortho(-1,1,-1,1,-20,20);
	//	P = Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);


	// matrices location in shader
	P_loc = glGetUniformLocation(program, "P");
	V_loc = glGetUniformLocation(program, "V");
	M_loc = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(P_loc, 1, false, P);
	glUniformMatrix4fv(V_loc, 1, false, V);
	glUniformMatrix4fv(M_loc, 1, false, M);




	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

}


void display(){

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear the window//
	//	glClear(GL_COLOR_BUFFER_BIT); // clear the window//

	glEnableVertexAttribArray(vao);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0,36);

	/* Push each element in buffer_vertices to the vertex shader */
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	//	int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	//	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);


	glDisableVertexAttribArray(vao);
	glutSwapBuffers();
}

void free_buffers(){
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_cube_vertices);
	glDeleteBuffers(1, &vbo_cube_colors);
	glDeleteBuffers(1, &ibo_cube_elements);
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(screen_width, screen_height);
	//	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(onReshape);
	//	----------------------------------------
	//	 If you are using freeglut, the next two lines will check if
	//	 the code is truly 3.2. Otherwise, comment them out
	//	glutInitContextVersion( 3, 2 );
	//	glutInitContextProfile( GLUT_CORE_PROFILE );
	//	----------------------------------------

	glutCreateWindow("Ashraf's Cube");

	glewInit();

	init();

	glutDisplayFunc(display);
	//	glutKeyboardFunc(keyboard);
	//	glutSpecialFunc(keyboard_special);
	//	glutMouseFunc(onMouse);
	//	glutPassiveMotionFunc(onPassiveMotion);
	//	glutReshapeFunc(onReshape);
	glutMainLoop();
	return 0;
}
