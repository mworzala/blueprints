#pragma once

#include <vector>

#include "Component.h"

class HBox : public Component {
private:
    std::vector<Component*> m_children;
    float m_width = 0.0f, m_height = 0.0f;

    float m_spacing;
public:
    explicit HBox(float spacing = 0.0f);

    void addChild(Component* child);

    float getWidth() const override;
    float getHeight() const override;

    void render(Renderer *renderer) override;

    void layout() override;
};


