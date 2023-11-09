#pragma once

#include "../webgpu/primitives/buffers/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../webgpu/primitives/buffers/IndexBuffer.hpp"
#include "../webgpu/Scene.hpp"
#include "../State.hpp"
#include "ObjectResources.hpp"


namespace tinyrender {

    class Object {
    protected:
        Context *context;
        Scene *scene;

        ObjectPipeline targetPipeline = ColoredTriangle;
        
        bool isInitialised = false;
    public:
        virtual void onInit(Context *c, Scene *s);
        virtual void onUpdate(State &state, float dt);
        virtual void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
        virtual void onRemove() ;

        virtual void setColor(glm::vec3 c);
        virtual void setTexture(std::shared_ptr<tinyrender::Texture2D::Texture> texture);

        virtual ~Object() = default;

        ObjectPipeline currentTargetPipeline();

        bool HIDDEN = false;

        std::shared_ptr<tinyrender::ModelMatrixUniform> modelMatrix() const;
        std::shared_ptr<ObjectResources> resources = nullptr;
    };

}