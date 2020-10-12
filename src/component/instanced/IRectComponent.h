#pragma once

#include <glm/glm.hpp>

#include "../style.h"
#include "IComponent.h"

class IRectComponent : public IComponent {
private:
    float m_width, m_height;

public:
    IRectComponent(float width, float height) : IComponent(nullptr), m_width(width), m_height(height) {}
    explicit IRectComponent(IParentComponent* parent, float width, float height)
        : IComponent(parent), m_width(width), m_height(height) {}
    ~IRectComponent() override = default;

    void translate() override;
    void layout() override;
};
