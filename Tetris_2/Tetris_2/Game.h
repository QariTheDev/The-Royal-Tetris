#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Global.h"
#include "Tetriminos.h"
#include "well.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <thread>

using namespace sf;

class Game {

    //For Main Game
    RenderWindow window;
    Texture home_screen, startButtonTexture;
    Music mainMenuMusic;
    Sprite sprite;
    Sprite startButton;
    bool GameOver;
    bool LevelDone;
    vector <int> scores;
    vector <string> names;

    //For Game Screen
    int level;
    int score;
    int lines;
    Music InGameMusic;
    Texture game_screen;
    Sprite spr;
    Text LevelText, PlayerNameText, ScoreText, LinesText;
    Font LeveltextFont;
    vector<int> matrix;
    string background[8];
    string music[8];

    //For Enter Name Screen
    Texture EnterNameScreen;
    Sprite temp;
    Font font;
    Text inputName;
    string name;

    //For Stats Screen
    Texture StatsScreenTexture;
    Texture StatsButtonTexture;
    Sprite StatsScreenSprite;
    Sprite StatsButtonSprite;
    Music StatsScreenMusic;

    //For Pause Screen
    Texture pauseScreenTexture;
    Sprite pauseScreenSprite;
    Texture ResumeButtonTexture;
    Sprite ResumeButtonSprite;
    Texture ExitButtonTexture;
    Sprite ExitButtonSprite;


    //For GameOver Screen
    Texture GameOverScreen;
    Sprite GameOverSprite;

    //Matrix
    Matrix well;

public:
    Game() {
        window.create(VideoMode(800, 600), "The Royal Tetris");

        if (!home_screen.loadFromFile("home_screen.png") || !startButtonTexture.loadFromFile("start_button.png") || !mainMenuMusic.openFromFile("main_menu_song.wav")) {
            return;
        }

        GameOver = false;
        LevelDone = false;

        startButton.setTexture(startButtonTexture);
        /*sprite.setTexture(home_screen);*/

        //set position of sprite to center
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        //set position of sprite to center of the window
        sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        //resizing start button
        startButton.setScale(0.6f, 0.6f);
        //set position of start button
        startButton.setPosition(window.getSize().x / 2 - startButton.getGlobalBounds().width / 2, 330);
        //take size of start button
        Vector2u buttonSize = startButtonTexture.getSize();
        //making interactive
        FloatRect buttonRect = startButton.getGlobalBounds();

        //For Game Screen
        level = 1;
        score = 0;
        lines = 0;

        //select random game background pic
        srand(time(NULL));


        background[0] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/game_screen.png";
        background[1] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stages/space.png";
        background[2] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stages/haunted.png";
        background[3] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stages/egypt.png";
        background[4] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stages/underwater.png";
        background[5] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stages/futureRuin.jpg";
        background[6] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stages/beach.png";
        background[7] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stages/volcanic.png";

        music[0] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/LostCityMusic.wav";
        music[1] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/SpaceMusic.wav";
        music[2] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/HauntedMusic.wav";
        music[3] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/EgyptianMusic.wav";
        music[4] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/LostCityMusic.wav";
        music[5] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/FallenTech.wav";
        music[6] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/BeachMusic.wav";
        music[7] = "C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/Volcanic.wav";

        int b_index = rand() % 8; //temporarily


        if (!LeveltextFont.loadFromFile("valorant.ttf")) {
            std::cout << "Failed";
        }


        //PlayerName Text
        FloatRect textBounds = PlayerNameText.getGlobalBounds();
        float textX = -30 + (162 - textBounds.width) / 2;
        float textY = 127 + (128 - textBounds.height) / 2;
        PlayerNameText.setPosition(textX, textY);

        PlayerNameText.setFont(LeveltextFont);
        PlayerNameText.setCharacterSize(21);
        PlayerNameText.setOrigin(PlayerNameText.getLocalBounds().width / 2, PlayerNameText.getLocalBounds().height / 2);
        PlayerNameText.setPosition(textX, textY);
        PlayerNameText.setFillColor(Color::White);



        //For EnterNameScreen

        if (!EnterNameScreen.loadFromFile("C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/enter_name.png") || !font.loadFromFile("Valorant.ttf")) {
            std::cout << "Failed";
        }

        inputName.setFont(font);
        inputName.setCharacterSize(24);
        inputName.setFillColor(Color::White);
        inputName.setPosition(300, 420);

        temp.setTexture(EnterNameScreen);


        //For Stats Screen

        if (!StatsScreenTexture.loadFromFile("C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stats_screen.png") || !StatsButtonTexture.loadFromFile("C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/stats_button.png") || !StatsScreenMusic.openFromFile("C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Music/statsScreenMusic.wav")) {
            std::cout << "Failed";
        }

        StatsButtonSprite.setTexture(StatsButtonTexture);
        StatsScreenSprite.setTexture(StatsScreenTexture);

        Vector2u StatsbuttonSize = StatsButtonTexture.getSize();
        //making interactive
        FloatRect StatsbuttonRect = StatsButtonSprite.getGlobalBounds();

    }


