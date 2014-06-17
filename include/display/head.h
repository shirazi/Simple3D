#ifndef DISPLAY_HEAD_H
#define DISPLAY_HEAD_H

/*This mean the function can just be called in the opengl Context*/
#define CONTEXT_API
/*This mean the class should be used only in opengl context thread*/
#define CONTEXT_CLASS
/*If defined this, means the method of a CONTEXT_CLASS can be called outside context*/
#define CONTEXT_FREE_API

#include <assert.h>
#define OPENGL_ASSERT(x) assert(x)

#endif
