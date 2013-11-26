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



class cube {

private:
	static GLfloat vertices[];
	static GLfloat colors[];
	static GLushort elements[];

public:
	GLuint pos_vbo, col_vbo;
	cube();
	cube(GLuint program, int offset_index);
	virtual ~cube();
	void bind_position();
	void bind_color();
};

#endif /* CUBE_H_ */
