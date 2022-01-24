#if defined __ANDROID__
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define GLES20 1
#else
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
//#include <opengl/gl.h>
#define __gl_h_
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
//#include <opengl/gl3.h>
//#include <opengl/gl3ext.h>
#define GL33 1
#endif