    void enterNameScreen(Music& mainMenuMusic);
    void statsScreen();
    void GameScreen(std::string PlayerName);
    void pauseScreen();
    void saveScore(int& score, string& name);
    void checkGameOVER(Matrix& well);
    void GameOverSCREEN();
    void run();

    void UpdateLvlText() {
        LevelText.setString(std::to_string(level));
    }
    void setLevel() {
        level += 1;
        UpdateLvlText();
    }
    void setScore() {
        score += 100;
    }
    void setLines() {
        lines += 1;
    }
};


void Game::GameScreen(std::string PlayerName) {
    Clock clock;
    int count = 0;

    if (!InGameMusic.openFromFile(music[7]) || !game_screen.loadFromFile(background[7])) {
        std::cout << "Failed!!!";
    }

    //ALL TETRIMINO SHAPES
    I_Tetrimino i;
    J_Tetrimino j;
    L_Tetrimino l;
    O_Tetrimino o;
    S_Tetrimino s;
    T_Tetrimino t;
    Z_Tetrimino z;

    vector<Tetrimino*> tetriminos = { &i, &j, &l, &o, &s, &t, &z };
    level = 1;
    score = 0;
    lines = 0;

    //Level Text
    LevelText.setFont(LeveltextFont);
    LevelText.setCharacterSize(31);
    LevelText.setPosition(120, 350);
    LevelText.setFillColor(Color::White);

    //Score Text
    ScoreText.setFont(LeveltextFont);
    ScoreText.setCharacterSize(31);
    ScoreText.setPosition(685, 191);
    ScoreText.setFillColor(Color::White);

    //Lines Text
    LinesText.setFont(LeveltextFont);
    LinesText.setCharacterSize(31);
    LinesText.setPosition(685, 350);
    LinesText.setFillColor(Color::White);


    InGameMusic.setLoop(true);
    InGameMusic.play();

    //randomization
    srand(time(NULL));


    /* vector of all tetriminos*/
    int tetr_index = rand() % tetriminos.size();
    Tetrimino* currentTetromino = tetriminos[tetr_index];

    ScoreText.setString(std::to_string(score));
    LinesText.setString(std::to_string(lines));
    LevelText.setString(std::to_string(level));


    PlayerNameText.setString(PlayerName);
    bool needNewTetromino = true;
    int tetrominosFallen = 0;
    bool check = false;

    //game screen loop
    try {
        while (window.isOpen()) {

            while (count < 8) {

                float deltaTime = clock.getElapsedTime().asSeconds();

                if (needNewTetromino) {
                    tetr_index = rand() % tetriminos.size();
                    currentTetromino = tetriminos[tetr_index];
                    needNewTetromino = false;
                }

                if (!(currentTetromino->locked)) {
                    if (deltaTime >= START_FALL_SPEED_ARRAY[7]) {
                        if (currentTetromino->canMoveDown(well) && !currentTetromino->locked) {
                            currentTetromino->moveDown(well, window);
                            clock.restart();
                        }
                        else {
                            currentTetromino->lock(well, currentTetromino->getSprite(), score, lines, window);
                            ScoreText.setString(std::to_string(score));
                            LinesText.setString(std::to_string(lines));
                            LevelText.setString(std::to_string(level));

                            checkGameOVER(well);

                            if (GameOver) {
                                GameOverSCREEN();
                            }

                            currentTetromino->reset();

                            cout << "lock called";
                            cout << "tetrimino " << tetrominosFallen++;

                            needNewTetromino = true;
                        }
                    }
                }
                else {
                    needNewTetromino = true;
                }


                Event event;
                while (window.pollEvent(event)) {

                    if (event.type == Event::Closed) {
                        window.close();
                    }

                    if (event.type == Event::KeyPressed) {
                        if (event.key.code == Keyboard::S || event.key.code == Keyboard::Down) {
                            if (currentTetromino->canMoveDown(well)) {
                                currentTetromino->moveDown(well, window);
                            }
                        }
                        if (event.key.code == Keyboard::W || event.key.code == Keyboard::Up) {
                            currentTetromino->rotate(well);
                        }
                        if (event.key.code == Keyboard::A || event.key.code == Keyboard::Left) {
                            if ((currentTetromino->canMoveDown(well))/* && currentTetromino->canMoveLeftRight(well)*/) {
                                currentTetromino->moveLeft(well, window);
                            }
                        }
                        if (event.key.code == Keyboard::D || event.key.code == Keyboard::Right) {
                            if (currentTetromino->canMoveDown(well)/* && currentTetromino->canMoveLeftRight(well)*/) {
                                currentTetromino->moveRight(well, window);
                            }
                        }
                        if (event.key.code == Keyboard::Escape) {
                            InGameMusic.pause();
                            pauseScreen();
                            InGameMusic.play();
                        }

                    }

                }


                //clear window
                window.clear();

                // Draw the sprites to the window
                spr.setTexture(game_screen);
                window.draw(spr);
                window.draw(LevelText);
                window.draw(PlayerNameText);
                window.draw(ScoreText);
                window.draw(LinesText);



                // Draw the grid lines
                well.draw(window);

                // Draw the Tetriminos
                currentTetromino->draw(window);

                vector<RectangleShape> block_arr;
                RectangleShape block(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));




                // Check for locked tetrominos and draw them separately
                for (int i = 0; i < well.matrix.size(); i++) {
                    for (int j = 0; j < well.matrix[i].size(); j++) {
                        if (well.matrix[i][j] != 0) {

                            Color current_color;
                            switch (well.matrix[i][j]) {
                            case 1: current_color = Color::Cyan; break;
                            case 2: current_color = darkBlue; break;
                            case 3: current_color = Orange; break;
                            case 4: current_color = Color::Yellow; break;
                            case 5: current_color = Color::Green; break;
                            case 6: current_color = Color::Magenta; break;
                            case 7: current_color = Color::Red; break;
                            }

                            block.setFillColor(current_color);
                            block.setPosition(CELL_POS_X + (j * CELL_SIZE), CELL_POS_Y + (i * CELL_SIZE));
                            block_arr.push_back(block);
                        }
                    }
                }

                for (int i = 0; i < block_arr.size(); i++) {
                    window.draw(block_arr[i]);
                }

                //display window
                window.display();

                if (lines >= target && level < 8) {
                    count++;
                    check = false;
                    lines = 0;
                    level += 1;
                    LevelText.setString(std::to_string(level));
                    LinesText.setString(std::to_string(lines));


                    //empty matrix
                    for (int i = 0; i < ROWS; i++) {
                        for (int j = 0; j < COLUMNS; j++) {
                            well.matrix[i][j] = 0;
                        }
                    }

                    if (!InGameMusic.openFromFile(music[count])) {
                        std::cout << "Failed to load music file!!!";
                        return;
                    }

                    if (!game_screen.loadFromFile(background[count])) {
                        std::cout << "Failed to load game screen file!!!";
                        return;
                    }

                    InGameMusic.play();

                    clock.restart();

                }

                if (level >= 8 && lines >= target) {
                    count = 0;
                    check = false;
                    lines = 0;
                    level = 1;

                    //empty matrix
                    for (int i = 0; i < ROWS; i++) {
                        for (int j = 0; j < COLUMNS; j++) {
                            well.matrix[i][j] = 0;
                        }
                    }

                    if (!InGameMusic.openFromFile(music[count])) {
                        std::cout << "Failed to load music file!!!";
                        return;
                    }

                    if (!game_screen.loadFromFile(background[count])) {
                        std::cout << "Failed to load game screen file!!!";
                        return;
                    }

                    InGameMusic.play();

                    clock.restart();
                }
            }

        }
    }
    catch (const std::out_of_range& e) {
        cerr << "GAME OVER\n";
        GameOverSCREEN();
        saveScore(score, name);
    }

}

