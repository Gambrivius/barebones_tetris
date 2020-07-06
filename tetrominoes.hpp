#ifndef __TETROMINOES_HPP__
#define __TETROMINOES_HPP__

#include <iostream>
#include <string.h>



char shape0[17] = "0100"
                  "0100"
                  "0100"  
                  "0100";

char shape1[17] = "0000"
                  "0110"
                  "0110"  
                  "0000";

char shape2[17] = "0000"
                  "1110"
                  "0100"  
                  "0000";

char shape3[17] = "0000"
                  "0110"
                  "1100"  
                  "0000";

char shape4[17] = "0000"
                  "1100"
                  "0110"  
                  "0000";

char shape5[17] = "0100"
                  "0100"
                  "0110"  
                  "0000";

char shape6[17] = "0010"
                  "0010"
                  "0110"  
                  "0000";

class Tetromino 
{
public:
    
    int cell[17];
    Tetromino(int stype) {
        char *shape_ptr;
        switch (stype) {
            case 0:
                shape_ptr = shape0;
                break;
            case 1:
                shape_ptr = shape1;
                break;
            case 2:
                shape_ptr = shape2;
                break;
            case 3:
                shape_ptr = shape3;
                break;
            case 4:
                shape_ptr = shape4;
                break;
            case 5:
                shape_ptr = shape5;
                break;
            case 6:
                shape_ptr = shape6;
                break;
        }
        

        int pos = 0;

        
        for (pos = 0; pos <= 16; pos ++)
        {
            int i = 0;
            //std::cout << &shape_ptr[pos];
            if (shape_ptr[pos] == '1') {
                i = stype+1;
            }
            cell[pos] = i;

        }
    }
    void print() {
        int row = 0;
        int col = 0;
        for (row = 0; row < 4; row++) {
            for (col = 0; col < 4; col++) {
                int index = row * 4 + col;
                std::cout << cell[index];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    int get_cell (int x, int y)
    {
        int index = y * 4 + x;
        return cell[index];
    }
    void rotate() {
        int new_cell[17];
        int row = 0;
        int col = 0;
        for (row = 0; row < 4; row++) {
            for (col = 0; col < 4; col++) {
                int new_row = col;
                int new_col = 3-row;
                int index = row * 4 + col;
                int new_index = new_row * 4 + new_col;
                new_cell[new_index] = cell[index];
            }
        }
        mempcpy(cell, new_cell, 16*sizeof(int));
        
    }


};

Tetromino * MakeTetromino (int i =rand()%7) {
    return new Tetromino(i);
}

#endif