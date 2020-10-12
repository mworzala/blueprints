#pragma once

#include <vector>

#include "IComponent.h"

class IParentComponent : public IComponent {
private:
    std::vector<IComponent*> m_children;

public:
    IParentComponent() : IComponent(nullptr) {}
    explicit IParentComponent(IParentComponent* parent) : IComponent(parent) {}
    ~IParentComponent() override {
        for (auto* child : m_children)
            delete child;
    }

};