/*
 * cube.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: leo
 */

#include "cube.h"
#include <iostream>
using namespace std;

const GLfloat vertices[] = { -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f,

		-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,

};

const GLfloat colors[] = { 0.0, 1.0, 0.0,  //blue
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

const GLushort elements[] = {
// front
		0, 1, 2, 2, 3, 0,
		// top
		3, 2, 6, 6, 7, 3,
		// back
		7, 6, 5, 5, 4, 7,
		// bottom
		4, 5, 1, 1, 0, 4,
		// left
		4, 0, 3, 3, 7, 4,
		// right
		1, 5, 6, 6, 2, 1, };


cube::cube(GLuint program) {
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLint posAttrib = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &col_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLint colAttrib = glGetAttribLocation(program, "in_Color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

cube::cube() {
	col_vbo = pos_vbo = -1;
}

cube::~cube() {
	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &col_vbo);
//	glDeleteBuffers(1, &elements_ibo);
}

void cube::bind_position(){
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
}

void cube::bind_color(){
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);;
}

