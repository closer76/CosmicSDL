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

// Main renderer
SDL_Renderer* g_renderer = nullptr;

SDL_Texture* g_currentTexture = nullptr;

vector<string> g_imageNameList {
    "hello_world.bmp",
    "press.bmp",
    "up.bmp",
    "down.bmp",
    "left.bmp",
    "right.bmp",
    "loaded.png"
};

map<string,SDL_Texture*> g_textureList;

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

SDL_Texture* loadTexture(const string& path)
{
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    
    surface = IMG_Load(path.c_str());
    if (!surface)
    {
        printf("Unable to load image %s! SDL Error: %s\n",
               path.c_str(),
               SDL_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(g_renderer, surface);
        if (!texture)
        {
            printf("Unable to create texture from image %s! SDL Error: %s\n",
                   path.c_str(),
                   SDL_GetError());
        }
        
        // release old surface
        SDL_FreeSurface(surface);
    }
    
    return texture;
}

bool loadMedia()
{
    bool result = true;
    SDL_Texture* texture = nullptr;
    
    for (const auto& str : g_imageNameList)
    {
        texture = loadTexture(str.c_str());
        if (texture) {
            g_textureList[str] = texture;
        } else {
            result = false;
        }
    }
    
    // Set current surface to default one.
    g_currentTexture = g_textureList["loaded.png"];
    
    return result;
}

void releaseMedia() {
    for (const auto& pair : g_textureList)
    {
        SDL_DestroyTexture(pair.second);
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
        
        // Screen layout
        SDL_Rect rectViewPort;

        // 「先畫大部份的區域，然後再利用 viewport 在 mainloop 裡更新小區域」
        // 這個方法是行不通的！
        // 所有的區域還是得在 mainloop 裡做。
        // 1. SDL_RenderClear() 不會理會 viewport，會清楚整個 window 的區域。
        // 2. 不呼叫 SDL_RenderClear() 的話，畫面會不斷閃動。
#if 0
        SDL_RenderClear(g_renderer);

        // Upper-right: instruction
        rectViewPort.x = SCREEN_WIDTH / 2;
        rectViewPort.y = 0;
        rectViewPort.w = SCREEN_WIDTH / 2;
        rectViewPort.h = SCREEN_HEIGHT / 2;
        SDL_RenderSetViewport(g_renderer, &rectViewPort);
        SDL_RenderCopy(g_renderer, g_textureList["press.bmp"], NULL, NULL);
        
        // Lower: hello
        rectViewPort.x = 0;
        rectViewPort.y = SCREEN_HEIGHT / 2;
        rectViewPort.w = SCREEN_WIDTH;
        rectViewPort.h = SCREEN_HEIGHT / 2;
        SDL_RenderSetViewport(g_renderer, &rectViewPort);
        SDL_RenderCopy(g_renderer, g_textureList["hello_world.bmp"], NULL, NULL);
#endif
        
        bool bQuit = false;
        SDL_Event evt;
        
        // main loop
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
                            g_currentTexture = g_textureList["up.bmp"];
                            break;
                            
                        case SDLK_DOWN:
                            g_currentTexture = g_textureList["down.bmp"];
                            break;
                            
                        case SDLK_LEFT:
                            g_currentTexture = g_textureList["left.bmp"];
                            break;
                            
                        case SDLK_RIGHT:
                            g_currentTexture = g_textureList["right.bmp"];
                            break;
                            
                        case SDLK_ESCAPE:
                            g_currentTexture = g_textureList["press.bmp"];
                            break;
                            
                        default:
                            break;
                    }
                }
            }

            SDL_RenderClear(g_renderer);
            
            // Upper-right: instruction
            rectViewPort.x = SCREEN_WIDTH / 2;
            rectViewPort.y = 0;
            rectViewPort.w = SCREEN_WIDTH / 2;
            rectViewPort.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport(g_renderer, &rectViewPort);
            SDL_RenderCopy(g_renderer, g_textureList["press.bmp"], NULL, NULL);
            
            // Lower: hello
            rectViewPort.x = 0;
            rectViewPort.y = SCREEN_HEIGHT / 2;
            rectViewPort.w = SCREEN_WIDTH;
            rectViewPort.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport(g_renderer, &rectViewPort);
            SDL_RenderCopy(g_renderer, g_textureList["hello_world.bmp"], NULL, NULL);

            // Upper-left
            rectViewPort.x = 0;
            rectViewPort.y = 0;
            rectViewPort.w = SCREEN_WIDTH / 2;
            rectViewPort.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport(g_renderer, &rectViewPort);
            SDL_RenderCopy(g_renderer, g_currentTexture, nullptr, nullptr);
            
            SDL_RenderPresent(g_renderer);
        } // main loop
        
    } while(0);
    
    cleanUp();
    
    return 0;
}
