#include <iostream>
#include <limits>
#include <array>
#include <random>
#include <algorithm>

class TicTacToe {
private:
    std::array<char, 10> board;
    bool isPlayer1Turn;
    bool isSinglePlayer;
    
    const char PLAYER1_MARK = 'X';
    const char PLAYER2_MARK = 'O';
    std::random_device rd;
    std::mt19937 rng{rd()};
    
    bool isValidMove(int position) const {
        return position >= 1 && position <= 9 && 
               board[position] != PLAYER1_MARK && 
               board[position] != PLAYER2_MARK;
    }
    
    bool checkLine(int pos1, int pos2, int pos3) const {
        return (board[pos1] == board[pos2] && board[pos2] == board[pos3]);
    }
    
    void clearScreen() const {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    bool isBoardFull() const {
        for (int i = 1; i <= 9; ++i) {
            if (board[i] != PLAYER1_MARK && board[i] != PLAYER2_MARK) {
                return false;
            }
        }
        return true;
    }

    // Check if making a move at 'position' would result in a win
    bool isWinningMove(char mark, int position) const {
        if (!isValidMove(position)) return false;
        
        std::array<char, 10> tempBoard = board;
        tempBoard[position] = mark;
        
        // Check rows
        for (int i = 1; i <= 7; i += 3) {
            if (tempBoard[i] == mark && tempBoard[i+1] == mark && tempBoard[i+2] == mark) return true;
        }
        // Check columns
        for (int i = 1; i <= 3; ++i) {
            if (tempBoard[i] == mark && tempBoard[i+3] == mark && tempBoard[i+6] == mark) return true;
        }
        // Check diagonals
        if (tempBoard[1] == mark && tempBoard[5] == mark && tempBoard[9] == mark) return true;
        if (tempBoard[3] == mark && tempBoard[5] == mark && tempBoard[7] == mark) return true;
        
        return false;
    }

    int getComputerMove() {
        // First, try to win
        for (int i = 1; i <= 9; ++i) {
            if (isWinningMove(PLAYER2_MARK, i)) return i;
        }
        
        // Then, block player's winning move
        for (int i = 1; i <= 9; ++i) {
            if (isWinningMove(PLAYER1_MARK, i)) return i;
        }
        
        // Try to take center if available
        if (isValidMove(5)) return 5;
        
        // Try to take corners
        std::array<int, 4> corners = {1, 3, 7, 9};
        std::shuffle(corners.begin(), corners.end(), rng);
        for (int corner : corners) {
            if (isValidMove(corner)) return corner;
        }
        
        // Take any available side
        std::array<int, 4> sides = {2, 4, 6, 8};
        std::shuffle(sides.begin(), sides.end(), rng);
        for (int side : sides) {
            if (isValidMove(side)) return side;
        }
        
        return -1; // Should never reach here if board is not full
    }

public:
    TicTacToe(bool singlePlayer) : isPlayer1Turn(true), isSinglePlayer(singlePlayer) {
        for (int i = 1; i <= 9; ++i) {
            board[i] = '0' + i;
        }
    }
    
    void displayBoard() const {
        clearScreen();
        std::cout << "\n\n\tTic Tac Toe\n\n";
        std::cout << "Player 1 (" << PLAYER1_MARK << ") - " 
                 << (isSinglePlayer ? "Computer" : "Player 2") 
                 << " (" << PLAYER2_MARK << ")\n\n";
        
        for (int i = 1; i <= 9; i += 3) {
            std::cout << "     |     |     \n";
            std::cout << "  " << board[i] << "  |  " << board[i+1] << "  |  " << board[i+2] << "  \n";
            if (i < 7) std::cout << "_____|_____|_____\n";
            else std::cout << "     |     |     \n";
        }
        std::cout << "\n";
    }
    
    bool makeMove(int position) {
        if (!isValidMove(position)) {
            std::cout << "Invalid move! Position " << position << " is already taken or out of range.\n";
            std::cout << "Press Enter to try again...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return false;
        }
        
        board[position] = (isPlayer1Turn) ? PLAYER1_MARK : PLAYER2_MARK;
        isPlayer1Turn = !isPlayer1Turn;
        return true;
    }

    void makeComputerMove() {
        int position = getComputerMove();
        if (position != -1) {
            std::cout << "Computer chooses position " << position << "\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            makeMove(position);
        }
    }
    
    int checkGameState() const {
        // Check rows
        for (int i = 1; i <= 7; i += 3) {
            if (checkLine(i, i+1, i+2)) return 1;
        }
        
        // Check columns
        for (int i = 1; i <= 3; ++i) {
            if (checkLine(i, i+3, i+6)) return 1;
        }
        
        // Check diagonals
        if (checkLine(1, 5, 9)) return 1;
        if (checkLine(3, 5, 7)) return 1;
        
        return isBoardFull() ? 0 : -1;
    }
    
    bool isComputerTurn() const {
        return isSinglePlayer && !isPlayer1Turn;
    }
    
    int getCurrentPlayer() const {
        return isPlayer1Turn ? 1 : 2;
    }
};

int main() {
    std::cout << "Welcome to Tic-Tac-Toe!\n\n";
    std::cout << "Select game mode:\n";
    std::cout << "1. Single Player (vs Computer)\n";
    std::cout << "2. Two Players\n";
    std::cout << "Enter your choice (1 or 2): ";
    
    int choice;
    while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        std::cout << "Invalid input! Please enter 1 or 2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    TicTacToe game(choice == 1);
    int position;
    int gameState;
    
    do {
        game.displayBoard();
        
        if (game.isComputerTurn()) {
            game.makeComputerMove();
        } else {
            std::cout << "Player " << game.getCurrentPlayer() << ", enter a position (1-9): ";
            while (!(std::cin >> position)) {
                std::cout << "Invalid input! Please enter a number between 1 and 9: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            
            if (!game.makeMove(position)) {
                continue;
            }
        }
        
        gameState = game.checkGameState();
        
    } while (gameState == -1);
    
    game.displayBoard();
    
    if (gameState == 1) {
        if (choice == 1) {
            std::cout << (game.getCurrentPlayer() == 1 ? "Computer wins!\n" : "You win!\n");
        } else {
            int winner = game.getCurrentPlayer() == 1 ? 2 : 1;
            std::cout << "Player " << winner << " wins!\n";
        }
    } else {
        std::cout << "Game is a draw!\n";
    }
    
    return 0;
}