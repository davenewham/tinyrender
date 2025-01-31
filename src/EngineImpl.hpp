#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.hpp>
#include <memory>
#include <vector>
#include <map>

#include "objects/Object.hpp"
#include "Engine.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>
#endif

#include "Engine.hpp"
#include "State.hpp"
#include "webgpu/Context.hpp"
#include "webgpu/pipelines/Pipeline.hpp"
#include "webgpu/Scene.hpp"
#include "webgpu/pipelines/TrianglePipeline.hpp"
#include "webgpu/pipelines/TexturedTrianglePipeline.hpp"
#include "webgpu/pipelines/WavesPipeline.hpp"
#include "webgpu/pipelines/VoxelPipeline.hpp"
#include "webgpu/primitives/textures/Texture2D.hpp"
#include "camera/Camera.hpp"
#include "camera/CameraImpl.hpp"
#include "objects/Object.hpp"


class EngineImpl {
public:
    EngineImpl();
    ~EngineImpl();

    void launch(int width, int height); // Launch GLFW window

    bool isRunning();
    void onFrame();

    void addObject(std::shared_ptr<tinyrender::Object>);

    void onResize(int, int);
    void setCamera(std::shared_ptr<tinyrender::Camera> controller);

    GLFWwindow *getWindow() { return window; };

    std::shared_ptr<Context> getContext() { return context; }
    std::shared_ptr<Scene> getScene() { return scene; }

    CameraImpl* camera = nullptr;

    // Objects in scene
    std::vector<std::shared_ptr<tinyrender::Object>> objects;
protected:
    // GLFW
    GLFWwindow *window;
    #ifdef __EMSCRIPTEN__
    EmscriptenFullscreenStrategy strategy;
    #endif

    // State for passing to objects
    std::shared_ptr<State> state;

    // WebGPU
    std::shared_ptr<Context> context = nullptr;
    std::shared_ptr<Scene> scene = nullptr;

    // Pipeline objects
    std::map<ObjectPipeline, std::unique_ptr<Pipeline>> pipelines;
};

void onWindowResize(GLFWwindow* window, int, int);
void onWindowMouseMove(GLFWwindow* window, double xpos, double ypos);
void onWindowMouseButton(GLFWwindow* window, int button, int action, int mods);
void onWindowScroll(GLFWwindow* window, double xoffset, double yoffset);
void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods);

#ifdef __EMSCRIPTEN__
__attribute__((unused))
static EM_BOOL EmscriptenWindowResizedCallback(int eventType, const void *event, void *userData);
#endif
