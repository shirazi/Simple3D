#include <stdlib.h>
#include <GL/GLContext.h>
#include "test/GLTest.h"
int main()
{
    srand(time(NULL));
    GLContext::init();
    GLTestSuit::run();
    GLContext::destroy();
}
