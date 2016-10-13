//
//  Texture.cpp
//  CosmicSDL
//
//  Created by Kenneth Lo on 2016/10/13.
//  Copyright © 2016 L's Studio. All rights reserved.
//

#include "Texture.h"
#include <SDL2_image/SDL_image.h>

using std::string;

CTexture::CTexture()
    : m_texture(nullptr)
    , m_width(0)
    , m_height(0)
{
}

CTexture::~CTexture()
{
    free();
}

void CTexture::renderFill(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, m_texture, nullptr, nullptr);
}

void CTexture::render(SDL_Renderer* renderer, int x, int y)
{
    SDL_Rect rectDest { x, y, m_width, m_height};
    
    SDL_RenderCopy(renderer, m_texture, nullptr, &rectDest);
}

bool CTexture::loadFromFile(SDL_Renderer* renderer, const string& path)
{
    // Deallocated existing texture
    free();
    
    SDL_Surface* surface = nullptr;
    
    surface = IMG_Load(path.c_str());
    if (!surface)
    {
        printf("Unable to load image %s! SDL Error: %s\n",
               path.c_str(),
               SDL_GetError());
    }
    else
    {
        // Set color key
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));
        
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!m_texture)
        {
            printf("Unable to create texture from image %s! SDL Error: %s\n",
                   path.c_str(),
                   SDL_GetError());
        }
        else
        {
            m_width = surface->w;
            m_height = surface->h;
        }
        
        // release old surface
        SDL_FreeSurface(surface);
    }
    
    return (m_texture != nullptr);
}

void CTexture::free()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}


