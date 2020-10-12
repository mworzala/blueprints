#include "InstancedRenderer.h"

int InstancedRenderer::drawCalls = 0;


void InstancedRenderer::InitPolygonRenderer() {

}

void InstancedRenderer::InitFontRenderer() {

}

void InstancedRenderer::Render(glm::mat4 projectionView) {
    drawCalls = 0;
    InstancedQuadRenderer::Render(projectionView);
}

InstancedQuad *InstancedRenderer::AllocateQuad() {
    return InstancedQuadRenderer::AllocateQuad();
}

void InstancedRenderer::FreeQuad(int index) {
    InstancedQuadRenderer::FreeQuad(index);
}
