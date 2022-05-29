#include "Mesh.h"

Mesh::MeshEntry::MeshEntry()
    : VB(INVALID_OGL_VALUE),
      IB(INVALID_OGL_VALUE),
      VA(INVALID_OGL_VALUE),
      numIndices(0),
      materialIndex(INVALID_MATERIAL) {}

Mesh::MeshEntry::~MeshEntry() {
    if (VB == INVALID_OGL_VALUE) glDeleteBuffers(1, &VB);

    if (IB == INVALID_OGL_VALUE) glDeleteBuffers(1, &IB);

    if (VA == INVALID_OGL_VALUE) glDeleteVertexArrays(1, &VA);
}

bool Mesh::MeshEntry::init(const std::vector<Vertex> &vertices,
                           const std::vector<unsigned int> &indices) {
    numIndices = indices.size();

    glGenVertexArrays(1, &VA);
    glBindVertexArray(VA);

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferDataVector(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferDataVector(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const GLvoid *>(VERTEX_POS_OFFSET));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const GLvoid *>(VERTEX_TEX_OFFSET));
    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<const GLvoid *>(VERTEX_NORMAL_OFFSET));

    glBindVertexArray(0);

    return true;
}

Mesh::Mesh() = default;

Mesh::~Mesh() { clear(); }

void Mesh::clear() {}

bool Mesh::loadMesh(const std::string &filename) {
    clear();

    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(
        filename.c_str(),
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (scene) {
        return initFromScene(scene, filename);
    }

    std::cerr << "Error parsing '" << filename << "': '"
              << importer.GetErrorString() << "'" << std::endl;
    return false;
}

bool Mesh::initFromScene(const aiScene *scene, const std::string &filename) {
    mEntries.resize(scene->mNumMeshes);
    mTextures.resize(scene->mNumMaterials);

    for (auto i = 0; i < mEntries.size(); i++) {
        const aiMesh *mesh = scene->mMeshes[i];
        initMesh(i, mesh);
    }

    return initMaterials(scene, filename);
}

void Mesh::initMesh(unsigned int index, const aiMesh *mesh) {
    mEntries[index].materialIndex = mesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for (auto i = 0; i < mesh->mNumVertices; i++) {
        const aiVector3D *pos = &(mesh->mVertices[i]);
        const aiVector3D *normal = &(mesh->mNormals[i]);
        const aiVector3D *texCoord =
            mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(glm::vec3(pos->x, pos->y, pos->z),
                 glm::vec2(texCoord->x, texCoord->y),
                 glm::vec3(normal->x, normal->y, normal->z));
        vertices.push_back(v);
    }

    for (auto i = 0; i < mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    mEntries[index].init(vertices, indices);
}

bool Mesh::initMaterials(const aiScene *scene, const std::string &filename) {
    auto slashIndex = filename.find_last_of('/');
    std::string dir;

    if (slashIndex == std::string::npos) {
        dir = ".";
    } else if (slashIndex == 0) {
        dir = "/";
    } else {
        dir = filename.substr(0, slashIndex);
    }

    bool ret = true;

    for (auto i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial *material = scene->mMaterials[i];

        mTextures[i] = nullptr;
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) ==
                AI_SUCCESS) {
                std::string fullPath = dir + "/" + path.data;
                mTextures[i] =
                    std::make_shared<Texture>(GL_TEXTURE_2D, fullPath);

                if (!mTextures[i]->load()) {
                    std::cerr << "Error loading texture '" << fullPath << "'"
                              << std::endl;
                    mTextures[i] = nullptr;
                    ret = false;
                }
            }
        }

        if (!mTextures[i]) {
            mTextures[i] =
                std::make_shared<Texture>(GL_TEXTURE_2D, "./content/white.png");
            ret = mTextures[i]->load();
        }
    }

    return ret;
}

void Mesh::render() {
    for (const auto &entry : mEntries) {
        glBindVertexArray(entry.VA);
        glBindBuffer(GL_ARRAY_BUFFER, entry.VB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entry.IB);

        const auto materialIndex = entry.materialIndex;
        if (materialIndex < mTextures.size() && mTextures[materialIndex])
            mTextures[materialIndex]->bind(GL_TEXTURE0);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(entry.numIndices),
                       GL_UNSIGNED_INT, nullptr);
    }
}
