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
#include <SDL2_image/SDL_image.h>
#include "Texture.h"

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

// Main renderer
SDL_Renderer* g_renderer = nullptr;

vector<string> g_imageNameList {
    "background.png",
    "foo.png",
};

map<string,CTexture*> g_textureList;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

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
    
    // Create Renderer
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_renderer) {
        printf("Cannot create renderer! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    // Initialize renderer color
    SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize SDL_image.
    int imgFlags = IMG_INIT_PNG;
    if ( (IMG_Init(imgFlags) & imgFlags) == 0) {
        printf("Unable to initialize SDL_image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Get main renderer.
    g_renderer = SDL_GetRenderer(g_window);

    return true;
}

bool loadMedia()
{
    bool result = true;
    CTexture* texture = nullptr;
    
    for (const auto& str : g_imageNameList)
    {
        texture = new CTexture();
        if (texture->loadFromFile(g_renderer, str))
        {
            g_textureList[str] = texture;
        }
        else
        {
            result = false;
        }
    }
    
    return result;
}

void releaseMedia() {
    for (const auto& pair : g_textureList)
    {
        delete pair.second;
        g_textureList[pair.first] = nullptr;
    }
    g_textureList.clear();
}

void cleanUp()
{
    // Release surface
    releaseMedia();
    
    // Destroy window
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow( g_window );
    g_renderer = nullptr;
    g_window = nullptr;
    
    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    do {
        if (!init())
        {
            printf("Failed to initialize...\n");
            break;
        }
        
        if (!loadMedia()) {
            printf("Failed to load media...\n");
            break;
        }
        
        bool bQuit = false;
        SDL_Event evt;
        
        // main loop
        while (!bQuit) {
            // Handle user events
            while (SDL_PollEvent(&evt) != 0) {
                if (evt.type == SDL_QUIT) {
                    bQuit = true;
                }
            }

            SDL_RenderClear(g_renderer);

            g_textureList["background.png"]->renderFill(g_renderer);
            g_textureList["foo.png"]->render(g_renderer, 240, 190);
            
            SDL_RenderPresent(g_renderer);
        } // main loop
        
    } while(0);
    
    cleanUp();
    
    return 0;
}
