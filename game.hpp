#ifndef __GAME_HPP__
#define __GAME_HPP__
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "tetrominoes.hpp"
#define GAME_WIDTH  10
#define GAME_HEIGHT 20

#define TILE_WIDTH 32
#define TILE_HEIGHT 32
int cur_x = 5;
int cur_y = 0;

class Game 
{
    Tetromino * cur_piece;
    Tetromino * next_piece;
    int grid[GAME_WIDTH][GAME_HEIGHT];
    
public:    
    int score = 0;
    int lines = 0;
    bool speed_drop = true;
    bool gamestarted = false;
    int get_level() 
    {
        if (lines >= 600) return lines/60;
        if (lines >= 500) return 9;
        if (lines >= 400) return 8;
        if (lines >= 300) return 7;
        if (lines >= 250) return 6;
        if (lines >= 200) return 5;
        if (lines >= 150) return 4;
        if (lines >= 100) return 3;
        if (lines >= 50) return 2;
        if (lines >= 10) return 1;
        return 0;

    }
    int get_speed() 
    {
        int speed = 0;
        switch (get_level()) {
            case 0:
                speed = 300;
                break;
            case 1:
                speed = 275;
                break;
            case 2:
                speed = 250;
                break;
            case 3:
                speed = 225;
                break;
            case 4:
                speed = 200;
                break;
            case 5:
                speed = 175;
                break;
            case 6:
                speed = 150;
                break;
            case 7:
                speed = 125;
                break;
            case 8:
                speed = 100;
                break;
            case 9:
                speed = 75;
                break;
            case 10:
                speed = 50;
                break;
            case 11:
                speed = 25;
                break;
            default:
                speed = 5;
                break;
        }
        return speed;
    }
    void get_random_piece() 
    {
        cur_x = GAME_WIDTH/2-2;
        cur_y = 0;
        cur_piece = next_piece;
        next_piece = MakeTetromino();
    }
    void clear_game()
    {
        for (int y = 0; y < GAME_HEIGHT; y++)
        {
            for (int x = 0; x < GAME_WIDTH; x++)
            {
                grid[x][y] = 0;
            }
        }
    }
    
