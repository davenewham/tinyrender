#include "ColorUniform.hpp"

ColorUniform::ColorUniform(Context *context) {
    this->uniform.color = glm::vec3(0.0);

    // Buffer base class is initialised
    this->context = context;
    this->type = UNIFORM;
    this->size = sizeof(ColorUniforms);
    this->mapped = false;
    this->initialise(&uniform);
}

void
ColorUniform::set(glm::vec3 color) {
    this->uniform.color = color;
    context->queue.writeBuffer(underlying, offsetof(ColorUniforms, color),
                               &uniform.color,sizeof(ColorUniforms::color));
}

