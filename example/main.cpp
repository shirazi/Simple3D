#include "core/GLBmp.h"
#include "GL/GLContext.h"
#include "GL/GLBitmapWork.h"
#include "GL/GLTextureMatrixWork.h"

int main()
{
    GLAutoContext __c;
    GLMatrix4 current;
    GPPtr<GLBmp> src = new GLBmp("input2.jpg");
    GPPtr<GLBmp> dst = new GLBmp("input.jpg");
    {
        GLMatrix4 scale;
        scale.setScale(0.1,0.1, 1);
        GLMatrix4 rotate;
        rotate.setRotate(0, 0, 1, 30);
        GLMatrix4 trans;
        trans.setTranslate(0.5, 0.5, 0);
        current=trans*scale*rotate;
    }
    {
        GPPtr<GLTextureWork> _w= new GLTextureMatrixWork(current);
        GPPtr<GLBitmapWork> w = new GLBitmapWork(_w);
        w->set(src, dst);
        w->runOnePass();
    }
    dst->save("output/GLTextureMatrixWorkTest.png");
    return 1;
}