void Game::statsScreen() {
    window.clear();

    StatsScreenMusic.setLoop(1);
    StatsScreenMusic.play();

    //finding center and radius of the circular area
    Vector2f center = { 52, 516 };
    float radius = 44;

    //extracting data from file
    vector<string> fileContents = readFromFile("top_5_list.txt");

    Text text("", font, 30);
    text.setFillColor(Color::White);
    text.setPosition(100, 100);
    text.setOutlineThickness(1.2);
    Color DarkGolden(145, 106, 29);
    text.setOutlineColor(DarkGolden);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                Vector2i mousePosition = sf::Mouse::getPosition(window);
                float distance = std::sqrt(std::pow(mousePosition.x - center.x, 2) + std::pow(mousePosition.y - center.y, 2));

                if (distance <= radius) {
                    StatsScreenMusic.stop();
                    Game::run();
                }
            }

        }

        window.clear();
        window.draw(StatsScreenSprite);

        for (int i = 0; i < fileContents.size(); i++) {
            text.setString(fileContents[i]);
            text.setPosition(250, 130 + i * 35);
            window.draw(text);
        }
        window.display();
    }
}

void Game::enterNameScreen(Music& mainMenuMusic) {
    window.clear();

    std::string inputString;

    //main game loop

    try {
        while (window.isOpen()) {

            Event event;
            while (window.pollEvent(event)) {

                if (event.type == Event::Closed) {
                    window.close();
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        return;
                    }
                    else if (event.key.code == Keyboard::BackSpace && inputString.size() > 0) {
                        inputString.erase(inputString.size() - 1);
                        inputName.setString(inputString);
                    }
                    else if (event.key.code == Keyboard::Enter) {
                        /*saveNameToFile(inputString);*/
                        name = inputString;
                        mainMenuMusic.stop();

                        Game::GameScreen(inputString);
                        return;
                    }
                }
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        inputString += static_cast <char> (event.text.unicode);
                        inputName.setString(inputString);
                    }
                }
            }

            window.clear();
            window.draw(temp);
            window.draw(inputName);
            window.display();
        }
    }
    catch(const std::out_of_range &e) {
        cerr << "Error !!!" << e.what() << endl;
    }
}

