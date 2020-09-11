#pragma once

#include "Component.h"

class Button : public Component {
private:
    float m_x, m_y;
    float m_width, m_height;
public:
    Button(int x, int y, int width, int height);

    void render(Renderer *renderer) override;
};


