#include "display/GLCurveObject.h"
#include <string>
#include <sstream>
using namespace std;
const static string trans_m("transform");
const static string proj_m("projection");
const static string us_f("us");
const static string vs_f("vs");
const static string tex_v("tex");
const static string texture_s("texture");

void GLCurveObject::GenerateShader(std::ostream& vertex, std::ostream& frag, const std::string& xf, const std::string& yf, const std::string& zf)
{
    //Vertex shader
    string vTex = "v_"+tex_v;
    vertex << "attribute vec2 "<<tex_v<<";\n";
    vertex << "varying vec2 "<<vTex<<";\n";
    vertex << "uniform mat4 "<<trans_m <<";\n";
    vertex << "uniform mat4 "<<proj_m <<";\n";
    vertex << "uniform float "<<us_f <<";\n";
    vertex << "uniform float "<<vs_f <<";\n";
    vertex << "void main(void)\n{\n";
    vertex << "vec4 temp = vec4(1.0);\n";
    //Compute Curve
    vertex << "float u,v;\n";
    vertex << "u = "<<tex_v<<".x * "<<us_f<<"; v = "<<tex_v<<".y * "<<vs_f<<";\n";
    vertex << "temp.x = " << xf << ";\n";
    vertex << "temp.y = " << yf << ";\n";
    vertex << "temp.z = " << zf << ";\n";
    //Compute MVP
    vertex << "temp = "<<trans_m <<" * "<<proj_m<<" * temp;\n";
    vertex << "gl_Position = temp;\n";
    vertex << vTex <<" = "<<tex_v<<";\n";
    vertex << "}\n";
    
    //Fragment shader
    frag << "uniform sampler2D "<<texture_s<<";\n";
    frag << "varying vec2 "<<vTex<<";\n";
    frag << "void main(void)\n{\n";
    frag << "gl_FragColor = texture2D("<<texture_s << ", "<<vTex<<");\n";
    frag << "}\n";
}

/*Vertex*/
/*x = x(t), y=y(t), z=z(t);
 GL_position = x,y,z,1 * transform* projection*/
/*Fragment: color = texture2D(u,v)*/
GLCurveObject::GLCurveObject()
{
    mTex = NULL;
    mVbo = NULL;
    mUs = 1.0;
    mVs = 1.0;
}
GLCurveObject::~GLCurveObject()
{
    mPro.destroy();
    SAVE_UNREF(mTex);
    SAVE_UNREF(mVbo);
}
void GLCurveObject::setTexture(GLTexture* tex)
{
    assert(NULL!=tex);
    tex->addRef();
    SAVE_UNREF(mTex);
    mTex = tex;
}
void GLCurveObject::setVBO(GLvboBuffer* vbo)
{
    assert(NULL!=vbo);
    vbo->addRef();
    SAVE_UNREF(mVbo);
    mVbo = vbo;
}

void GLCurveObject::setFormula(const std::string& formula_x, const std::string& formula_y, const std::string& formula_z)
{
    ostringstream vertex, frag;
    GenerateShader(vertex, frag, formula_x, formula_y, formula_z);
    mPro.load(vertex.str(), frag.str());
}
void GLCurveObject::setColor(unsigned int argb)
{
    SAVE_UNREF(mTex);
    mTex = new GLTexture();
    mTex->upload(&argb, 1, 1);
}
void GLCurveObject::onDraw(const GLMatrix4& transform, const GLMatrix4& projection)
{
    assert(NULL!=mVbo);
    mPro.use();
    if (NULL == mTex) setColor(0xFF00FF00);//Default green
    mTex->use();
    GLProgram::setMatrix(projection, mPro.uniform(proj_m.c_str()));
    GLProgram::setMatrix(transform, mPro.uniform(trans_m.c_str()));
    GLProgram::setUniform(mUs, mPro.uniform(us_f.c_str()));
    GLProgram::setUniform(mVs, mPro.uniform(vs_f.c_str()));
    mVbo->use(mPro.attr(tex_v.c_str()));
    mVbo->draw();
}