void Game::pauseScreen() {

    window.clear();

    if (!pauseScreenTexture.loadFromFile("C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/pauseScreen.png") || !ResumeButtonTexture.loadFromFile("C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/resumeButton.png")) {
        std::cout << "Failed";
    }

    pauseScreenSprite.setTexture(pauseScreenTexture);
    pauseScreenSprite.setPosition((window.getSize().x - pauseScreenSprite.getGlobalBounds().width) / 2, (window.getSize().y - pauseScreenSprite.getGlobalBounds().height) / 2);

    ResumeButtonSprite.setTexture(ResumeButtonTexture);
    ResumeButtonSprite.setScale(0.4f, 0.4f);
    ResumeButtonSprite.setPosition(12, 143);

    //taking size
    Vector2u ResumebuttonSize = ResumeButtonTexture.getSize();

    //making interactive
    FloatRect ResumebuttonRect = ResumeButtonSprite.getGlobalBounds();


    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (ResumebuttonRect.contains(mousePosF))
                {
                    // Button was clicked
                    std::cout << "Resume Button clicked!" << std::endl;

                    return;

                }
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    return;
                }
            }

        }

        window.clear();
        window.draw(pauseScreenSprite);
        window.draw(ResumeButtonSprite);
        window.display();
    }

}

void Game::run() {
    window.clear();

    mainMenuMusic.setLoop(true);
    mainMenuMusic.play();

    Sprite sprite;
    Sprite startButton(startButtonTexture);
    Sprite statsButtonSprite(StatsButtonTexture);

    sprite.setTexture(home_screen);

    //set position of sprite to center
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    //set position of sprite to center of the window
    sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    //resizing start button and stats button
    startButton.setScale(0.6f, 0.6f);
    statsButtonSprite.setScale(0.5f, 0.5f);
    //set position of start button & stats button
    startButton.setPosition(window.getSize().x / 2 - startButton.getGlobalBounds().width / 2, 330);
    statsButtonSprite.setPosition(664, 55);
    //take size of start button & stats button
    Vector2u buttonSize = startButtonTexture.getSize();
    Vector2u StatsbuttonSize = StatsButtonTexture.getSize();
    //making interactive
    FloatRect buttonRect = startButton.getGlobalBounds();
    FloatRect StatsbuttonRect = statsButtonSprite.getGlobalBounds();

    bool buttonClicked = false;
    bool statsButtonClicked = false;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (buttonRect.contains(mousePosF))
                {
                    buttonClicked = true;
                    // Button was clicked, start game or perform desired action
                    std::cout << "Start Button clicked!" << std::endl;

                    //new screen (enter name screen)
                    Game::enterNameScreen(mainMenuMusic);

                }

                if (StatsbuttonRect.contains(mousePosF))
                {
                    statsButtonClicked = true;
                    // Button was clicked, start game or perform desired action
                    std::cout << "Stats Button clicked!" << std::endl;

                    //new screen (enter name screen)
                    mainMenuMusic.stop();
                    Game::statsScreen();

                }
            }

        }

        window.clear();
        window.draw(sprite);
        window.draw(startButton);
        window.draw(statsButtonSprite);
        window.display();
    }
}

