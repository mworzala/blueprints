#include "IComponent.h"

#include "IParentComponent.h"

float IComponent::getCanvasX() const {
    return (m_parent == nullptr ? 0.0f : m_parent->getCanvasX()) + getX();
}

float IComponent::getCanvasY() const {
    return (m_parent == nullptr ? 0.0f : m_parent->getCanvasY()) + getY();
}
