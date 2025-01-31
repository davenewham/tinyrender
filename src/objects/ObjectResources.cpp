#include "ObjectResources.hpp"


ObjectResources::ObjectResources(Context *context, Scene *scene, std::shared_ptr<AttributeBuffer> attrs, ObjectPipeline target): context(context), scene(scene), attributeBuffer(attrs) {
    this->modelMatrix = std::make_shared<ModelMatrixUniform>(context);
    this->color = std::make_shared<ColorUniform>(context);

    this->resetBindGroup(target);
}

ObjectResources::~ObjectResources() {
    if(bindGroup != nullptr)
        bindGroup.release();
}

void ObjectResources::resetBindGroup(ObjectPipeline target) {
    /*
    * To expose uniforms to the shader program in WebGPU, you have to instantiate a 'BindGroup' which is bound to
    * a particular shader program. This method refreshes the bindGroup property to be created against the correct
    * shader, avoiding any errors. Note the order uniforms are passed into the BindGroupDescriptor must 1-1 match
    * the shader program.
    */
    this->targetPipeline = target;
    if(bindGroup != nullptr) {
        bindGroup.release();
    }

    std::vector<std::shared_ptr<Uniform>> uniforms;
    switch(target) {
        case TexturedTriangle:
            uniforms = { 
                modelMatrix,
                std::make_shared<TextureUniform>(texture),
                std::make_shared<TextureSamplerUniform>(texture) 
            };
            break;
        case ColoredTriangle:
            uniforms = {
                modelMatrix, 
                color 
            };
            break;
        case Waves:
            uniforms = {
                modelMatrix,
                std::make_shared<TextureUniform>(texture),
                std::make_shared<TextureSamplerUniform>(texture),
                maxDisplacement,
                color
            };
            break;
        case Voxels:
            uniforms = {
                globalModelMatrix,
                modelMatrix,
                std::make_shared<TextureUniform>(voxelColors),
                std::make_shared<TextureSamplerUniform>(voxelColors)
            };
            break;
    }

    // Object class must have correctly populated the uniforms
    for(auto &unif : uniforms) {
        (void) unif;
        assert(unif != nullptr);
    }
    
    int n = uniforms.size();
    bindGroupData = std::vector<BindGroupEntry>(n);
    for(int i = 0; i < n; i++)
        bindGroupData[i] = uniforms[i]->generateUniformBindGroupEntry(i);
    
    BindGroupDescriptor bindGroupDesc;
    bindGroupDesc.layout = scene->shaders[target]->objectBindGroupLayout();
    bindGroupDesc.entryCount = (uint32_t)bindGroupData.size();
    bindGroupDesc.entries = bindGroupData.data();
    bindGroup = context->device.createBindGroup(bindGroupDesc);
}

