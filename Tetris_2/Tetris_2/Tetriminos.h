#ifndef TETRIMINOS_H
#define TETRIMINOS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "well.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace sf;

Color darkBlue(0, 0, 255);
Color Orange(255, 165, 0);

//base class
class Tetrimino { //base class
protected:
	int x;
	int y;
	vector<vector <int>> shape;
	Texture texture;
	vector<vector<int>> initialShape;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	Color color;
	Sprite sprite;
	Tetrimino() :color(Color::Red) {}
	virtual vector<vector<int>> getShape() const = 0;
	virtual void setShape(vector<vector<int>> shape) = 0;
	virtual void rotate(Matrix& well) = 0;
	virtual void draw(RenderWindow& window) = 0;
	virtual void moveDown(Matrix& well, RenderWindow& window) = 0;
	virtual void moveLeft(Matrix& well, RenderWindow& window) = 0;
	virtual void moveRight(Matrix& well, RenderWindow& window) = 0;
	virtual bool canMoveDown(Matrix& well) = 0;
	virtual bool canMoveLeftRight(Matrix& well) = 0;
	virtual void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) = 0;
	virtual void checkGameOver(Matrix& well) = 0;
	virtual Sprite& getSprite() = 0;
	virtual Texture getTexture() = 0;
	virtual Color getColor() = 0;
	virtual void setPosition(float starting_x, float starting_y) = 0;
	virtual void setRotation() = 0;
	virtual void reset() = 0;
};

//derived ones
class I_Tetrimino : public Tetrimino {
protected:
	int x;
	int y;
	int rotate_no = 0;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	vector<vector <int>> shape;
	vector<vector<int>> initialShape;
	Sprite sprite;
	Color color;

	I_Tetrimino() {
		shape = { {0, 0, 1, 0},
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 }
		};
		initialShape = shape;
		x = -1;
		y = COLUMNS / 2 - 2;
		color = Color::Cyan;
		locked = false;
		lines_removed = 0;
		gameOver = false;
	}

	Color getColor() {
		this->color = Color::Cyan;
		return this->color;
	}

	vector<vector<int>> getShape() const {
		return shape;
	}

	void setShape(vector<vector<int>> shape) {
		this->shape = shape;
	}

	void rotate(Matrix& well) {
		vector<vector<int>> rotated(shape[0].size(), vector<int>(shape.size(), 0));

		// Determine the target shape based on the current shape
		if ((shape == vector <vector<int>>{ {0, 0, 1, 0},
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 } }))
		{
			rotated = { {1, 1, 1, 1} };
		}

		else if (shape == vector<vector<int>>{ {1, 1, 1, 1} } && rotate_no == 0) {
			rotated = { { {0, 1, 0, 0},
				{ 0, 1, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 1, 0, 0 } } };

			rotate_no++;
		}

		else if (shape == vector<vector<int>>{ {0, 1, 0, 0},
			{ 0, 1, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 1, 0, 0 } }) {
			rotated = { {1, 1, 1, 1} };
		}

		else if (shape == vector<vector<int>>{ {1, 1, 1, 1} }&& rotate_no == 1) {
			rotated = { { {0, 0, 1, 0},
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 } } };

			rotate_no = 0;
		}

		// Check if the target shape is valid
		int new_x = x;
		int new_y = y;
		for (int i = 0; i < rotated.size(); i++) {
			for (int j = 0; j < rotated[0].size(); j++) {
				int row = i + x;
				int col = j + y;
				if (rotated[i][j] != 0) {
					if (col < 0) {
						new_y++;
					}
					else if (col >= COLUMNS) {
						new_y--;
					}
					else if (row >= ROWS || well.getGrid()[row][col] != 0) {
						// The rotation collides with another block or goes out of bounds
						setShape(shape);
						return;
					}
				}
			}
		}

		// Set the new shape and position
		setShape(rotated);
		x = new_x;
		y = new_y;
	}





	void draw(RenderWindow& window) {
		RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

		Color Orange(255, 165, 0);
		block.setFillColor(Color::Cyan);

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0) {
					try {
						block.setPosition(TETRIMINO_APPEAR_X + (y + j) * CELL_SIZE + 1, TETRIMINO_APPEAR_Y + (x + i) * CELL_SIZE + 1);
						window.draw(block);
					}
					catch (std::out_of_range& e) {
						// Handle the exception here
						cerr << "Exception caught: " << e.what() << endl;
					}
				}
			}
		}
	}

	void moveLeft(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the left
		y--;
	}

	void moveRight(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == COLUMNS - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the right
		y++;
	}



	void moveDown(Matrix& well, RenderWindow& window) {
		// Check if the tetrimino can move down
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if bottom is reached
					if (x + i == ROWS - 1) {
						return;
					}
					// Check if there's a block below
					if (well.getGrid()[x + i + 1][y + j] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino down
		x++;
	}

	bool canMoveLeftRight(Matrix& well) {
		cout << shape[0][0] << endl;
		
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return false;
					}
				}
			}
		}

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == well.getNumColumns() - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return false;
					}
				}
			}
		}

		return true;
	}


	bool canMoveDown(Matrix& well) {
		//if (locked) {
		//	return false;
		//}
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {

				if (shape[i][j] == 1) {
					// Check if there's a block belown or matrix en
					if (x + shape.size() - 1 >= ROWS - 1 || well.getGrid()[x + i + 1][y + j] != 0) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) {
		if (locked) {
			return;
		}


		// Update the well with the tetromino's shape
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[i].size(); j++) {
				if (shape[i][j] != 0) {
					int row = x + i;
					int col = y + j;
					if (row >= ROWS) {
						// Do not update if the tetromino is partially filling the last row
						continue;
					}
					if (col < 0 || col >= COLUMNS || well.getGridValue(row, col) != 0) {
						gameOver = true;
						exit(1);
					}
					//if (x + i == ROWS - 3 && well.getGridValue(x + i + 1, y + j) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					//if (shape[i][j] == 1 && row == ROWS - 3 && well.getGridValue(row + 1, col) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					well.setGridValue(row, col, shape[i][j]);
				}
			}
		}


		

		vector<int> rowsToShift; // Store indices of rows to shift downwards
		// Check for filled rows and empty them
		for (int i = well.matrix.size() - 1; i >= 0; i--) {
			bool rowFilled = true;
			for (int j = 0; j < well.matrix[i].size(); j++) {
				if (well.matrix[i][j] == 0) {
					rowFilled = false;
					break;
				}
			}
			if (rowFilled) {
				// Empty the row
				for (int j = 0; j < well.matrix[i].size(); j++) {
					well.setGridValue(i, j, 0);
				}
				score += 100;
				lines += 1;
				rowsToShift.push_back(i); // Add the index of the cleared row to the rowsToShift vector
			}
		}

		// Shift the rows above the cleared row downwards
		for (int i = rowsToShift.size() - 1; i >= 0; i--) {
			int curRow = rowsToShift[i];
			for (int j = 0; j < well.matrix[curRow].size(); j++) {
				for (int k = curRow - 1; k >= 0; k--) {
					well.setGridValue(k + 1, j, well.matrix[k][j]);
					well.setGridValue(k, j, 0);
				}
			}
		}


		// Update the sprite's position to match the locked tetromino's position
		/*sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);*/

		cout << endl << endl;
		//cout << canMoveDown(well) ? "true" : "false\n";
		for (int i = 0; i < well.matrix.size(); i++) {
			for (int j = 0; j < well.matrix[i].size(); j++) {
				cout << well.matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << "score = " << score << " lines = " << lines << endl << endl;

		// Signal that the tetromino has locked
		locked = true;
	}

	void checkGameOver(Matrix& well) {
		for (int j = 0; j < well.matrix[0].size(); j++) {
			if (well.matrix[0][j] != 0) {
				gameOver = true;
				break;
			}
		}
	}


	Sprite& getSprite() {
		return sprite;
	}
	Texture getTexture() {
		return texture;
	}
	void setPosition(float starting_x, float starting_y) {
		x = starting_x;
		y = starting_y;
	}
	void setRotation() {
		shape = { {0, 0, 1, 0},
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 }
		};
	}
	void reset() {
		x = -1;
		y = COLUMNS / 2 - 2;
		shape = initialShape;
		locked = false;
	}

};

