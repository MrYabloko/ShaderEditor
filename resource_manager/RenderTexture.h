#ifndef SHADEREDITOR_RENDERTEXTURE_H
#define SHADEREDITOR_RENDERTEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RenderTexture
{
private:
    GLuint framebuffer;
    GLuint depthbuffer;
    GLuint renderedTexture;
public:
    void startRender(GLsizei sizeX, GLsizei sizeY);
    void endRender();
    GLuint getRenderedTextureID();
    RenderTexture();
    ~RenderTexture();
};


#endif
