#include "core/GLBmp.h"
#include "GL/GLContext.h"
#include "GL/GLBitmapWork.h"
#include "GL/GLMultiBitmapWork.h"
#include "GL/GLTextureMatrixWork.h"

int main()
{
    GLAutoContext __c;
    GPPtr<GLBmp> origin = new GLBmp("input.jpg");
    GPPtr<GLBmp> dst = new GLBmp(origin->width(), origin->height());
    GPPtr<GLMultiBitmapWork> work = new GLMultiBitmapWork(dst);
    {
        GLMatrix4 current;
        work->addWork(new GLTextureMatrixWork(current), origin);
    }
    {
        GPPtr<GLBmp> src = new GLBmp("input2.jpg");
        GLMatrix4 current;
        GLMatrix4 scale;
        scale.setScale(0.1,0.1, 1);
        GLMatrix4 rotate;
        rotate.setRotate(0, 0, 1, 30);
        GLMatrix4 trans;
        trans.setTranslate(-0.5, -0.5, 0);
        current=trans*scale*rotate;
        work->addWork(new GLTextureMatrixWork(current), src);
    }
    work->runOnePass();
    dst->save("output/temp.png");
    return 1;
}
