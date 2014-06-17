#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "display/GLProgram.h"
#include "display/GLvboBuffer.h"
#include "display/GLTexture.h"
#include "display/GLCurveObject.h"
#include "vertex/GL_position.h"
#include "vertex/GL_texcord.h"
#include "vertex/GL_Normal.h"
#include "math/GLMatrix4.h"
#include "math/GLSphere.h"
#include "core/GLRasterization.h"
#include "utils/debug.h"
#include "core/GLBmp.h"
#include <fstream>


using namespace std;

float gPos[] = {-0.9,1.0,0.4,0.5,-1.0,-0.5};

GLProgram gProgram;
GLTexture* gTexture = NULL;
GLvboBuffer* gBuffer = NULL;
GLvboBuffer* gTexBuffer = NULL;

static void testCurveShader()
{
    ofstream vertex("output/shader.vertex");
    ofstream frag("output/shader.frag");
    GLCurveObject::GenerateShader(vertex, frag, string("1-u"), string("u*v"), string("v*v"));
    vertex.close();
    frag.close();
}

static void init()
{
    testCurveShader();
    gProgram.loadFiles("glsl/basic.vertex", "glsl/basic.frag");
    gTexture = new GLTexture();
    GLBmp b;
    b.loadPicture("ori.png");
    gTexture->upload(b.pixels(), b.getWidth(), b.getHeight());
    GLMatrix4 projection = GLMatrix4::projection(-300, 300, -300, 300, 100, 400, 1);
    GLSphere sphere(150, 0, 0, -350);
    GLRectArea area;
    area.set(0,0,1,1,0.01,0.01);
    //Vertex
    GL_Normal normal;
    //Texcorder
    GL_texcord tex;
    GL_position p;
    GLCSVertexGenerate(&p, &normal, &tex, &sphere, &area, 0);
    assert(p.size() == tex.size());
    p.reshape();
    p.transform(projection);
    p.normalize();
    gBuffer = new GLvboBuffer(&p);
    gTexBuffer = new GLvboBuffer(&tex);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gProgram.use();
    int pid = gProgram.attr("pos");
    int tid = gProgram.attr("tex");
    gTexBuffer->use(tid);
    gBuffer->use(pid);
    assert(gBuffer->size() == gTexBuffer->size());
    gTexture->use();
    gBuffer->draw();
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, gBuffer->size());
    glutSwapBuffers(); 
} 

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);                            // Initialize GLUT 
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);        // Set display mode 
    glutInitWindowPosition(50,100);                    // Set top-left display window position 
    glutInitWindowSize(500, 500);                    // set display window width and height 
    glutCreateWindow("An Example OpenGL Program");    // Create display window 

    glewInit();
    init();
    glutDisplayFunc(display);                        // Send graphics to display window 

    glutMainLoop();                                    // Display everything and wait 
    return 1;
}
