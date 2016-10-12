//
//  main.cpp
//  CosmicSDL
//
//  Created by Kenneth Lo on 2016/10/12.
//  Copyright © 2016年 L's Studio. All rights reserved.
//

/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Global variables
//The window we'll be rendering to
SDL_Window* g_window = nullptr;

//The surface contained by the window
SDL_Surface* g_screenSurface = nullptr;

SDL_Surface* g_hellowWorldSurface = nullptr;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    else
    {
        //Create window
        g_window = SDL_CreateWindow( "SDL Tutorial",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( g_window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
            // Get window surface
            g_screenSurface = SDL_GetWindowSurface(g_window);
        }
    }
    
    return true;
}

bool loadMedia()
{
    g_hellowWorldSurface = SDL_LoadBMP("hello_world.bmp");
    if (!g_hellowWorldSurface) {
        printf("Unable to load image %s! SDL Error: %s\n",
               "02_getting_an_image_on_the_screen/hello_world.bmp",
               SDL_GetError());
        return false;
    }
    
    return true;
}

void terminate()
{
    // Release surface
    SDL_FreeSurface(g_hellowWorldSurface);
    g_hellowWorldSurface = nullptr;
    
    // Destroy window
    SDL_DestroyWindow( g_window );
    g_window = nullptr;
    
    // Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    if (!init())
    {
        printf("Failed to initialize...\n");
    }
    else
    {
        // Load media
        if (!loadMedia()) {
            printf("Failed to load media...\n");
        } else {
            SDL_BlitSurface(g_hellowWorldSurface, NULL, g_screenSurface, NULL);
            
            SDL_UpdateWindowSurface(g_window);
            
            SDL_Delay(2000);
        }
    }
    
    terminate();
    
    return 0;
}
