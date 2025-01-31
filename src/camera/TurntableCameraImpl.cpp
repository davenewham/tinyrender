#include "camera/TurntableCameraImpl.hpp"


void
TurntableCameraImpl::updateInternalBuffer() {
    auto position = this->getPosition();
    this->viewProjectionMatrix->updateCameraPosition(position);
    this->viewProjectionMatrix->updateViewMatrix(glm::lookAt(position, vec3(0.0f),
                                                 vec3(0, 0, 1)));
}

void
TurntableCameraImpl::onMouseMove(double xpos, double ypos) {
    if(!dragging)
        return;

    vec2 current = vec2((float) -xpos, (float) ypos);
    vec2 delta = startPosition - current;

    phi -= delta.x * mouseSensitivity;
    theta += delta.y * mouseSensitivity;
    theta = glm::clamp(theta, 1e-5f, glm::pi<float>() - 1e-5f);

    startPosition = current;

    updateInternalBuffer();
}

void
TurntableCameraImpl::onMouseButton(GLFWwindow *window, int button, int action, int mods) {
    (void) mods;

    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    switch(action) {
        case GLFW_PRESS:
            dragging = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            startPosition = vec2(-(float)xpos, (float)ypos);
            break;
        case GLFW_RELEASE:
            dragging = false;
            break;
        default:
            break;
    }
}

void
TurntableCameraImpl::onScroll(double xoffset, double yoffset) {
    (void) xoffset;

    r += scrollSensitivity * (float) yoffset;
    r = glm::clamp(r, -2.0f, 5.0f);

    updateInternalBuffer();
}

void
TurntableCameraImpl::onKeyEvent(int key, int scancode, int action, int mods) {
    (void) key;
    (void) scancode;
    (void) action;
    (void) mods;
}

void
TurntableCameraImpl::enableListen(GLFWwindow *window, std::shared_ptr<ViewProjMatrixUniform> vpMatrix) {
    (void) window;
    this->viewProjectionMatrix = vpMatrix;
}

vec3 
TurntableCameraImpl::getPosition() {
    float rr = std::exp(r);
    return vec3(rr*sin(theta)*cos(phi), rr*sin(theta)*sin(phi), rr*cos(theta));
}

void TurntableCameraImpl::onFrame(float dt)
{
    (void) dt;
}

tinyrender::TurntableCamera::TurntableCamera(): Camera(std::make_unique<TurntableCameraImpl>()) {}
tinyrender::TurntableCamera::~TurntableCamera() = default;