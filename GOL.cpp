#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 720;
const int PIXEL_SIZE = 10;
const int GRIDX = SCREEN_WIDTH / PIXEL_SIZE;
const int GRIDY = SCREEN_HEIGHT/ PIXEL_SIZE;

void initGrid(int grid[GRIDX][GRIDY]);
void renderCurrent(int grid[GRIDX][GRIDY], SDL_Renderer *renderer);
void timeStep(int grid[GRIDX][GRIDY]);
int sumRound(int grid[GRIDX][GRIDY], int posX, int posY);

int main(int argc, char* args[]) {
    srand(time(NULL));

    //initialises grid
    int grid[GRIDX][GRIDY];
    initGrid(grid);

    //initialise window and event handler
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event e;

    //SDL error handling
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else{
        if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) < 0){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            bool quit = false;
            while(!quit){   //game loop
                while(SDL_PollEvent( &e ) != 0 ){
                    if( e.type == SDL_QUIT ){//User requests quit
                        quit = true;
                    }
                }
                renderCurrent(grid, renderer);
                SDL_Delay(1000);
                timeStep(grid);
            }
        }
    } 
    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();
    return 0;
}

void initGrid(int grid[GRIDX][GRIDY]){
    int i, j;
    //main blank grid
    for(i = 0; i < GRIDX - 1; i++){
        for(j = 1; j < GRIDY - 1; j++){
            grid[i][j] = 0;
        }
    }
    //border
    for (i = 0; i < GRIDX; i++){
        grid[i][0] = -1;
        grid[i][GRIDY - 1] = -1;
    }
    for (j = 1; j < GRIDY; j++){
        grid[0][j] = -1;
        grid[GRIDX - 1][j] = -1;
    }

    //starting cells
    grid[35][35] = 1;
    grid[35][36] = 1;
    grid[34][35] = 1;
    grid[33][35] = 1;
    grid[34][34] = 1;
}

void renderCurrent(int grid[GRIDX][GRIDY], SDL_Renderer *renderer){
    //clears screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    //Updates display
    SDL_RenderPresent(renderer);
    int i, j;
    SDL_Rect unit;
    for(i = 0; i < GRIDX; i++){
        for(j = 0; j < GRIDY; j++){
            if(grid[i][j] == -1){
                unit = {i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);  //sets border colour
                SDL_RenderFillRect(renderer, &unit);
            }
            else if(grid[i][j] == 1){
                unit = {i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
                SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, SDL_ALPHA_OPAQUE);   //sets rand colour for each live cell
                SDL_RenderFillRect(renderer, &unit);
            }
        }
    }
    SDL_RenderPresent(renderer);    //renders grid
}

void timeStep(int grid[GRIDX][GRIDY]){
    int gridCopy[GRIDX][ GRIDY];

    int i, j;
    for (i = 0; i < GRIDX; i++){
        for(j = 0; j < GRIDY; j++){
            gridCopy[i][j] = grid[i][j];    //copies grid
        }
    }
    //sets based on rules
    for (i = 1; i < GRIDX - 1; i++){
        for(j = 1; j < GRIDY - 1; j++){
            if(sumRound(gridCopy, i, j) > 3 || sumRound(gridCopy, i, j) < 2){
                grid[i][j] = 0;
            }
            else if(sumRound(gridCopy, i, j) == 3 || (sumRound(gridCopy, i, j) == 2 && gridCopy[i][j] == 1)){
                grid[i][j] = 1;
            }
        }
    }
    return;
}

int sumRound(int grid[GRIDX][GRIDY], int posX, int posY){
    int sum = 0;
    //counts surrounding live pixels
    if(grid[posX - 1][posY] > 0){
        sum++;
    }
    if(grid[posX - 1][posY - 1] > 0){
        sum++;
    }
    if(grid[posX][posY - 1] > 0){
        sum++;
    }
    if(grid[posX + 1][posY - 1] > 0){
        sum++;
    }
    if(grid[posX - 1][posY + 1] > 0){
        sum++;
    }
    if(grid[posX + 1][posY + 1] > 0){
        sum++;
    }
    if(grid[posX + 1][posY] > 0){
        sum++;
    }
    if(grid[posX][posY + 1] > 0){
        sum++;
    }
    return sum;
}