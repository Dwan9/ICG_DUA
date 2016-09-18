///Dua Template Project for Interactive Computer Graphic
///2016/9/18
#include "glsupport.h"
#include <GL/freeglut.h>
#include <GL/glew.h>

#include <stdio.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma region Private Value
GLuint program;
//
GLuint vertexPosition;
GLuint vertexColor;
GLuint vertexTexCoord;
//Attribute data
GLuint positionAttribute;
GLuint colorAttribute;
GLuint textCoordAttribute;
//Texture
GLuint texture1;
GLuint texture2;
#pragma endregion

#pragma region Func
void init(void);
void display(void);
void reshape(int w, int h);
void idle(void);
#pragma endregion

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Dua First Glut");

	GLenum err = glewInit();

#ifdef DEBUG
	if (err != GLEW_OK) {
		std::cout << "ERROR:" << glewGetErrorString(err) << std::endl;
	}
	else {
		std::cout << "CLEW_OK!" << std::endl;
	}
#endif

	//Register Func
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();
	glutMainLoop();

	return 0;
}

void init(void)
{
	//background
	glClearColor(0.5, 0.5, 0.5, 0.0);

	//Texture
	int w, h, comp;
	unsigned char* image = stbi_load("Isaac5.2.PNG", &w, &h, &comp, STBI_rgb_alpha);

	if (image == nullptr) {
		std::cout << "Unable to load image. Make sure the image is in the same path as the executable.\n";
	}
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (comp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (comp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);

	//Create Program
	program = glCreateProgram();

	//shader
	readAndCompileShader(program, "vertex_textured.glsl", "fragment_textured.glsl");
	
#pragma region Attribute Data
	//"attr_position" is the value name list in .glsl
	positionAttribute = glGetAttribLocation(program, "attr_position");
	colorAttribute = glGetAttribLocation(program, "attr_color");
	textCoordAttribute = glGetAttribLocation(program, "attr_texCoord");

	//position attribute data
	glGenBuffers(1, &vertexPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosition);
	//Triangle Vertices
	GLfloat verts_Position[12] = 
	{ 
		-0.57, -0.8,
		-0.57, 0.8,
		0.57, 0.8,
		-0.57, -0.8,
		0.57,-0.8,
		0.57, 0.8 
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts_Position, GL_STATIC_DRAW);

	//color attribute data
	glGenBuffers(1, &vertexColor);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColor);
	GLfloat verts_Color[24] = 
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), verts_Color, GL_STATIC_DRAW);

	//texture
	glGenBuffers(1, &vertexTexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoord);
	GLfloat verts_TexCoord[12] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts_TexCoord, GL_STATIC_DRAW);

#pragma endregion

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
//RENDERING
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	//Position
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosition);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);
	//Color
	glBindBuffer(GL_ARRAY_BUFFER, vertexColor);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);
	//Texture
	glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoord);
	glVertexAttribPointer(textCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	//Disable
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);

	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
void idle(void)
{
	glutPostRedisplay();
}