#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

enum TextureFiltering
{
    NO_TEXTURE_FILTER,
    TEXTURE_FILTER_MAG_NEAREST,
    TEXTURE_FILTER_MAG_LINEAR,
    TEXTURE_FILTER_MIN_NEAREST,
    TEXTURE_FILTER_MIN_LINEAR,
    TEXTURE_FILTER_MIN_NEAREST_MIPMAP_NEAREST,
    TEXTURE_FILTER_MIN_NEAREST_MIPMAP_LINEAR,
    TEXTURE_FILTER_MIN_LINEAR_MIPMAP_NEAREST,
    TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR
};

class TextureAtlas
{
    uint _width, _height, _bpp;
    GLuint _texture;
    GLuint _sampler;
    int _textureUnit;
    bool _mipmapGenerated;
    int _magFilter;
    int _minFilter;
    string _textureFileName;
    int _rowCount;
    int _columnCount;

public:
    TextureAtlas();
    
    void init();

    bool loadTexture(const string& textureFileName, int rowCount, int columnCount);
    int rowCount();
    int columnCount();
    void bindTexture(int textureUnit);
    int textureUnit();
    void setFiltering(int magFilter, int minFilter);

    void releaseTexture();
};

class Texture
{
    GLuint texture_;
    GLuint sampler_;
    int textureUnit_;
    int width_;
    int height_;
    int bpp_;

public:
    Texture();
    
    void init();
    
    void createFromBackground(int x, int y, int width, int height);
    
    void loadTexture(const string& texFilename);
    void bindTexture(int textureUnit);
    void setFiltering(int minFilter, int magFilter);
    void setSamplerProp(GLenum prop, GLenum value);
};

//class TextureLoader
//{
//public:
//    TextureLoader(const string& texFilename, int rows, int columns);
//
//};

#endif //TEXTURE_H