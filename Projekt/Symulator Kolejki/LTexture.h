#pragma once
#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "h_Includes.h"
#include <iostream>
#include <string>
using namespace std;

class LTexture
{
    public:
        LTexture();
        ~LTexture();
        bool loadTextureFromFile( std::string path );
        bool loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height );
        void freeTexture();
        void render( GLfloat x, GLfloat y );
        GLuint getTextureID();
        GLuint textureWidth();
        GLuint textureHeight();
    private:
        GLuint mTextureID;
        GLuint mTextureWidth;
        GLuint mTextureHeight;
};

#endif