class J_Tetrimino : public Tetrimino {
protected:
	int x;
	int y;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	vector<vector <int>> shape;
	vector<vector<int>> initialShape;
	Sprite sprite;
	Color color;

	J_Tetrimino() {
		shape = { { 0, 2, 0 },
				  { 0, 2, 0 },
				  { 2, 2, 0 }
		};
		x = -1;
		y = COLUMNS / 2 - 2;
		initialShape = shape;
		color = darkBlue;
		locked = false;
		lines_removed = 0;
		gameOver = false;
	}

	Color getColor() {
		this->color = darkBlue;
		return this->color;
	}

	vector<vector<int>> getShape() const {
		return shape;
	}

	void setShape(vector<vector<int>> shape) {
		this->shape = shape;
	}

	void rotate(Matrix& well) {
		vector<vector<int>> rotated(shape[0].size(), vector<int>(shape.size(), 0));

		// Determine the target shape based on the current shape
		if (shape == vector<vector<int>>{{ 0, 2, 0 },
										 { 0, 2, 0 },
										 { 2, 2, 0 }}) {
			rotated = { {2, 0, 0},
					   {2, 2, 2},
					   {0, 0, 0} };
		}
		else if (shape == vector<vector<int>>{{ 2, 0, 0 },
											  { 2, 2, 2 },
											  { 0, 0, 0 }}) {
			rotated = {{0, 2, 2},
					   {0, 2, 0},
					   {0, 2, 0} };
		}
		else if (shape == vector<vector<int>>{ {0, 2, 2},
			{ 0, 2, 0 },
			{ 0, 2, 0 }}) {
			rotated = { {0, 0, 0},
					   {2, 2, 2},
					   {0, 0, 2} };
		}
		else if (shape == vector<vector<int>>{ {0, 0, 0},
			{ 2, 2, 2 },
			{ 0, 0, 2 }}) {
			rotated = { { 0, 2, 0 },
						{ 0, 2, 0 },
						{ 2, 2, 0 } };
		}

		// Check if the target shape is valid
		int new_x = x;
		int new_y = y;
		for (int i = 0; i < rotated.size(); i++) {
			for (int j = 0; j < rotated[0].size(); j++) {
				int row = i + x;
				int col = j + y;
				if (rotated[i][j] != 0) {
					if (col < 0) {
						new_y++;
					}
					else if (col >= COLUMNS) {
						new_y--;
					}
					else if (row >= ROWS || well.getGrid()[row][col] != 0) {
						// The rotation collides with another block or goes out of bounds
						setShape(shape);
						return;
					}
				}
			}
		}

		// Set the new shape and position
		setShape(rotated);
		x = new_x;
		y = new_y;
	}

