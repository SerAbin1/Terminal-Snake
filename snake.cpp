/*Features to Add

1. Scoring:

Keep track of the snake's length or the number of food items eaten.

Display the score on the screen.



2. Levels:

Increase the snake’s speed (reduce timeout() delay) as the game progresses.



3. Obstacles:

Add walls or randomly placed obstacles to increase difficulty.



4. Persistence:

Save high scores to a file and display them at the start.



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

struct Point {
    int x, y;
};

int main() {
    // Initialize ncurses
    initscr();
    noecho(); //To disable user input echo,displaying the char typed b                y user.
    cbreak(); //Disable line buffering. Send input directly without wa               iting for enter.
    keypad(stdscr, TRUE); //for arrow key usage.
    curs_set(0); //To hide the cursor.
    timeout(100); // Non-blocking input (100ms delay)
    
    int currScore = 0; //to keep track of the hcurrent core.
    int highScore = 0; //to keeep track of the hig sckore.

    bool game_over = false;
    
    while (!game_over) {
        currScore = game(game_over);
        clear();
        mvprintw(0, 0, "Press p to play again and q to quit");
        highScore = highScorer(currScore);
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
    int max_x, max_y, currScore;
    getmaxyx(stdscr, max_y, max_x); //gets the size of the window and                                       stores it in the variables.
    std::deque<Point> snake = {{max_y / 2, max_x / 2}}; // Snake start                                                      s at the center
    Point food = {rand() % max_y, rand() % max_x};
    int dir_x = 1, dir_y = 0; // Initial direction (right)

    // Main game loop
    while (!game_over) {
        // Input handling
        timeout(100);
        int ch = getch();
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
            currScore++;
        } else {
            snake.pop_back(); // Remove tail
        }

        // Render the game
        clear();
        mvprintw(food.x, food.y, "O"); // Draw food
        for (const auto& segment : snake) {
            mvprintw(segment.x, segment.y, "#"); // Draw snake
        }
        mvprintw(0, 0, "%d", currScore);
        refresh();
    }
    return currScore;
}

// Helper function to check if a string contains only digits
bool isValidNumber(const std::string &str) {
    for (char c : str) {
        if (!std::isdigit(c)) return false;
    }
    return !str.empty(); // Ensure the string is not empty
}

int highScorer(int &currScore) {
    int highScore = 0; // Default high score
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
    if (currScore > highScore) {
        highScore = currScore; // Update high score
        std::ofstream outfile("scoretracker.txt", std::ios::trunc); // Overwrite file
        if (outfile.is_open()) {
            outfile << highScore;
            outfile.close();
        }
    }

    return highScore;
}
