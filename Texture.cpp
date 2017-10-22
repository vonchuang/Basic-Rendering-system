#include "Texture.h"
#include <lodepng.h>

// Reference https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml

static GLenum WrapModeOpenGLMapping[] =
{ GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };

static GLenum FilterModeOpenGLMapping[] =
{ GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_NEAREST, 
GL_LINEAR_MIPMAP_NEAREST };

Texture2D Texture2D::LoadFromFile(const std::string & pngfile)
{
    // load png and decode into raw data
    std::vector<uint8_t> raw_data;
    uint32_t width=0, height=0;
    lodepng::decode(raw_data, width, height, pngfile);
    if(width==0 || height==0){
        return Texture2D(0U);
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    // upload!!
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_data.data());
    Texture2D ret(id);
    ret.m_mipmap = false;
    ret.setFilter(FilterMode::eNearest, FilterMode::eNearest);
    ret.setWrap(WrapMode::eRepeat, WrapMode::eRepeat);
    return ret;
}

void Texture2D::bindToChannel(GLuint channel)
{
    glActiveTexture(GL_TEXTURE0+channel);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::setWrap(WrapMode s, WrapMode t, WrapMode r)
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapModeOpenGLMapping[static_cast<uint32_t>(s)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapModeOpenGLMapping[static_cast<uint32_t>(t)]);
    // r is meaning less in texture2d
}

void Texture2D::release()
{
    glDeleteTextures(1, &m_id);
}

void Texture2D::setFilter(FilterMode min, FilterMode mag)
{
    // if mipmapping is required, generate it
    if (min >= FilterMode::eNearestMipmapLinear) {
        glGenerateMipmap(GL_TEXTURE_2D);
        m_mipmap = true;
    }
    // check parameter..
    if (mag >= FilterMode::eNearestMipmapLinear) {
        mag = FilterMode::eNearest;
    }

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterModeOpenGLMapping[static_cast<uint32_t>(min)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterModeOpenGLMapping[static_cast<uint32_t>(mag)]);
}

bool Texture2D::hasMipmap()
{
    return m_mipmap;
}

Texture2D::Texture2D(GLuint id)
    : ITexture(), m_id(id)
{
}