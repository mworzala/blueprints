#pragma once

#include "Component.h"

class ParentComponent : public Component {
protected:
    std::vector<Component *> m_children;

public:
    void addChild(Component *child) {
        m_children.push_back(child);
        layout();
    }

    void handleClick(float x, float y) override {
        Component::handleClick(x, y);

        for (auto* child : m_children) {
            if (child->contains(x, y))
                child->handleClick(x - child->getX(), y - child->getY());
        }
    }
};