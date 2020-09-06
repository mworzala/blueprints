#include "entity.h"

void Entity::render(Renderer* renderer) {
    renderer->drawQuad(glm::vec2(500, 500));
}
