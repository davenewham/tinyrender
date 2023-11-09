#pragma once

#include "../Buffer.hpp"

namespace tinyrender {

    class ColorUniform : public Buffer {
    protected:
        ColorUniforms uniform{};
    public:
        explicit ColorUniform(Context *context);
        void set(glm::vec3 color);
    };

}