    void end_fall() {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                int x0 = cur_x + x;
                int y0 = cur_y + y;
                int cell = cur_piece->get_cell(x,y);
                if (cell > 0) grid[x0][y0] = cell;
            }
        }
        delete cur_piece;
        get_random_piece();
    }
    bool detect_collisions() {
        // returns 0 if tetromino position is valid
        // returns 1 if tetromino is overlapping another piece
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (cur_piece->get_cell(x,y)>0)
                {
                    int x0 = cur_x + x;
                    int y0 = cur_y + y;
                    if (x0 < 0 || x0 >= GAME_WIDTH) return 1;
                    if (y0 < 0 || y0 >= GAME_HEIGHT) return 1;

                    if (grid[x0][y0]>0) return 1;
                }
            }
        }
        return 0;

    }
    void handle_input(unsigned char key, int x, int y)
    {
        switch (key) {
            case GLUT_KEY_UP:
                cur_piece->rotate();
                if (detect_collisions()) 
                {
                    // hacky way to achieve this.
                    // optimize later if i care
                    // if rotating collides, rotate 3 more times to arrive at original state
                    cur_piece->rotate();
                    cur_piece->rotate();
                    cur_piece->rotate();
                }
                break;
            case GLUT_KEY_CTRL_L:
            case GLUT_KEY_CTRL_R:
                speed_drop = !speed_drop;
                break;
            case GLUT_KEY_DOWN:
                cur_y ++;
                if (detect_collisions()) cur_y --;
               
                break;
            case GLUT_KEY_LEFT:
                cur_x --;
                if (detect_collisions()) cur_x ++;
                break;
            case GLUT_KEY_RIGHT:
                cur_x ++;
                if (detect_collisions()) cur_x --;
                break;
            default:
                break;
        }
    }
    void setup()
    {
        next_piece = MakeTetromino();
        lines = 0;
        score = 0;
        clear_game();
        get_random_piece();
    }
   
    void restart()
    {
        if (!gamestarted) {
            gamestarted = true;
            setup();
            draw();
        }
    }
    void gameover() 
    {
        gamestarted = false;
    }
    void fall()
    {
        if (!gamestarted) return;
        if (detect_collisions()) {
            // collisions at the beginning of a fall indicate gameover
            end_fall();
            gameover();
        }
        cur_y ++;
        if (detect_collisions())
        {
            cur_y --;
            end_fall();
        }
    }
    void select_color (int block_type)
    {
        switch (block_type)
        {
            case 1:
                glColor3f(1.0, 0.0, 0.0);
                break;
            case 2:
                glColor3f(0.0, 1.0, 0.0);
                break;
            case 3:
                glColor3f(0.0, 0.0, 1.0);
                break;
            case 4:
                glColor3f(0.0, 1.0, 1.0);
                break;
            case 5:
                glColor3f(1.0, 0.0, 1.0);
                break;
            case 6:
                glColor3f(1.0, 1.0, 0.0);
                break;
            case 7:
                glColor3f(1.0, 0.0, 1.0);
                break;
            case 8:
                glColor3f(0.0, 0.0, 0.0);
                break;
        }
    }
    void draw_block(int block_type, int x, int y)
    {
        int x0 = x * TILE_WIDTH;
        int y0 = y * TILE_HEIGHT;
        int x1 = x0 + TILE_WIDTH;
        int y1 = y0 + TILE_HEIGHT;

        
        if (block_type > 0)
        {
            select_color(block_type);
            glBegin(GL_QUADS);
            glVertex2f(x0, y0);
            glVertex2f(x1, y0);
            glVertex2f(x1, y1);
            glVertex2f(x0, y1);
            glEnd();

            if (block_type == 8)
            {
                glColor3f(0.3, 0.3, 0.3);
                glBegin(GL_LINE_LOOP);
                glVertex2f(x0, y0);
                glVertex2f(x1, y0);
                glVertex2f(x1, y1);
                glVertex2f(x0, y1);
                glEnd();
            } else {
                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_LINE_LOOP);
                glVertex2f(x0, y0);
                glVertex2f(x1, y0);
                glVertex2f(x1, y1);
                glVertex2f(x0, y1);
                glEnd();
            }
        }
    }

    void draw_cur_piece() 
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                int block_type = cur_piece->get_cell(x,y);
                if (block_type > 0) draw_block(block_type, x+cur_x, y+cur_y);
            }
        }
    }
    void instant_drop()
    {
        do
        {
            cur_y++;
        } while (!detect_collisions());
        cur_y--;
    }
    void draw_projection()
    {
        int tmp_y = cur_y;
        instant_drop();
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                int block_type = cur_piece->get_cell(x,y);
                if (block_type > 0) draw_block(8, x+cur_x, y+cur_y);
            }
        }
        cur_y = tmp_y;
        
    }
    void draw_next_piece() 
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                int block_type = next_piece->get_cell(x,y);
                if (block_type > 0) draw_block(block_type, x+GAME_WIDTH+1, y+1);
            }
        }
    }
    void remove_lines() {
        int clears = 0;
        for (int y = GAME_HEIGHT-1; y >= 0; y--)
        {
            bool full_row  = true;
            for (int x = 0; x < GAME_WIDTH; x++)
            {
                full_row &= (grid[x][y]>0);
            }
            if (full_row)
            {
                clears ++;
                int tmp_y = y;
                for (y=y; y >= 0; y--)
                {
                    for (int x = 0; x < GAME_WIDTH; x++)
                    {
                        if (y>0) grid[x][y] = grid[x][y-1];
                        else grid[x][y] = 0;
                    }
                
                }
                y = tmp_y+1;
            }

        }
        lines += clears;
        do {
            if (clears >= 4) {
                clears -= 4;
                score += 1200*(get_level()+1);
            } else if (clears >= 3) {
                clears -= 3;
                score += 300*(get_level()+1);
            } else if (clears >= 2) {
                clears -= 2;
                score += 100*(get_level()+1);
            } else if (clears >= 1) {
                clears -= 1;
                score += 40*(get_level()+1);
            }
        }
        while (clears > 0);
    }
    void draw_ui() {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(GAME_WIDTH*TILE_WIDTH, 0);
        glVertex2f(GAME_WIDTH*TILE_WIDTH, GAME_HEIGHT*TILE_HEIGHT);
        glVertex2f(0, GAME_HEIGHT*TILE_HEIGHT);
        glEnd();

        glColor3f(1.0, 1.0, 1.0); 
        glRasterPos2f((GAME_WIDTH )*TILE_WIDTH, GAME_HEIGHT/2*TILE_HEIGHT-28);
        char score_string [25];
        memset(score_string, ' ', sizeof(score_string));
        sprintf (score_string, "Score: %d", score);
        const unsigned char* t = reinterpret_cast<const unsigned char *>(score_string);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);
        
        memset(score_string, ' ', sizeof(score_string));
        sprintf (score_string, "Level: %d", get_level());
        glRasterPos2f((GAME_WIDTH )*TILE_WIDTH, GAME_HEIGHT/2*TILE_HEIGHT);
         t = reinterpret_cast<const unsigned char *>(score_string);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);

        memset(score_string, ' ', sizeof(score_string));
        sprintf (score_string, "Speed: %d", get_speed());
        glRasterPos2f((GAME_WIDTH )*TILE_WIDTH, GAME_HEIGHT/2*TILE_HEIGHT+28*1);
        t = reinterpret_cast<const unsigned char *>(score_string);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);

        const char *instruct_string = "Controls\nUp = Rotate\nLeft/Right = Move\nDown = Speed Drop\nEnter = Instant Drop\nCTRL = Preview\nEsc = Escape\nSpace = Start";
        glRasterPos2f((GAME_WIDTH)*TILE_WIDTH, GAME_HEIGHT/2*TILE_HEIGHT+28*2);
        t = reinterpret_cast<const unsigned char *>(instruct_string);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);

        if (!gamestarted) {
            const char *instruct_string = "Press Space To Begin";
            glRasterPos2f((GAME_WIDTH)*TILE_WIDTH/2-100, GAME_HEIGHT/2*TILE_HEIGHT);
            t = reinterpret_cast<const unsigned char *>(instruct_string);
            glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);
        }
    }
    void draw_screen()
    {
        for (int y = 0; y < GAME_HEIGHT; y++)
        {
            for (int x = 0; x < GAME_WIDTH; x++)
            {

                int block_type = grid[x][y];
                if (block_type > 0) draw_block(block_type, x, y);
            }
        }
    }
     void draw()
    {
        draw_screen();
        if (gamestarted) {
            if (speed_drop) draw_projection();
            draw_cur_piece();
	        draw_next_piece();

            
        }
        draw_ui();
    }
};


#endif