#ifndef CG_LAB1_PSUPDATETECHNIQUE_H
#define CG_LAB1_PSUPDATETECHNIQUE_H

#include "../technique/Technique.h"
class PSUpdateTechnique : public Technique {
    public:
        PSUpdateTechnique(
            std::string vertexShaderFilename, std::string geometryShaderFilename
        );

        bool init() override;

        void setParticleLifetime(float lifetime);

        void setDeltaTimeMillis(float deltaTimeMillis) const;

        void setTime(int time) const;

        void setRandomTextureUnit(int textureUnit) const;

        void setLauncherLifetime(float lifetime) const;

        void setShellLifetime(float lifetime) const;

        void setSecondaryShellLifetime(float lifetime) const;

    private:
        std::string mVertexShaderFilename;
        std::string mGeometryShaderFilename;

        GLint mDeltaTimeMillisLocation;
        GLint mRandomTextureLocation;
        GLint mTimeLocation;
        GLint mLauncherLifetimeLocation;
        GLint mShellLifetimeLocation;
        GLint mSecondaryShellLifetimeLocation;
};

#endif  // CG_LAB1_PSUPDATETECHNIQUE_H
