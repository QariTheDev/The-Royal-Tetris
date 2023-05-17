#ifndef WELL_H
#define WELL_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Global.h"
#include "Tetriminos.h"
#include <vector>

using namespace sf;

class Matrix {
private:
    int rows;
    int cols;
    RectangleShape cell;
    RectangleShape border;
    vector<Sprite> sprites;
    Texture texture;

public:
    vector <vector <unsigned int>> matrix;
    Matrix() {
        this->rows = ROWS;
        this->cols = COLUMNS;
        this->matrix.resize(rows, vector<unsigned int>(cols, 0));
        this->cell.setSize(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        this->cell.setFillColor(Color::White);
        this->border.setSize(Vector2f(5 * CELL_SIZE, 5 * CELL_SIZE));
        this->border.setFillColor(Color(0, 0, 0));

    }

    vector<vector<unsigned int>>& getGrid() { return matrix; }

    int getGridValue(int row, int column) const {
        return this->matrix[row][column];
    }
    void setGridValue(int row, int col, unsigned int value) { this->matrix[row][col] = value; }
    Texture* getTexture() {
        return &texture;
    }
    //Sprite* getSpriteAt(float x, float y) {
    //    for (auto& sprite : sprites) {
    //        FloatRect bounds = sprite.getGlobalBounds();
    //        if (bounds.contains(x, y)) {
    //            return &sprite;
    //        }
    //    }
    //    return nullptr;
    //}




    //void addSprite(Sprite& sprite) {
    //    bool spriteExists = false;
    //    for (int i = 0; i < sprites.size(); i++) {
    //        if (sprites[i].getPosition().x == sprite.getPosition().x && sprites[i].getPosition().y == sprite.getPosition().y) {
    //            spriteExists = true;
    //            // Remove the old sprite
    //            sprites.erase(sprites.begin() + i);
    //            break;
    //        }
    //    }
    //    if (!spriteExists) {
    //        // Add the new sprite
    //        sprites.push_back(sprite);
    //    }
    //}

    int getNumColumns() const {
        return cols;
    }

    int getNumRows() const {
        return rows;
    }



    void draw(RenderWindow& window) {
        for (const auto& sprite : sprites) {
            window.draw(sprite);
        }
        for (int i = 0; i < this->cols; i++) {
            for (int j = 0; j < this->rows; j++) {
                this->cell.setPosition(CELL_POS_X + CELL_SIZE * i, CELL_POS_Y + CELL_SIZE * j);
                this->border.setOutlineThickness(0);
                this->border.setPosition(5 * CELL_POS_X + CELL_SIZE * i, 5 * CELL_POS_Y + CELL_SIZE * j);
                window.draw(this->cell);
                window.draw(this->border);
            }
        }
    }

};

#endif //WELL_H
