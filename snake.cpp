/*Features to Add


3. Obstacles:

Add walls or randomly placed obstacles to increase difficulty.


5. Customizations:

Allow the player to set the grid size or snake speed.*/

#include <cctype>
#include <fstream>
#include <ios>
#include <iostream>
#include <ncurses.h>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <string>

int game(bool&);
int highScorer(int&);
void levelCalculater(int&);

struct Point {
    int x, y;
};

int main() {
    //Initialize ncurses for terminal-based graphical interface.
    initscr();
    noecho(); //Disable input echo to hide characters typed by the user.
    cbreak(); //Disable line buffering to proccess input immediately withou waiting for enter.
    keypad(stdscr, TRUE); //Enable arrow key and special key handling.
    curs_set(0); //To hide the cursor.
    timeout(100); // Non-blocking input (100ms delay)
    
    int currentScore = 0; //to keep track of the current score.
    int highScore = 0; //to keeep track of the high score.

    bool game_over = false;
    
    // Repeat the game if 'p' is pressed; quit otherwise.
    while (!game_over) {
        currentScore = game(game_over);
        clear();
        mvprintw(0, 0, "Press p to play again and q to quit");
        highScore = highScorer(currentScore);
        mvprintw(1,1, "The highscore is: %d", highScore);
        refresh();
        char choice;
        std::cin>>choice;
        if (choice=='p') {
            game_over = false;
        }
    }
    
    endwin();
    return 0;
}
    
int game(bool &game_over){
    // Set up game variables
    int max_x, max_y, currentScore;
    getmaxyx(stdscr, max_y, max_x); //Get window dimensions.
    std::deque<Point> snake = {{max_y / 2, max_x / 2}}; // Snake starts at the center
    Point food = {rand() % max_y, rand() % max_x};
    int dir_x = 1, dir_y = 0; // Initial direction (right)

    while (!game_over) {
        //Initializes level to track the speed.
        int level = 200;
        timeout(level);
        int ch = getch();
        // Allow movement, preventing reverse direction.
        switch (ch) {
            case KEY_UP:    if (dir_y != 1) { dir_y = -1; dir_x = 0; } break;
            case KEY_DOWN:  if (dir_y != -1) { dir_y = 1; dir_x = 0; } break;
            case KEY_LEFT:  if (dir_x != 1) { dir_x = -1; dir_y = 0; } break;
            case KEY_RIGHT: if (dir_x != -1) { dir_x = 1; dir_y = 0; } break;
        }

        // Move snake
        Point new_head = {snake.front().x + dir_y, snake.front().y + dir_x};
        snake.push_front(new_head);

        // Check for collisions
        if (new_head.x < 0 || new_head.y < 0 || new_head.x >= max_y || new_head.y >= max_x) {
            game_over = true; // Wall collision
        }
        for (size_t i = 1; i < snake.size(); ++i) {
            if (snake[i].x == new_head.x && snake[i].y == new_head.y) {
                game_over = true; // Self-collision
            }
        }
        
        // Check if food is eaten
        if (new_head.x == food.x && new_head.y == food.y) {
            food = {rand() % max_y, rand() % max_x}; // Place new food
            currentScore++;
        } else {
            snake.pop_back(); // Remove tail
        }

        // Render the game
        clear();
        mvprintw(food.x, food.y, "O"); // Draw food
        for (const auto& segment : snake) {
            mvprintw(segment.x, segment.y, "#"); // Draw snake
        }
        mvprintw(0, 0, "%d", currentScore);
        refresh();
        // Increase difficulty by reducing delay after the player eats more than 2 food items.
        if (currentScore>2) {
            levelCalculater(level);
        }
    }
    return currentScore;
}

// Helper function to check if a string contains only digits
bool isValidNumber(const std::string &str) {
    for (char c : str) {
        if (!std::isdigit(c)) return false;
    }
    return !str.empty(); // Ensure the string is not empty
}

int highScorer(int &currentScore) {
    int highScore = 0;
    std::string line;

    // Open file for reading
    std::ifstream file("scoretracker.txt");
    if (file.is_open()) {
        if (std::getline(file, line) && isValidNumber(line)) {
            highScore = std::stoi(line); // Convert to integer
        }
        file.close();
    }

    // If current score is higher, update the file
    if (currentScore > highScore) {
        highScore = currentScore; //Update high score
        std::ofstream outfile("scoretracker.txt", std::ios::trunc); // Overwrite file
        if (outfile.is_open()) {
            outfile << highScore;
            outfile.close();
        }
    }

    return highScore;
}

//Increase speed as level increases by reducing the timeout.
void levelCalculater(int& level){
    if (!(level<1)) {
        level -= 10;    
    }
}