void Game::checkGameOVER(Matrix& well){

    for (int j = 0; j < well.matrix[0].size(); j++) {
        if (well.matrix[0][j] != 0) {
            GameOver = true;
            break;
        }
    }
    // Check if the top two rows are filled
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < well.matrix[i].size(); j++) {
            if (well.matrix[i][j] != 0) {
                GameOver = true;
                break;
            }
        }
        if (GameOver) {
            break;
        }
    }
}

void Game::GameOverSCREEN() {

    if (GameOver) {

        window.clear();
        InGameMusic.stop();

        Texture GameOverScreen;
        if (!GameOverScreen.loadFromFile("C:/Users/Moeez/Desktop/University/FINAL YEAR PROJECT/OOP Project/Tetris/Images/GameOver.png")) {
            std::cerr << "Failed to load game over screen image\n";
            return;
        }

        Sprite GameOverSprite(GameOverScreen);

        std::string message = "Your score: " + std::to_string(score);
        Text GameOverText(message, font, 25);
        GameOverText.setFillColor(Color::Black);
        GameOverText.setPosition(563, 104);

        saveScore(score, name);

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                    exit(1);
                }
                else if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        window.close();
                        exit(1);
                    }
                }
            }

            window.clear();
            window.draw(GameOverSprite);
            window.draw(GameOverText);
            window.display();

            std::this_thread::sleep_for(std::chrono::seconds(15));
        }
    }
}

void Game::saveScore(int& score, string& name) {
    try {
        // Load scores and names from file
        vector<int> scores;
        vector<string> names;
        ifstream fin("top_5_list.txt");
        string line;
        while (getline(fin, line)) {
            istringstream iss(line);
            string name;
            int score;
            if (iss >> name >> score) {
                names.push_back(name);
                scores.push_back(score);
            }
        }
        fin.close();

        // Update score if player with same name already exists
        bool nameExists = false;
        for (int i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                scores[i] = max(scores[i], score);
                nameExists = true;
                break;
            }
        }

        // Add new score and name if it does not already exist
        if (!nameExists) {
            names.push_back(name);
            scores.push_back(score);
        }

        // Sort scores and names in descending order
        vector<pair<int, string>> scores_and_names;
        for (int i = 0; i < scores.size(); i++) {
            scores_and_names.push_back(make_pair(scores[i], names[i]));
        }
        sort(scores_and_names.rbegin(), scores_and_names.rend());

        // Truncate vectors to keep only top 5 scores and names
        scores_and_names.resize(min((size_t)5, scores_and_names.size()));

        // Write top 5 scores and names to file
        ofstream fout("top_5_list.txt");
        if (fout.is_open()) {
            for (auto& score_and_name : scores_and_names) {
                fout << score_and_name.second << "\t\t\t" << score_and_name.first << endl;
            }
            fout.close();
        }
        else {
            throw runtime_error("Could not open file for writing");
        }
    }
    catch (const exception& e) {
        cerr << "Exception caught: " << e.what() << endl;
    }
}



#endif //GAME_H