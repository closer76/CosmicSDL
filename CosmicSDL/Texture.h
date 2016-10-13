//
//  Texture.h
//  CosmicSDL
//
//  Created by Kenneth Lo on 2016/10/13.
//  Copyright © 2016 L's Studio. All rights reserved.
//

#ifndef __Texture_h__
#define __Texture_h__

#include <SDL2/SDL.h>
#include <string>

class CTexture {
    SDL_Texture* m_texture;
    
    int m_width;
    int m_height;
    
public:
    CTexture();
    ~CTexture();
    
    bool loadFromFile(SDL_Renderer* renderer, const std::string& path);
    
    // Deallocate texture
    void free();
    
    // Render full viewport
    void renderFill(SDL_Renderer* renderer);
    
    // Render at given (x, y)
    void render(SDL_Renderer* renderer, int x, int y);
    
    int getWidth();
    int getHeight();
};

#endif /* Texture_h */
