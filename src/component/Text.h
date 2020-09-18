#pragma once

#include <utility>

#include "Component.h"

class Text : public Component {
private:
    std::string m_text;

    float m_width{}, m_height{};

public:
    explicit Text(std::string  text);
    ~Text();

    float getWidth() const override;
    float getHeight() const override;

    std::string getText() const;
    void setText(std::string text);

    void render() override;

    void layout() override;

private:
    void computeSize();
};


