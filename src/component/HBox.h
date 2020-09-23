#pragma once

#include <vector>

#include "ParentComponent.h"

enum HBoxAlignment {
    TOP, BOTTOM
};

class HBox : public ParentComponent {
private:
    float m_width = 0.0f, m_height = 0.0f;
    float m_spacing;

    HBoxAlignment m_alignment;
public:
    explicit HBox(float spacing = 0.0f, HBoxAlignment alignment = HBoxAlignment::TOP);

    float getWidth() const override;
    float getHeight() const override;

    void render() override;

    void layout() override;
};


