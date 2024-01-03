#include "texture.h"

#include <cassert>

Texture::Texture(const std::string& filename) : m_textureId(0), m_isValid(false)
{
    if(filename != "")
        Load(filename);
}

Texture::~Texture()
{
    if(IsValid())
        glDeleteTextures(1, &m_textureId);
}

bool Texture::Load(const std::string& filename)
{
    // Initialize Devil only once:
    static bool alreadyInitialized = false;
    if(!alreadyInitialized)
    {
        ilInit();
        alreadyInitialized = true;
    }

    // Use Devil library to load image data in memory
    ILuint texid;
    ilGenImages(1, &texid);
    ilBindImage(texid);

    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilEnable(IL_ORIGIN_SET);

    if (!ilLoadImage((const ILstring)filename.c_str()))
        return false;

    if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
        return false;

    // Create mipmapped opengl texture from image data
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

    if (ilGetInteger(IL_IMAGE_WIDTH) < 1000)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    ilDeleteImages(1, &texid);

    m_isValid = true;
    return true;
}

bool Texture::IsValid() const
{
    return m_isValid;
}

void Texture::Bind() const
{
    assert(IsValid());
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}
