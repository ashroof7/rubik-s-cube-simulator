/*
 * cube.h
 *
 *  Created on: Nov 25, 2013
 *      Author: Ashraf Saleh
 */

#ifndef CUBE_H_
#define CUBE_H_

#include <GL/glew.h>
#include <GL/glut.h>


using namespace std;

extern const GLfloat vertices[];
extern const GLfloat colors[];
extern const GLushort elements[];


class cube {

public:
	GLuint pos_vbo, col_vbo;
	cube();
	cube(GLuint program);
	virtual ~cube();
	void bind_position();
	void bind_color();
};

#endif /* CUBE_H_ */
