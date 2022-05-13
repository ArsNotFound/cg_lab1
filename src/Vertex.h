#ifndef CG_LAB1_VERTEX_H
#define CG_LAB1_VERTEX_H


#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define VERTEX_POS_OFFSET 0
#define VERTEX_TEX_OFFSET VERTEX_POS_OFFSET + sizeof(glm::vec3)
#define VERTEX_NORMAL_OFFSET VERTEX_TEX_OFFSET + sizeof(glm::vec2)

struct Vertex {
    glm::vec3 mPos;
    glm::vec2 mTex;
    glm::vec3 mNormal;

    Vertex() = default;

    Vertex(const glm::vec3 &mPos, const glm::vec2 &mTex) :
            mPos(mPos),
            mTex(mTex),
            mNormal(glm::vec3(0.0f, 0.0f, 0.0f)) {}
};


#endif //CG_LAB1_VERTEX_H
