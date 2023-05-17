#pragma once

constexpr unsigned char CELL_SIZE = 30;

const int GRID[2][2] = { {257, 0}, {556, 598} };

const unsigned int difficulty = 6; // for testing

const unsigned int target = 5;

const unsigned char COLUMNS = 10;

const unsigned char ROWS = 20;

const unsigned int CELL_POS_X = 259;

const unsigned int CELL_POS_Y = 0;

const float TETRIMINO_APPEAR_X = 257.7;

const float TETRIMINO_APPEAR_Y = -1;

const float START_FALL_SPEED = 1; //decrease value = more speed , increase value = less speed

float START_FALL_SPEED_ARRAY[8] = { START_FALL_SPEED, START_FALL_SPEED - 0.1, START_FALL_SPEED - 0.2, START_FALL_SPEED - 0.3, START_FALL_SPEED - 0.5, START_FALL_SPEED - 0.6,  START_FALL_SPEED - 0.7, START_FALL_SPEED - 0.8 };
