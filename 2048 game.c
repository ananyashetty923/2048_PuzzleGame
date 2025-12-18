#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4

int board[SIZE][SIZE];

// Function to draw the "GUI" in the terminal
void drawBoard() {
    system("clear"); // Use "cls" for Windows, "clear" for Linux/Online compilers
    printf("--- 2048 C GAME ---\n\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) printf(".    ");
            else printf("%-5d", board[i][j]);
        }
        printf("\n\n");
    }
    printf("Controls: W (Up), S (Down), A (Left), D (Right), Q (Quit)\n");
}

// Function to add a random 2 or 4 to an empty spot
void addRandom() {
    int r, c;
    do {
        r = rand() % SIZE;
        c = rand() % SIZE;
    } while (board[r][c] != 0);
    board[r][c] = (rand() % 10 < 9) ? 2 : 4;
}

int main() {
    srand(time(NULL));
    addRandom();
    addRandom();

    char input;
    while (1) {
        drawBoard();
        printf("Enter move: ");
        scanf(" %c", &input); // Note the space before %c

        if (input == 'q' || input == 'Q') break;

        // Logic for sliding and merging would go here
        // For now, we just add a new number to show the board updates
        addRandom();
    }

    return 0;
}
