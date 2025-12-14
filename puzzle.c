#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define GRID_SIZE 4

// Game state structure
typedef struct {
    int grid[GRID_SIZE][GRID_SIZE];
    int score;
    int gameOver;
    int won;
} GameState;

// Initialize game state
void init_game(GameState *game) {
    int i, j;
    
    // Clear grid
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            game->grid[i][j] = 0;
        }
    }
    
    game->score = 0;
    game->gameOver = 0;
    game->won = 0;
    
    // Seed random number generator
    srand(time(NULL));
    
    // Add two initial tiles
    add_random_tile(game);
    add_random_tile(game);
}

// Add a random tile (2 or 4) to an empty position
void add_random_tile(GameState *game) {
    int empty_cells[GRID_SIZE * GRID_SIZE][2];
    int count = 0;
    int i, j;
    
    // Find all empty cells
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            if (game->grid[i][j] == 0) {
                empty_cells[count][0] = i;
                empty_cells[count][1] = j;
                count++;
            }
        }
    }
    
    // If there are empty cells, add a tile
    if (count > 0) {
        int index = rand() % count;
        int row = empty_cells[index][0];
        int col = empty_cells[index][1];
        
        // 90% chance of 2, 10% chance of 4
        game->grid[row][col] = (rand() % 10 < 9) ? 2 : 4;
    }
}

// Rotate grid 90 degrees clockwise
void rotate_grid(GameState *game) {
    int temp[GRID_SIZE][GRID_SIZE];
    int i, j;
    
    // Copy to temp
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            temp[i][j] = game->grid[i][j];
        }
    }
    
    // Rotate
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            game->grid[j][GRID_SIZE - 1 - i] = temp[i][j];
        }
    }
}

// Move and merge tiles to the left
int move_left(GameState *game) {
    int moved = 0;
    int i, j, k;
    
    for (i = 0; i < GRID_SIZE; i++) {
        int row[GRID_SIZE];
        int index = 0;
        
        // Collect non-zero values
        for (j = 0; j < GRID_SIZE; j++) {
            if (game->grid[i][j] != 0) {
                row[index++] = game->grid[i][j];
            }
        }
        
        // Merge adjacent equal tiles
        for (j = 0; j < index - 1; j++) {
            if (row[j] == row[j + 1]) {
                row[j] *= 2;
                game->score += row[j];
                
                // Check for win condition
                if (row[j] == 2048) {
                    game->won = 1;
                }
                
                // Shift remaining tiles
                for (k = j + 1; k < index - 1; k++) {
                    row[k] = row[k + 1];
                }
                index--;
            }
        }
        
        // Check if row changed
        int original[GRID_SIZE];
        for (j = 0; j < GRID_SIZE; j++) {
            original[j] = game->grid[i][j];
        }
        
        // Update grid row
        for (j = 0; j < GRID_SIZE; j++) {
            game->grid[i][j] = (j < index) ? row[j] : 0;
            
            if (game->grid[i][j] != original[j]) {
                moved = 1;
            }
        }
    }
    
    return moved;
}

// Move tiles in specified direction
// direction: 0=up, 1=left, 2=down, 3=right
int move_tiles(GameState *game, int direction) {
    int moved = 0;
    
    if (game->gameOver && !game->won) {
        return 0;
    }
    
    switch (direction) {
        case 0: // Up
            rotate_grid(game);
            rotate_grid(game);
            rotate_grid(game);
            moved = move_left(game);
            rotate_grid(game);
            break;
            
        case 1: // Left
            moved = move_left(game);
            break;
            
        case 2: // Down
            rotate_grid(game);
            moved = move_left(game);
            rotate_grid(game);
            rotate_grid(game);
            rotate_grid(game);
            break;
            
        case 3: // Right
            rotate_grid(game);
            rotate_grid(game);
            moved = move_left(game);
            rotate_grid(game);
            rotate_grid(game);
            break;
    }
    
    if (moved) {
        add_random_tile(game);
        
        if (check_game_over(game)) {
            game->gameOver = 1;
        }
    }
    
    return moved;
}

// Check if game is over (no more moves possible)
int check_game_over(GameState *game) {
    int i, j;
    
    // Check for empty cells
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            if (game->grid[i][j] == 0) {
                return 0;
            }
        }