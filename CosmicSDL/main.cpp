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

#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Global variables
//The window we'll be rendering to
SDL_Window* g_window = nullptr;

//The surface contained by the window
SDL_Surface* g_screenSurface = nullptr;

SDL_Surface* g_currentSurface = nullptr;

vector<string> g_imageNameList {
    "hello_world.bmp",
    "press.bmp",
    "up.bmp",
    "down.bmp",
    "left.bmp",
    "right.bmp"
};

map<string,SDL_Surface*> g_surfaceList;

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

SDL_Surface* loadSurface(const string& path)
{
    SDL_Surface* surfaceRaw = nullptr;
    SDL_Surface* surfaceOptimized = nullptr;
    
    surfaceRaw = SDL_LoadBMP(path.c_str());
    if (!surfaceRaw)
    {
        printf("Unable to load image %s! SDL Error: %s\n",
               path.c_str(),
               SDL_GetError());
    }
    else
    {
        surfaceOptimized = SDL_ConvertSurface(surfaceRaw, g_screenSurface->format, 0);
        if (!surfaceOptimized)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n",
                   path.c_str(),
                   SDL_GetError());
        }
        
        // release old surface
        SDL_FreeSurface(surfaceRaw);
    }
    
    return surfaceOptimized;
}

bool loadMedia()
{
    bool result = true;
    SDL_Surface* surface = nullptr;
    
    for (const auto& str : g_imageNameList)
    {
        surface = loadSurface(str.c_str());
        if (surface) {
            g_surfaceList[str] = surface;
        } else {
            result = false;
        }
    }
    
    // Set current surface to default one.
    g_currentSurface = g_surfaceList["press.bmp"];
    
    return result;
}

void releaseMedia() {
    for (const auto& pair : g_surfaceList)
    {
        SDL_FreeSurface(pair.second);
        g_surfaceList[pair.first] = nullptr;
    }
    g_surfaceList.clear();
}

void terminate()
{
    // Release surface
    releaseMedia();
    
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
            bool bQuit = false;
            SDL_Event evt;
            
            while (!bQuit) {
                // Handle user events
                while (SDL_PollEvent(&evt) != 0) {
                    if (evt.type == SDL_QUIT) {
                        bQuit = true;
                    }
                    else if (evt.type == SDL_KEYDOWN)
                    {
                        switch (evt.key.keysym.sym) {
                            case SDLK_UP:
                                g_currentSurface = g_surfaceList["up.bmp"];
                                break;

                            case SDLK_DOWN:
                                g_currentSurface = g_surfaceList["down.bmp"];
                                break;

                            case SDLK_LEFT:
                                g_currentSurface = g_surfaceList["left.bmp"];
                                break;

                            case SDLK_RIGHT:
                                g_currentSurface = g_surfaceList["right.bmp"];
                                break;
                                
                            case SDLK_ESCAPE:
                                g_currentSurface = g_surfaceList["press.bmp"];
                                break;
                                
                            default:
                                break;
                        }
                    }
                }
                
                //SDL_BlitSurface(g_currentSurface, NULL, g_screenSurface, NULL);
                SDL_Rect rectBlit;
                rectBlit.x = 0;
                rectBlit.y = 0;
                rectBlit.w = SCREEN_WIDTH;
                rectBlit.h = SCREEN_HEIGHT;
                SDL_BlitScaled(g_currentSurface, NULL, g_screenSurface, &rectBlit);
                
                SDL_UpdateWindowSurface(g_window);
            }
        }
    }
    
    terminate();
    
    return 0;
}
