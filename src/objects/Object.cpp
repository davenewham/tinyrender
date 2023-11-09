#include "Object.hpp"

#include <utility>

void
tinyrender::Object::setColor(glm::vec3 c) {
    this->resources->color->set(c);
    this->resources->texture = nullptr;

    if(this->targetPipeline != ColoredTriangle) {
        this->targetPipeline = ColoredTriangle;
        this->resources->resetBindGroup(this->context, this->scene, this->targetPipeline);
    }
}

void
tinyrender::Object::setTexture(std::shared_ptr<tinyrender::Texture2D::Texture> texture) {
    this->resources->texture = std::move(texture);

    if(this->targetPipeline != TexturedTriangle) {
        this->targetPipeline = TexturedTriangle;
        this->resources->resetBindGroup(this->context, this->scene, this->targetPipeline);
    }
}

std::shared_ptr<tinyrender::ModelMatrixUniform>
tinyrender::Object::modelMatrix() const {
    return this->resources->modelMatrix;
}

ObjectPipeline tinyrender::Object::currentTargetPipeline() {
    return targetPipeline;
}

void 
tinyrender::Object::onInit(Context *c, Scene *s) {
    this->context = c;
    this->scene = s;

    if(this->isInitialised)
        throw std::runtime_error("Object is already initialised");

    this->isInitialised = true;
}

void tinyrender::Object::onUpdate(State &state, float dt)
{
    (void) dt;
    (void) state;
    return;
}

void 
tinyrender::Object::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    // Check if set to hidden
    if(HIDDEN)
        return;
    // Object may have no data to draw, which will confuse GPU
    if(this->resources->attributeBuffer->getDrawCalls() == 0)
        return;
    
    // Queue drawing the object
    renderPass.setVertexBuffer(vertexBufferSlot, this->resources->attributeBuffer->getUnderlyingBuffer(), 0, this->resources->attributeBuffer->getSize());
    renderPass.setBindGroup(bindGroupSlot, this->resources->bindGroup, 0, nullptr);
    renderPass.draw(this->resources->attributeBuffer->getDrawCalls(), 1, 0, 0);
}

void 
tinyrender::Object::onRemove() {
    if(!this->isInitialised)
        throw std::runtime_error("Object has not been initialised");

    this->isInitialised = false;
}
