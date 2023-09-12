//
// Created by Harvey Williams on 09/08/2023.
//

#ifndef TINYGAME_UNIFORMMVPBUFFER_H
#define TINYGAME_UNIFORMMVPBUFFER_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "../Buffer.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;

namespace engine {

    struct ViewProjectionUniforms {
        mat4x4 projectionMatrix;
        mat4x4 viewMatrix;
        vec4 color; // Unused
        float time;
        float _pad[3];
    };
    static_assert(sizeof(ViewProjectionUniforms) % 16 == 0, "Must be multiple of 16 bytes");

    class UniformViewProjection : public Buffer {
    protected:
        ViewProjectionUniforms uniforms{};

        // Geometric properties
        vec3 lookFrom;
        vec3 lookAt;
        float focalLength;
        float near;
        float far;
        float fov;
    public:
        UniformViewProjection(Context *context, vec3 lookFrom, vec3 lookAt);
        void refreshProjectionMatrix(Context *context); // Useful if dimensions of window change
        void updateRotationAboutZAxis();
        void updateViewMatrix(glm::mat4x4 viewMatrix);
    };

}

#endif //TINYGAME_UNIFORMMVPBUFFER_H
