#pragma once

#include <set>
#include <tuple>

#include "../event/EventBus.h"

class Mouse {
private:
    static std::set<int> m_buttonsDown;
    static float m_x, m_y;

public:
    static void Init();

    static bool IsPrimaryDown();
    static bool IsSecondaryDown();
    static bool IsButtonDown(int button);

    static std::tuple<float, float> GetMousePosition();

private:
    static void OnEvent(Event* event);
};
