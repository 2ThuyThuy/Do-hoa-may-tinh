#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
using namespace std;
void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glEnd();
	glutSwapBuffers();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // Set up some memory buffers for our display
	glutInitWindowSize(700, 700); // Set the window size
	glutCreateWindow("OpenGL example");
	glutReshapeFunc(changeViewPort); // Bind the two functions (above) to respond when necessary
	glutDisplayFunc(render); // Very important! This initializes the entry points in the OpenGL driver so we can call all the functions in the API.
		GLenum err = glewInit();
	if (GLEW_OK != err) {
		cout << stderr << " GLEW error";
		return 1;
	}
	glutMainLoop();
	return 0;
}