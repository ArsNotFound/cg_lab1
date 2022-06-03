#include "BillboardList.h"

#include <vector>

#include "../glut_backend/common.h"
#include "../utils/utils.h"

const int NUM_ROWS = 10;
const int NUM_COLS = 10;

BillboardList::BillboardList()
    : mVB(INVALID_OGL_VALUE),
      mTexture(nullptr),
      mTechnique(
          "./shaders/billboard_vertex.glsl",
          "./shaders/billboard_geometry.glsl",
          "./shaders/billboard_fragment.glsl"
      ) {}

BillboardList::~BillboardList() {
    if (mVB != INVALID_OGL_VALUE) glDeleteBuffers(1, &mVB);
}

bool BillboardList::init(const std::string &texFilename) {
    mTexture = std::make_unique<Texture>(GL_TEXTURE_2D, texFilename);
    if (!mTexture->load()) {
        return false;
    }

    createPositionBuffer();

    if (!mTechnique.init()) {
        return false;
    }

    return true;
}

void BillboardList::createPositionBuffer() {
    std::vector<glm::vec3> positions(NUM_ROWS * NUM_COLS);

    for (auto j = 0; j < NUM_ROWS; j++) {
        for (auto i = 0; i < NUM_COLS; i++) {
            glm::vec3 pos(static_cast<float>(j), static_cast<float>(i), 0.0f);
            positions[j * NUM_COLS + i] = pos;
        }
    }

    glGenVertexArrays(1, &mVA);
    glGenBuffers(1, &mVB);

    glBindVertexArray(mVA);
    glBindBuffer(GL_ARRAY_BUFFER, mVB);

    glBufferDataVector(GL_ARRAY_BUFFER, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
}

void BillboardList::render(const glm::mat4 &vp, const glm::vec3 &cameraPos) {
    mTechnique.enable();
    mTechnique.setVP(vp);
    mTechnique.setCameraPosition(cameraPos);

    mTexture->bind(COLOR_TEXTURE_UNIT);

    glBindVertexArray(mVA);

    glDrawArrays(GL_POINTS, 0, NUM_ROWS * NUM_COLS);

    glBindVertexArray(0);
}