	void draw(RenderWindow& window) {
		RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

		Color Orange(255, 165, 0);
		block.setFillColor(darkBlue);

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 2) {
					try {
						block.setPosition(TETRIMINO_APPEAR_X + (y + j) * CELL_SIZE + 1, TETRIMINO_APPEAR_Y + (x + i) * CELL_SIZE + 1);
						window.draw(block);
					}
					catch (std::out_of_range& e) {
						// Handle the exception here
						// For example, you could print an error message or log the error
						cerr << "Exception caught: " << e.what() << endl;
					}
				}
			}
		}
	}

	void moveLeft(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 2) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the left
		y--;
	}

	void moveRight(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 2) {
					// Check if there is a block to the right
					if (y + j == COLUMNS - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the right
		y++;
	}



	void moveDown(Matrix& well, RenderWindow& window) {
		// Check if the tetrimino can move down
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 2) {
					// Check if bottom is reached
					if (x + i == ROWS - 1) {
						return;
					}
					// Check if there's a block below
					if (well.getGrid()[x + i + 1][y + j] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino down
		x++;
	}

	bool canMoveLeftRight(Matrix& well) {
		cout << shape[0][0] << endl;

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return false;
					}
				}
			}
		}

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == well.getNumColumns() - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return false;
					}
				}
			}
		}

		return true;
	}


	bool canMoveDown(Matrix& well) {
		if (locked) {
			return false;
		}
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {

				if (shape[i][j] != 0 && shape[i][j] == 2) {
					// Check if there's a block belown or matrix en
					if (x + i + 1 >= ROWS || well.getGrid()[x + i + 1][y + j] != 0) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) {
		if (locked) {
			return;
		}

		// Update the well with the tetromino's shape
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[i].size(); j++) {
				if (shape[i][j] != 0) {
					int row = x + i;
					int col = y + j;
					if (row >= ROWS) {
						// Do not update if the tetromino is partially filling the last row
						continue;
					}
					if (col < 0 || col >= COLUMNS || well.getGridValue(row, col) != 0) {
						gameOver = true;
						exit(1);
					}
					//if (x + i == ROWS - 3 && well.getGridValue(x + i + 1, y + j) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					//if (shape[i][j] == 2 && row == ROWS - 3 && well.getGridValue(row + 1, col) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					well.setGridValue(row, col, shape[i][j]);
				}
			}
		}


		// Check if any of the top rows are occupied
	
		vector<int> rowsToShift; // Store indices of rows to shift downwards
		// Check for filled rows and empty them
		for (int i = well.matrix.size() - 1; i >= 0; i--) {
			bool rowFilled = true;
			for (int j = 0; j < well.matrix[i].size(); j++) {
				if (well.matrix[i][j] == 0) {
					rowFilled = false;
					break;
				}
			}
			if (rowFilled) {
				// Empty the row
				for (int j = 0; j < well.matrix[i].size(); j++) {
					well.setGridValue(i, j, 0);
				}
				score += 100;
				lines += 1;
				rowsToShift.push_back(i); // Add the index of the cleared row to the rowsToShift vector
			}
		}

		// Shift the rows above the cleared row downwards
		for (int i = rowsToShift.size() - 1; i >= 0; i--) {
			int curRow = rowsToShift[i];
			for (int j = 0; j < well.matrix[curRow].size(); j++) {
				for (int k = curRow - 1; k >= 0; k--) {
					well.setGridValue(k + 1, j, well.matrix[k][j]);
					well.setGridValue(k, j, 0);
				}
			}
		}


		// Update the sprite's position to match the locked tetromino's position
		/*sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);*/

		cout << endl << endl;
		//cout << canMoveDown(well) ? "true" : "false\n";
		for (int i = 0; i < well.matrix.size(); i++) {
			for (int j = 0; j < well.matrix[i].size(); j++) {
				cout << well.matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << "score = " << score << " lines = " << lines << endl << endl;

		// Signal that the tetromino has locked
		locked = true;
	}

	void checkGameOver(Matrix& well) {
		for (int j = 0; j < well.matrix[0].size(); j++) {
			if (well.matrix[0][j] != 0) {
				gameOver = true;
				break;
			}
		}
	}


	Sprite& getSprite() {
		return sprite;
	}
	Texture getTexture() {
		return texture;
	}
	void setPosition(float starting_x, float starting_y) {
		x = starting_x;
		y = starting_y;
	}
	void setRotation() {
		shape = { { 0, 2, 0 },
				  { 0, 2, 0 },
				  { 2, 2, 0 }
		};
	}
	void reset() {
		x = -1;
		y = COLUMNS / 2 - 2;
		shape = initialShape;
		locked = false;
	}
};

class L_Tetrimino : public Tetrimino {
protected:
	int x;
	int y;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	vector<vector <int>> shape;
	vector<vector<int>> initialShape;
	Sprite sprite;
	Color color;

	L_Tetrimino() {
		shape = { { 0, 3, 0},
				  { 0, 3, 0},
				  { 0, 3, 3}
		};
		x = -1;
		y = COLUMNS / 2 - 2;
		initialShape = shape;
		color = Orange;
		locked = false;
		lines_removed = 0;
		gameOver = false;
	}

	Color getColor() {
		this->color = Orange;
		return this->color;
	}

	vector<vector<int>> getShape() const {
		return shape;
	}

	void setShape(vector<vector<int>> shape) {
		this->shape = shape;
	}

	void rotate(Matrix& well) {
		vector<vector<int>> rotated(shape[0].size(), vector<int>(shape.size(), 0));

		// Determine the target shape based on the current shape
		if (shape == vector<vector<int>>{ { 0, 3, 0 },
			{ 0, 3, 0 },
			{ 0, 3, 3 }}) {
			rotated = { {0, 0, 0},
					   {3, 3, 3},
					   {3, 0, 0} };
		}
		else if (shape == vector<vector<int>>{ {0, 0, 0},
			{ 3, 3, 3 },
			{ 3, 0, 0 }}) {
			rotated = { {3, 3, 0},
					   {0, 3, 0},
					   {0, 3, 0} };
		}
		else if (shape == vector<vector<int>>{ {3, 3, 0},
			{ 0, 3, 0 },
			{ 0, 3, 0 }}) {
			rotated = { {0, 0, 3},
					   {3, 3, 3},
					   {0, 0, 0} };
		}
		else if (shape == vector<vector<int>>{ {0, 0, 3},
			{ 3, 3, 3 },
			{ 0, 0, 0 }}) {
			rotated = { { 0, 3, 0 },
						{ 0, 3, 0 },
						{ 0, 3, 3 } };
		}

		// Check if the target shape is valid
		int new_x = x;
		int new_y = y;
		for (int i = 0; i < rotated.size(); i++) {
			for (int j = 0; j < rotated[0].size(); j++) {
				int row = i + x;
				int col = j + y;
				if (rotated[i][j] != 0) {
					if (col < 0) {
						new_y++;
					}
					else if (col >= COLUMNS) {
						new_y--;
					}
					else if (row >= ROWS || well.getGrid()[row][col] != 0) {
						// The rotation collides with another block or goes out of bounds
						setShape(shape);
						return;
					}
				}
			}
		}

		// Set the new shape and position
		setShape(rotated);
		x = new_x;
		y = new_y;
	}

	void draw(RenderWindow& window) {
		RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

		block.setFillColor(Orange);

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 3) {
					try {
						block.setPosition(TETRIMINO_APPEAR_X + (y + j) * CELL_SIZE + 1, TETRIMINO_APPEAR_Y + (x + i) * CELL_SIZE + 1);
						window.draw(block);
					}
					catch (std::out_of_range& e) {
						// Handle the exception here
						// For example, you could print an error message or log the error
						cerr << "Exception caught: " << e.what() << endl;
					}
				}
			}
		}
	}

	void moveLeft(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 3) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the left
		y--;
	}

	void moveRight(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 3) {
					// Check if there is a block to the right
					if (y + j == COLUMNS - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the right
		y++;
	}



	void moveDown(Matrix& well, RenderWindow& window) {
		// Check if the tetrimino can move down
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 3) {
					// Check if bottom is reached
					if (x + i == ROWS - 1) {
						return;
					}
					// Check if there's a block below
					if (well.getGrid()[x + i + 1][y + j] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino down
		x++;
	}

	bool canMoveLeftRight(Matrix& well) {
		cout << shape[0][0] << endl;

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return false;
					}
				}
			}
		}

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == well.getNumColumns() - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return false;
					}
				}
			}
		}

		return true;
	}

	bool canMoveDown(Matrix& well) {
		if (locked) {
			return false;
		}
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {

				if (shape[i][j] != 0 && shape[i][j] == 3) {
					// Check if there's a block belown or matrix en
					if (x + i + 1 >= ROWS || well.getGrid()[x + i + 1][y + j] != 0) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) {
		if (locked) {
			return;
		}

		// Update the well with the tetromino's shape
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[i].size(); j++) {
				if (shape[i][j] != 0) {
					int row = x + i;
					int col = y + j;
					if (row >= ROWS) {
						// Do not update if the tetromino is partially filling the last row
						continue;
					}
					if (col < 0 || col >= COLUMNS || well.getGridValue(row, col) != 0) {
						gameOver = true;
						exit(1);
					}
					//if (x + i == ROWS - 3 && well.getGridValue(x + i + 1, y + j) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					//if (shape[i][j] == 3 && row == ROWS - 3 && well.getGridValue(row + 1, col) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					well.setGridValue(row, col, shape[i][j]);
				}
			}
		}


		vector<int> rowsToShift; // Store indices of rows to shift downwards
		// Check for filled rows and empty them
		for (int i = well.matrix.size() - 1; i >= 0; i--) {
			bool rowFilled = true;
			for (int j = 0; j < well.matrix[i].size(); j++) {
				if (well.matrix[i][j] == 0) {
					rowFilled = false;
					break;
				}
			}
			if (rowFilled) {
				// Empty the row
				for (int j = 0; j < well.matrix[i].size(); j++) {
					well.setGridValue(i, j, 0);
				}
				score += 100;
				lines += 1;
				rowsToShift.push_back(i); // Add the index of the cleared row to the rowsToShift vector
			}
		}

		// Shift the rows above the cleared row downwards
		for (int i = rowsToShift.size() - 1; i >= 0; i--) {
			int curRow = rowsToShift[i];
			for (int j = 0; j < well.matrix[curRow].size(); j++) {
				for (int k = curRow - 1; k >= 0; k--) {
					well.setGridValue(k + 1, j, well.matrix[k][j]);
					well.setGridValue(k, j, 0);
				}
			}
		}


		// Update the sprite's position to match the locked tetromino's position
		/*sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);*/

		cout << endl << endl;
		//cout << canMoveDown(well) ? "true" : "false\n";
		for (int i = 0; i < well.matrix.size(); i++) {
			for (int j = 0; j < well.matrix[i].size(); j++) {
				cout << well.matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << "score = " << score << " lines = " << lines << endl << endl;

		// Signal that the tetromino has locked
		locked = true;
	}

	void checkGameOver(Matrix& well) {
		for (int j = 0; j < well.matrix[0].size(); j++) {
			if (well.matrix[0][j] != 0) {
				gameOver = true;
				break;
			}
		}
	}


	Sprite& getSprite() {
		return sprite;
	}
	Texture getTexture() {
		return texture;
	}
	void setPosition(float starting_x, float starting_y) {
		x = starting_x;
		y = starting_y;
	}
	void setRotation() {
		shape = { { 0, 3, 0},
				  { 0, 3, 0},
				  { 0, 3, 3}
		};
	}
	void reset() {
		x = -1;
		y = COLUMNS / 2 - 2;
		shape = initialShape;
		locked = false;
	}
};

class O_Tetrimino : public Tetrimino {
protected:
	int x;
	int y;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	vector<vector <int>> shape;
	vector<vector<int>> initialShape;
	Sprite sprite;
	Color color;

	O_Tetrimino() {
		shape = { { 4, 4, 0, 0 },
				  { 4, 4, 0, 0 },
				  { 0, 0, 0, 0 },
				  { 0, 0, 0, 0 }
		};
		x = -1;
		y = COLUMNS / 2 - 2;
		initialShape = shape;
		color = Color::Yellow;
		locked = false;
		lines_removed = 0;
		gameOver = false;
	}

	Color getColor() {
		this->color = Color::Yellow;
		return this->color;
	}

	vector<vector<int>> getShape() const {
		return shape;
	}

	void setShape(vector<vector<int>> shape) {
		this->shape = shape;
	}

	void rotate(Matrix& well) {
		return;
	}

	void draw(RenderWindow& window) {
		RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

		Color Orange(255, 165, 0);
		block.setFillColor(Color::Yellow);

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 4) {
					try {
						block.setPosition(TETRIMINO_APPEAR_X + (y + j) * CELL_SIZE + 1, TETRIMINO_APPEAR_Y + (x + i) * CELL_SIZE + 1);
						window.draw(block);
					}
					catch (std::out_of_range& e) {
						// Handle the exception here
						// For example, you could print an error message or log the error
						cerr << "Exception caught: " << e.what() << endl;
					}
				}
			}
		}
	}

	void moveLeft(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 4) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the left
		y--;
	}

	void moveRight(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 4) {
					// Check if there is a block to the right
					if (y + j == COLUMNS - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the right
		y++;
	}



	void moveDown(Matrix& well, RenderWindow& window) {
		// Check if the tetrimino can move down
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 3) {
					// Check if bottom is reached
					if (x + i == ROWS - 1) {
						return;
					}
					// Check if there's a block below
					if (well.getGrid()[x + i + 1][y + j] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino down
		x++;
	}

	bool canMoveLeftRight(Matrix& well) {
		cout << shape[0][0] << endl;

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return false;
					}
				}
			}
		}

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == well.getNumColumns() - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return false;
					}
				}
			}
		}

		return true;
	}


	bool canMoveDown(Matrix& well) {
		if (locked) {
			return false;
		}
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {

				if (shape[i][j] != 0 && shape[i][j] == 4) {
					// Check if there's a block belown or matrix en
					if (x + i + 1 >= ROWS || well.getGrid()[x + i + 1][y + j] != 0) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) {
		if (locked) {
			return;
		}

		// Update the well with the tetromino's shape
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[i].size(); j++) {
				if (shape[i][j] != 0) {
					int row = x + i;
					int col = y + j;
					if (row >= ROWS) {
						// Do not update if the tetromino is partially filling the last row
						continue;
					}
					if (col < 0 || col >= COLUMNS || well.getGridValue(row, col) != 0) {
						gameOver = true;
						exit(1);
					}
					//if (x + i == ROWS - 3 && well.getGridValue(x + i + 1, y + j) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					//if (shape[i][j] == 4 && row == ROWS - 3 && well.getGridValue(row + 1, col) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					well.setGridValue(row, col, shape[i][j]);
				}
			}
		}


	

		vector<int> rowsToShift; // Store indices of rows to shift downwards
		// Check for filled rows and empty them
		for (int i = well.matrix.size() - 1; i >= 0; i--) {
			bool rowFilled = true;
			for (int j = 0; j < well.matrix[i].size(); j++) {
				if (well.matrix[i][j] == 0) {
					rowFilled = false;
					break;
				}
			}
			if (rowFilled) {
				// Empty the row
				for (int j = 0; j < well.matrix[i].size(); j++) {
					well.setGridValue(i, j, 0);
				}
				score += 100;
				lines += 1;
				rowsToShift.push_back(i); // Add the index of the cleared row to the rowsToShift vector
			}
		}

		// Shift the rows above the cleared row downwards
		for (int i = rowsToShift.size() - 1; i >= 0; i--) {
			int curRow = rowsToShift[i];
			for (int j = 0; j < well.matrix[curRow].size(); j++) {
				for (int k = curRow - 1; k >= 0; k--) {
					well.setGridValue(k + 1, j, well.matrix[k][j]);
					well.setGridValue(k, j, 0);
				}
			}
		}


		// Update the sprite's position to match the locked tetromino's position
		/*sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);*/

		cout << endl << endl;
		//cout << canMoveDown(well) ? "true" : "false\n";
		for (int i = 0; i < well.matrix.size(); i++) {
			for (int j = 0; j < well.matrix[i].size(); j++) {
				cout << well.matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << "score = " << score << " lines = " << lines << endl << endl;

		// Signal that the tetromino has locked
		locked = true;
	}

	void checkGameOver(Matrix& well) {
		for (int j = 0; j < well.matrix[0].size(); j++) {
			if (well.matrix[0][j] != 0) {
				gameOver = true;
				break;
			}
		}
	}

	Sprite& getSprite() {
		return sprite;
	}
	Texture getTexture() {
		return texture;
	}
	void setPosition(float starting_x, float starting_y) {
		x = starting_x;
		y = starting_y;
	}
	void setRotation() {
		return;
	}
	void reset() {
		x = -1;
		y = COLUMNS / 2 - 2;
		shape = initialShape;
		locked = false;
	}

};

class S_Tetrimino : public Tetrimino {
protected:
	int x;
	int y;
	int rotation_no = 0;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	vector<vector<int>> initialShape;
	vector<vector <int>> shape;
	Sprite sprite;
	Color color;

	S_Tetrimino() {
		shape = { { 0, 5, 5},
				  { 5, 5, 0},
				  { 0, 0, 0}
		};
		x = -1;
		y = COLUMNS / 2 - 2;
		initialShape = shape;
		color = Color::Green;
		locked = false;
		lines_removed = 0;
		gameOver = false;
	}

	Color getColor() {
		this->color = Color::Green;
		return this->color;
	}

	vector<vector<int>> getShape() const {
		return shape;
	}

	void setShape(vector<vector<int>> shape) {
		this->shape = shape;
	}

	void rotate(Matrix& well) {
		// Define the rotation matrix
		vector<vector<int>> rotation_matrix{ {0, 1}, {-1, 0} };

		// Apply the rotation matrix to each block in the tetromino shape
		vector<vector<int>> rotated(shape[0].size(), vector<int>(shape.size(), 0));
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				int new_i = j;
				int new_j = shape.size() - 1 - i;
				rotated[new_i][new_j] = shape[i][j];
			}
		}

		// Check if the target shape is valid
		int new_x = x;
		int new_y = y;
		for (int i = 0; i < rotated.size(); i++) {
			for (int j = 0; j < rotated[0].size(); j++) {
				int row = i + x;
				int col = j + y;
				if (rotated[i][j] != 0) {
					// Check if the rotated block is within the boundaries of the well matrix
					if (col < 0) {
						new_y++;
					}
					else if (col >= COLUMNS) {
						new_y--;
					}
					else if (row >= ROWS || well.getGrid()[row][col] != 0) {
						// The rotation collides with another block or goes out of bounds
						setShape(shape);
						return;
					}
				}
			}
		}

		// Set the new shape and position
		setShape(rotated);
		x = new_x;
		y = new_y;
	}



	void draw(RenderWindow& window) {
		RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

		Color Orange(255, 165, 0);
		block.setFillColor(Color::Green);

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 5) {
					try {
						block.setPosition(TETRIMINO_APPEAR_X + (y + j) * CELL_SIZE + 1, TETRIMINO_APPEAR_Y + (x + i) * CELL_SIZE + 1);
						window.draw(block);
					}
					catch (std::out_of_range& e) {
						// Handle the exception here
						// For example, you could print an error message or log the error
						cerr << "Exception caught: " << e.what() << endl;
					}
				}
			}
		}
	}

	void moveLeft(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 5) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the left
		y--;
	}

	void moveRight(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 5) {
					// Check if there is a block to the right
					if (y + j == COLUMNS - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the right
		y++;
	}



	void moveDown(Matrix& well, RenderWindow& window) {
		// Check if the tetrimino can move down
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 5) {
					// Check if bottom is reached
					if (x + i == ROWS - 1) {
						return;
					}
					// Check if there's a block below
					if (well.getGrid()[x + i + 1][y + j] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino down
		x++;
	}

	bool canMoveLeftRight(Matrix& well) {
		cout << shape[0][0] << endl;

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return false;
					}
				}
			}
		}

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == well.getNumColumns() - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return false;
					}
				}
			}
		}

		return true;
	}

	bool canMoveDown(Matrix& well) {
		if (locked) {
			return false;
		}
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {

				if (shape[i][j] != 0 && shape[i][j] == 5) {
					// Check if there's a block belown or matrix en
					if (x + i + 1 >= ROWS || well.getGrid()[x + i + 1][y + j] != 0) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) {
		if (locked) {
			return;
		}

		// Update the well with the tetromino's shape
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[i].size(); j++) {
				if (shape[i][j] != 0) {
					int row = x + i;
					int col = y + j;
					if (row >= ROWS) {
						// Do not update if the tetromino is partially filling the last row
						continue;
					}
					if (col < 0 || col >= COLUMNS || well.getGridValue(row, col) != 0) {
						gameOver = true;
						exit(1);
					}
					//if (x + i == ROWS - 3 && well.getGridValue(x + i + 1, y + j) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					//if (shape[i][j] == 5 && row == ROWS - 3 && well.getGridValue(row + 1, col) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					well.setGridValue(row, col, shape[i][j]);
				}
			}
		}


		
		vector<int> rowsToShift; // Store indices of rows to shift downwards
		// Check for filled rows and empty them
		for (int i = well.matrix.size() - 1; i >= 0; i--) {
			bool rowFilled = true;
			for (int j = 0; j < well.matrix[i].size(); j++) {
				if (well.matrix[i][j] == 0) {
					rowFilled = false;
					break;
				}
			}
			if (rowFilled) {
				// Empty the row
				for (int j = 0; j < well.matrix[i].size(); j++) {
					well.setGridValue(i, j, 0);
				}
				score += 100;
				lines += 1;
				rowsToShift.push_back(i); // Add the index of the cleared row to the rowsToShift vector
			}
		}

		// Shift the rows above the cleared row downwards
		for (int i = rowsToShift.size() - 1; i >= 0; i--) {
			int curRow = rowsToShift[i];
			for (int j = 0; j < well.matrix[curRow].size(); j++) {
				for (int k = curRow - 1; k >= 0; k--) {
					well.setGridValue(k + 1, j, well.matrix[k][j]);
					well.setGridValue(k, j, 0);
				}
			}
		}


		// Update the sprite's position to match the locked tetromino's position
		/*sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);*/

		cout << endl << endl;
		//cout << canMoveDown(well) ? "true" : "false\n";
		for (int i = 0; i < well.matrix.size(); i++) {
			for (int j = 0; j < well.matrix[i].size(); j++) {
				cout << well.matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << "score = " << score << " lines = " << lines << endl << endl;

		// Signal that the tetromino has locked
		locked = true;
	}

	void checkGameOver(Matrix& well) {
		for (int j = 0; j < well.matrix[0].size(); j++) {
			if (well.matrix[0][j] != 0) {
				gameOver = true;
				break;
			}
		}
	}


	Sprite& getSprite() {
		return sprite;
	}
	Texture getTexture() {
		return texture;
	}
	void setPosition(float starting_x, float starting_y) {
		x = starting_x;
		y = starting_y;
	}
	void setRotation() {
		shape = { { 0, 5, 5},
				  { 5, 5, 0},
				  { 0, 0, 0}
		};
	}
	void reset() {
		x = -1;
		y = COLUMNS / 2 - 2;
		shape = initialShape;
		locked = false;
	}
};

class T_Tetrimino : public Tetrimino {
protected:
	int x;
	int y;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	vector<vector <int>> shape;
	Sprite sprite;
	Color color;

	T_Tetrimino() {
		shape = { { 6, 6, 6},
				  { 0, 6, 0},
				  { 0, 0, 0}
		};
		x = -1;
		y = COLUMNS / 2 - 2;
		initialShape = shape;
		color = Color::Magenta;
		locked = false;
		lines_removed = 0;
		gameOver = false;
	}

	Color getColor() {
		this->color = Color::Magenta;
		return this->color;
	}

	vector<vector<int>> getShape() const {
		return shape;
	}

	void setShape(vector<vector<int>> shape) {
		this->shape = shape;
	}

	//void rotate(Matrix& well) {
	//	vector<vector<int>> rotated(shape[0].size(), vector<int>(shape.size(), 0));

	//	for (int i = 0; i < rotated.size(); i++) {
	//		for (int j = 0; j < rotated[0].size(); j++) {
	//			if (y + rotated[0].size() - j - 1 >= COLUMNS || y + rotated[0].size() - j - 1 < 0 || x + i >= ROWS || x + i < 0) {
	//				// The rotation is out of bounds
	//				setShape(shape);
	//				return;
	//			}
	//			if (well.getGrid()[x + i][y + rotated[0].size() - j - 1] != 0) {
	//				// The rotation collides with another block
	//				setShape(shape);
	//				return;
	//			}
	//			rotated[i][j] = shape[shape.size() - j - 1][i];
	//		}
	//	}
	//	setShape(rotated);
	//}

	void rotate(Matrix& well) {
		vector<vector<int>> rotated(shape[0].size(), vector<int>(shape.size(), 0));

		// Determine the target shape based on the current shape
		if (shape == vector<vector<int>>{ {6, 6, 6},
			{ 0, 6, 0 },
			{ 0, 0, 0 }}) {
			rotated = { {0, 6, 0},
						{6, 6, 0},
						{0, 6, 0} };
		}
		else if (shape == vector<vector<int>>{ {0, 6, 0},
			{ 6, 6, 0 },
			{ 0, 6, 0 }}) {
			rotated = { {0, 6, 0},
					   {6, 6, 6},
					   {0, 0, 0} };
		}
		else if (shape == vector<vector<int>>{ {0, 6, 0},
			{ 6, 6, 6 },
			{ 0, 0, 0 }}) {
			rotated = { {0, 6, 0},
					   {0, 6, 6},
					   {0, 6, 0} };
		}
		else if (shape == vector<vector<int>>{ {0, 6, 0},
			{ 0, 6, 6 },
			{ 0, 6, 0 }}) {
			rotated = { {6, 6, 6},
					   {0, 6, 0},
					   {0, 0, 0} };
		}

		// Check if the target shape is valid
		int new_x = x;
		int new_y = y;
		for (int i = 0; i < rotated.size(); i++) {
			for (int j = 0; j < rotated[0].size(); j++) {
				int row = i + x;
				int col = j + y;
				if (rotated[i][j] != 0) {
					if (col < 0) {
						new_y++;
					}
					else if (col >= COLUMNS) {
						new_y--;
					}
					else if (row >= ROWS || well.getGrid()[row][col] != 0) {
						// The rotation collides with another block or goes out of bounds
						setShape(shape);
						return;
					}
				}
			}
		}

		// Set the new shape and position
		setShape(rotated);
		x = new_x;
		y = new_y;
	}

	void draw(RenderWindow& window) {
		RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

		Color Orange(255, 165, 0);
		block.setFillColor(Color::Magenta);

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 6) {
					try {
						block.setPosition(TETRIMINO_APPEAR_X + (y + j) * CELL_SIZE + 1, TETRIMINO_APPEAR_Y + (x + i) * CELL_SIZE + 1);
						window.draw(block);
					}
					catch (std::out_of_range& e) {
						// Handle the exception here
						// For example, you could print an error message or log the error
						cerr << "Exception caught: " << e.what() << endl;
					}
				}
			}
		}
	}

	void moveLeft(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 6) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the left
		y--;
	}

	void moveRight(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 6) {
					// Check if there is a block to the right
					if (y + j == COLUMNS - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the right
		y++;
	}



	void moveDown(Matrix& well, RenderWindow& window) {
		// Check if the tetrimino can move down
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 6) {
					// Check if bottom is reached
					if (x + i == ROWS - 1) {
						return;
					}
					// Check if there's a block below
					if (well.getGrid()[x + i + 1][y + j] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino down
		x++;
	}

	bool canMoveLeftRight(Matrix& well) {
		cout << shape[0][0] << endl;
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return false;
					}
				}
			}
		}

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == well.getNumColumns() - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return false;
					}
				}
			}
		}

		return true;
	}


	bool canMoveDown(Matrix& well) {
		if (locked) {
			return false;
		}
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {

				if (shape[i][j] != 0 && shape[i][j] == 6) {
					// Check if there's a block belown or matrix en
					if (x + i + 1 >= ROWS || well.getGrid()[x + i + 1][y + j] != 0) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) {
		if (locked) {
			return;
		}

		// Update the well with the tetromino's shape
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[i].size(); j++) {
				if (shape[i][j] != 0) {
					int row = x + i;
					int col = y + j;
					if (row >= ROWS) {
						// Do not update if the tetromino is partially filling the last row
						continue;
					}
					if (col < 0 || col >= COLUMNS || well.getGridValue(row, col) != 0) {
						gameOver = true;
						exit(1);
					}
					//if (x + i == ROWS - 3 && well.getGridValue(x + i + 1, y + j) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					//if (shape[i][j] == 6 && row == ROWS - 3 && well.getGridValue(row + 1, col) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					well.setGridValue(row, col, shape[i][j]);
				}
			}
		}


		

		vector<int> rowsToShift; // Store indices of rows to shift downwards
		// Check for filled rows and empty them
		for (int i = well.matrix.size() - 1; i >= 0; i--) {
			bool rowFilled = true;
			for (int j = 0; j < well.matrix[i].size(); j++) {
				if (well.matrix[i][j] == 0) {
					rowFilled = false;
					break;
				}
			}
			if (rowFilled) {
				// Empty the row
				for (int j = 0; j < well.matrix[i].size(); j++) {
					well.setGridValue(i, j, 0);
				}
				score += 100;
				lines += 1;
				rowsToShift.push_back(i); // Add the index of the cleared row to the rowsToShift vector
			}
		}

		// Shift the rows above the cleared row downwards
		for (int i = rowsToShift.size() - 1; i >= 0; i--) {
			int curRow = rowsToShift[i];
			for (int j = 0; j < well.matrix[curRow].size(); j++) {
				for (int k = curRow - 1; k >= 0; k--) {
					well.setGridValue(k + 1, j, well.matrix[k][j]);
					well.setGridValue(k, j, 0);
				}
			}
		}


		// Update the sprite's position to match the locked tetromino's position
		/*sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);*/

		cout << endl << endl;
		//cout << canMoveDown(well) ? "true" : "false\n";
		for (int i = 0; i < well.matrix.size(); i++) {
			for (int j = 0; j < well.matrix[i].size(); j++) {
				cout << well.matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << "score = " << score << " lines = " << lines << endl << endl;

		// Signal that the tetromino has locked
		locked = true;
	}

	void checkGameOver(Matrix& well) {
		for (int j = 0; j < well.matrix[0].size(); j++) {
			if (well.matrix[0][j] != 0) {
				gameOver = true;
				break;
			}
		}
	}


	Sprite& getSprite() {
		return sprite;
	}
	Texture getTexture() {
		return texture;
	}
	void setPosition(float starting_x, float starting_y) {
		x = starting_x;
		y = starting_y;
	}
	void setRotation() {
		shape = { { 6, 6, 6},
				  { 0, 6, 0},
				  { 0, 0, 0}
		};
	}
	void reset() {
		x = -1;
		y = COLUMNS / 2 - 2;
		shape = initialShape;
		locked = false;
	}
};

class Z_Tetrimino : public Tetrimino {
protected:
	int x;
	int y;
	int rotation_no = 0;
public:
	bool locked;
	bool gameOver;
	unsigned int lines_removed;
	vector<vector <int>> shape;
	Sprite sprite;
	Color color;

	Z_Tetrimino() {
		shape = { { 7, 7, 0 },
				  { 0, 7, 7 },
				  { 0, 0, 0 }
		};
		x = -1;
		y = COLUMNS / 2 - 2;
		initialShape = shape;
		color = Color::Red;
		locked = false;
		lines_removed = 0;
		gameOver = false;
	}

	Color getColor() {
		this->color = Color::Red;
		return this->color;
	}

	vector<vector<int>> getShape() const {
		return shape;
	}

	void setShape(vector<vector<int>> shape) {
		this->shape = shape;
	}

	void rotate(Matrix& well) {
		// Define the rotation matrix
		vector<vector<int>> rotation_matrix{ {0, -1}, {1, 0} };

		// Apply the rotation matrix to each block in the tetromino shape
		vector<vector<int>> rotated(shape[0].size(), vector<int>(shape.size(), 0));
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				int new_i = j;
				int new_j = shape.size() - 1 - i;
				rotated[new_i][new_j] = shape[i][j];
			}
		}

		// Check if the target shape is valid
		int new_x = x;
		int new_y = y;
		for (int i = 0; i < rotated.size(); i++) {
			for (int j = 0; j < rotated[0].size(); j++) {
				int row = i + x;
				int col = j + y;
				if (rotated[i][j] != 0) {
					if (col < 0) {
						new_y++;
					}
					else if (col >= COLUMNS) {
						new_y--;
					}
					else if (row >= ROWS || well.getGrid()[row][col] != 0) {
						// The rotation collides with another block or goes out of bounds
						setShape(shape);
						return;
					}
				}
			}
		}

		// Set the new shape and position
		setShape(rotated);
		x = new_x;
		y = new_y;
	}


	void draw(RenderWindow& window) {
		RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

		Color Orange(255, 165, 0);
		block.setFillColor(Color::Red);

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 7) {
					try {
						block.setPosition(TETRIMINO_APPEAR_X + (y + j) * CELL_SIZE + 1, TETRIMINO_APPEAR_Y + (x + i) * CELL_SIZE + 1);
						window.draw(block);
					}
					catch (std::out_of_range& e) {
						// Handle the exception here
						// For example, you could print an error message or log the error
						cerr << "Exception caught: " << e.what() << endl;
					}
				}
			}
		}
	}

	void moveLeft(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 7) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the left
		y--;
	}

	void moveRight(Matrix& well, RenderWindow& window) {
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 7) {
					// Check if there is a block to the right
					if (y + j == COLUMNS - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino to the right
		y++;
	}



	void moveDown(Matrix& well, RenderWindow& window) {
		// Check if the tetrimino can move down
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] != 0 && shape[i][j] == 7) {
					// Check if bottom is reached
					if (x + i == ROWS - 1) {
						return;
					}
					// Check if there's a block below
					if (well.getGrid()[x + i + 1][y + j] != 0) {
						return;
					}
				}
			}
		}
		// Move the tetrimino down
		x++;
	}

	bool canMoveLeftRight(Matrix& well) {
		cout << shape[0][0] << endl;

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the left
					if (y + j == 0 || well.getGrid()[x + i][y + j - 1] != 0) {
						return false;
					}
				}
			}
		}

		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {
				if (shape[i][j] == 1) {
					// Check if there is a block to the right
					if (y + j == well.getNumColumns() - 1 || well.getGrid()[x + i][y + j + 1] != 0) {
						return false;
					}
				}
			}
		}

		return true;
	}


	bool canMoveDown(Matrix& well) {
		if (locked) {
			return false;
		}
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[0].size(); j++) {

				if (shape[i][j] != 0 && shape[i][j] == 7) {
					// Check if there's a block belown or matrix en
					if (x + i + 1 >= ROWS || well.getGrid()[x + i + 1][y + j] != 0) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void lock(Matrix& well, Sprite& sprite, int& score, int& lines, RenderWindow& window) {
		if (locked) {
			return;
		}

		// Update the well with the tetromino's shape
		for (int i = 0; i < shape.size(); i++) {
			for (int j = 0; j < shape[i].size(); j++) {
				if (shape[i][j] != 0) {
					int row = x + i;
					int col = y + j;
					if (row >= ROWS) {
						// Do not update if the tetromino is partially filling the last row
						continue;
					}
					if (col < 0 || col >= COLUMNS || well.getGridValue(row, col) != 0) {
						gameOver = true;
						exit(1);
					}
					//if (x + i == ROWS - 3 && well.getGridValue(x + i + 1, y + j) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					//if (shape[i][j] == 7 && row == ROWS - 3 && well.getGridValue(row + 1, col) != 0) {
					//	gameOver = true;
					//	exit(1);
					//}
					well.setGridValue(row, col, shape[i][j]);
				}
			}
		}


		vector<int> rowsToShift; // Store indices of rows to shift downwards
		// Check for filled rows and empty them
		for (int i = well.matrix.size() - 1; i >= 0; i--) {
			bool rowFilled = true;
			for (int j = 0; j < well.matrix[i].size(); j++) {
				if (well.matrix[i][j] == 0) {
					rowFilled = false;
					break;
				}
			}
			if (rowFilled) {
				// Empty the row
				for (int j = 0; j < well.matrix[i].size(); j++) {
					well.setGridValue(i, j, 0);
				}
				score += 100;
				lines += 1;
				rowsToShift.push_back(i); // Add the index of the cleared row to the rowsToShift vector
			}
		}

		// Shift the rows above the cleared row downwards
		for (int i = rowsToShift.size() - 1; i >= 0; i--) {
			int curRow = rowsToShift[i];
			for (int j = 0; j < well.matrix[curRow].size(); j++) {
				for (int k = curRow - 1; k >= 0; k--) {
					well.setGridValue(k + 1, j, well.matrix[k][j]);
					well.setGridValue(k, j, 0);
				}
			}
		}



		// Update the sprite's position to match the locked tetromino's position
		/*sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);*/

		cout << endl << endl;
		//cout << canMoveDown(well) ? "true" : "false\n";
		for (int i = 0; i < well.matrix.size(); i++) {
			for (int j = 0; j < well.matrix[i].size(); j++) {
				cout << well.matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << "score = " << score << " lines = " << lines << endl << endl;

		// Signal that the tetromino has locked
		locked = true;
	}

	void checkGameOver(Matrix& well) {
		for (int j = 0; j < well.matrix[0].size(); j++) {
			if (well.matrix[0][j] != 0) {
				gameOver = true;
				break;
			}
		}
	}


	Sprite& getSprite() {
		return sprite;
	}
	Texture getTexture() {
		return texture;
	}
	void setPosition(float starting_x, float starting_y) {
		x = starting_x;
		y = starting_y;
	}
	void setRotation() {
		shape = { { 7, 7, 0 },
				  { 0, 7, 7 },
				  { 0, 0, 0 }
		};
	}
	void reset() {
		x = -1;
		y = COLUMNS / 2 - 2;
		shape = initialShape;
		locked = false;
	}
};

#endif // TETRIMINOS_H