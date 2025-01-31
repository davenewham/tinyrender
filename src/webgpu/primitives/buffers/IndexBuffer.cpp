#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(Context *context, std::vector<uint16_t> indexData) {
    // Buffer base class is initialised
    this->context = context;
    this->type = INDEX;
    this->size = indexData.size() * sizeof(uint16_t);
    this->mapped = false;
    this->initialise(indexData.data());
}