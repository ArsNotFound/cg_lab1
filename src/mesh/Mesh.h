#ifndef CG_LAB1_MESH_H
#define CG_LAB1_MESH_H

#include <GL/glew.h>

#include <memory>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "../texture/Texture.h"
#include "../utils/utils.h"

#define VERTEX_POS_OFFSET 0
#define VERTEX_TEX_OFFSET (VERTEX_POS_OFFSET + sizeof(glm::vec3))
#define VERTEX_NORMAL_OFFSET (VERTEX_TEX_OFFSET + sizeof(glm::vec2))
#define VERTEX_TANGENT_OFFSET (VERTEX_NORMAL_OFFSET + sizeof(glm::vec3))

struct Vertex {
        glm::vec3 mPos;
        glm::vec2 mTex;
        glm::vec3 mNormal;
        glm::vec3 mTangent;

        Vertex() = default;

        Vertex(
            const glm::vec3& pos,
            const glm::vec2& tex,
            const glm::vec3& normal,
            const glm::vec3& tangent
        )
            : mPos(pos),
              mTex(tex),
              mNormal(normal),
              mTangent(tangent) {}
};

class Mesh {
    public:
        Mesh();

        ~Mesh();

        bool loadMesh(const std::string& filename);

        void render();

    private:
        bool initFromScene(const aiScene* scene, const std::string& filename);
        void initMesh(unsigned int index, const aiMesh* mesh);
        bool initMaterials(const aiScene* scene, const std::string& filename);
        void clear();

#define INVALID_MATERIAL 0xFFFFFFFF

        struct MeshEntry {
                MeshEntry();

                ~MeshEntry();

                bool init(
                    const std::vector<Vertex>& vertices,
                    const std::vector<unsigned int>& indices
                );

                GLuint VA;
                GLuint VB;
                GLuint IB;
                unsigned int numIndices;
                unsigned int materialIndex;
        };

        std::vector<MeshEntry> mEntries;
        std::vector<std::shared_ptr<Texture>> mTextures;
};

#endif  // CG_LAB1_MESH_H
