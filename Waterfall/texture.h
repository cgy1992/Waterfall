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

class Texture
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
    Texture();

    bool loadTexture(const string& textureFileName, bool mipmapRequired, int rowCount, int columnCount);
    int rowCount();
    int columnCount();
    void bindTexture(int textureUnit);
    int textureUnit();
    void setFiltering(int magFilter, int minFilter);

    void releaseTexture();
};

class TextureAtlas : public Texture
{
    int _rowCount;
    int _columnCount;

public:
    TextureAtlas(){}
    TextureAtlas(Texture const & texture, int rowCount, int columnCount);

    int getRowCount();
    int getColumnCount();
};

#endif //TEXTURE_H