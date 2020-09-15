#pragma once

#include <vector>

#include "ParentComponent.h"

class HBox : public ParentComponent {
private:
    float m_width = 0.0f, m_height = 0.0f;
    float m_spacing;
public:
    explicit HBox(float spacing = 0.0f);

    float getWidth() const override;
    float getHeight() const override;

    void render(Renderer *renderer) override;

    void layout() override;
};


