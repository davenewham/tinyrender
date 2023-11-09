#pragma once

#include "webgpu/webgpu.hpp"
#include "primitives/textures/Texture2D.hpp"
#include "primitives/buffers/uniforms/LightingUniform.hpp"
#include "primitives/shaders/TexturedShader.hpp"
#include "primitives/shaders/ColoredShader.hpp"
#include "primitives/shaders/WavesShader.hpp"
#include "primitives/buffers/uniforms/ViewProjMatrixUniform.hpp"

/*
 * Scene
 * Encapsulates any abstracted WebGPU resources that many classes require access to.
 */

struct Scene {
    // Depth Buffer, pipelines will require read/write to this.
    std::shared_ptr<tinyrender::Texture2D::Texture> depthTexture;

    void buildDepthBuffer(const std::shared_ptr<Context>& context) {
        depthTexture = std::make_shared<tinyrender::Texture2D::common::DefaultDepthTexture>(context.get());
    }

    // Lighting data
    std::shared_ptr<tinyrender::LightingUniform> lightingUniform;

    // Shaders
    std::shared_ptr<tinyrender::TexturedShader> texturedShader;
    std::shared_ptr<tinyrender::ColoredShader> coloredShader;
    std::shared_ptr<tinyrender::WavesShader> wavesShader;

    // Projection * View Matrix
    std::shared_ptr<tinyrender::ViewProjMatrixUniform> viewProjUniform;
    std::vector<wgpu::BindGroupEntry> viewProjUniformBindGroupData;
    wgpu::BindGroup coloredViewProjBindGroup = nullptr;
    wgpu::BindGroup texturedViewProjBindGroup = nullptr;
    wgpu::BindGroup wavesViewProjBindGroup = nullptr;

    void buildViewProj(const std::shared_ptr<Context>& context) {
        if(texturedShader == nullptr || coloredShader == nullptr)
            throw std::runtime_error("Shaders must be initialised first");

        this->viewProjUniform = std::make_shared<tinyrender::ViewProjMatrixUniform>(context.get(), vec3(5.0, -5.0, 5.0), vec3(0, 0, 0));

        // Build bind group
        viewProjUniformBindGroupData = std::vector<BindGroupEntry>(2);
        viewProjUniformBindGroupData[0] = viewProjUniform->generateUniformBindGroupEntry(0);
        viewProjUniformBindGroupData[1] = lightingUniform->generateUniformBindGroupEntry(1);

        {
            BindGroupDescriptor bindGroupDesc;
            bindGroupDesc.entryCount = (uint32_t) viewProjUniformBindGroupData.size();
            bindGroupDesc.entries = viewProjUniformBindGroupData.data();
            bindGroupDesc.layout = coloredShader->viewProjBindGroupLayout();
            coloredViewProjBindGroup = context->device.createBindGroup(bindGroupDesc);
        }

        {
            BindGroupDescriptor bindGroupDesc;
            bindGroupDesc.entryCount = (uint32_t) viewProjUniformBindGroupData.size();
            bindGroupDesc.entries = viewProjUniformBindGroupData.data();
            bindGroupDesc.layout = texturedShader->viewProjBindGroupLayout();
            texturedViewProjBindGroup = context->device.createBindGroup(bindGroupDesc);
        }

        {
            BindGroupDescriptor bindGroupDesc;
            bindGroupDesc.entryCount = (uint32_t) viewProjUniformBindGroupData.size();
            bindGroupDesc.entries = viewProjUniformBindGroupData.data();
            bindGroupDesc.layout = wavesShader->viewProjBindGroupLayout();
            wavesViewProjBindGroup = context->device.createBindGroup(bindGroupDesc);
        }
    }

    ~Scene() {
        coloredViewProjBindGroup.release();
        texturedViewProjBindGroup.release();
        wavesViewProjBindGroup.release();
    }
};
