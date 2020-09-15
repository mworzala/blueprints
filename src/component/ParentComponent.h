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
};