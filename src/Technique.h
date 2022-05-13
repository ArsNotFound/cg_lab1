#ifndef CG_LAB1_TECHNIQUE_H
#define CG_LAB1_TECHNIQUE_H

#include <GL/glew.h>

#include <list>
#include <string>

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

class Technique {
    public:
    Technique();
    ~Technique();
    virtual bool init();
    void enable() const;

    protected:
    bool addShader(GLenum shaderType, const std::string &shaderText);
    bool finalize();
    GLint getUniformLocation(const std::string &uniformName) const;

    private:
    GLuint mShaderProg;

    typedef std::list<GLuint> shaderObjList;
    shaderObjList mShaderObjList;
};

#endif  // CG_LAB1_TECHNIQUE_H
