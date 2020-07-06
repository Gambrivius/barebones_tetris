#include "tetrominoes.hpp"
#include "game.hpp"
#include "GL/freeglut.h"
#include "GL/gl.h"
Game game;

void display()
{
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT);
	game.draw();
	
    glutSwapBuffers();
}

void special_key(int key, int x, int y)
{
    game.handle_input(key, x, y);
	display();
}
void normal_key(unsigned char key, int x, int y)
{
    if (key == 27) exit(EXIT_SUCCESS);
	else if(key == 32) game.restart();
	else if(key== 13) game.instant_drop();
}

void loop(int i)
{
	game.fall();
	game.remove_lines();
	display();
	glutTimerFunc(game.get_speed() , loop, 0);
}
void setup() 
{
	srand(time(NULL));
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(TILE_WIDTH * (GAME_WIDTH + 7), TILE_HEIGHT * GAME_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Techris");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0, TILE_WIDTH * (GAME_WIDTH+7),  TILE_HEIGHT * GAME_HEIGHT, 0, 0, 1.0);
    glDisable(GL_DEPTH_TEST);
	
	glutDisplayFunc(display);
	glutSpecialFunc(special_key);
	glutKeyboardFunc(normal_key);

	game.setup();
	glutTimerFunc(game.get_speed(), loop, 0);

    glutMainLoop();
}




int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	setup();


    return 0;
}