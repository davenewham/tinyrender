//
// Created by Harvey Williams on 09/08/2023.
//

#include "UniformViewProjection.hpp"

engine::UniformViewProjection::UniformViewProjection(Context *context, vec3 lookFrom, vec3 lookAt): Buffer() {
    this->lookFrom = lookFrom;
    this->lookAt = lookAt;

    // Params
    this->focalLength = 2.0;
    this->near = 0.001f;
    this->far = 100.0f;
    float ratio = context->DISPLAY_WIDTH / context->DISPLAY_HEIGHT;
    this->fov = 2 * glm::atan(1 / focalLength);
    uniforms.viewMatrix = glm::lookAt(lookFrom, lookAt, vec3(0, 0, 1));
    uniforms.projectionMatrix = glm::perspective(fov, ratio, near, far);
    uniforms.time = 1.0f;

    // Buffer base class is initialised
    this->context = context;
    this->type = engine::UNIFORM;
    this->size = sizeof(engine::ViewProjectionUniforms);
    this->mapped = false;
    this->initialise(&uniforms);
}

void
engine::UniformViewProjection::updateRotationAboutZAxis() {
    uniforms.time = static_cast<float>(glfwGetTime()); // glfwGetTime returns a double
    // Only update the 1-st float of the buffer
    context->queue.writeBuffer(underlying, offsetof(engine::ViewProjectionUniforms, time),
                              &uniforms.time,sizeof(engine::ViewProjectionUniforms::time));

    // Rotate camera around z-axis
    mat4x4 newViewMatrix= glm::rotate(uniforms.viewMatrix, 2*glm::sin(uniforms.time),
                                      vec3(0.0, 0.0, 1.0));
    context->queue.writeBuffer(underlying, offsetof(engine::ViewProjectionUniforms, viewMatrix),
                              &newViewMatrix, sizeof(engine::ViewProjectionUniforms::viewMatrix));
}

void
engine::UniformViewProjection::updateViewMatrix(glm::mat4x4 viewMatrix) {
    uniforms.viewMatrix = viewMatrix;
    context->queue.writeBuffer(underlying, offsetof(engine::ViewProjectionUniforms, viewMatrix),
                              &uniforms.viewMatrix,sizeof(engine::ViewProjectionUniforms::viewMatrix));
}

void engine::UniformViewProjection::refreshProjectionMatrix(Context *context) {
    float ratio = context->DISPLAY_WIDTH / context->DISPLAY_HEIGHT;
    uniforms.projectionMatrix = glm::perspective(fov, ratio, near, far);
    context->queue.writeBuffer(underlying, offsetof(engine::ViewProjectionUniforms, projectionMatrix),
                               &uniforms.projectionMatrix,
                               sizeof(engine::ViewProjectionUniforms::projectionMatrix));
}
