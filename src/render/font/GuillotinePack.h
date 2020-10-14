#pragma once

#include <vector>
#include <tuple>

#define EMPTY_SPACE(width, height) { 0, 0, width, height }
struct RectSpace {
    int x, y, w, h;

    int area() const { return w * h; }
    bool canFit(int width, int height) const { return w >= width && h >= height; }

    std::tuple<RectSpace, RectSpace> insert(int width, int height) const {
        if (width > height) {
            // Split horizontally
            return {
                    {x + width, y,              w - width, /**/height},      // Right
                    {x,         y + height, /**/w,             h - height}   // Bottom
            };
        } else {
            // Split vertically
            return {
                    {x,         y + height, /**/width,         h - height},  // Bottom
                    {x + width, y,              w - width, /**/h}            // Right
            };
        }
    }
};

typedef unsigned char Hex;

class GuillotinePack {
private:
    static int m_channels, m_totalWidth;
    static std::vector<RectSpace>* m_space;

    static Hex* m_image;

public:
    static void StartPack(int channels, int width, int height) {
        m_channels = channels;
        m_totalWidth = width;
        m_space = new std::vector<RectSpace>[channels];
        for (int i = 0; i < channels; i++) // Fill with entire space
            m_space[i].push_back(EMPTY_SPACE(width, height));

        m_image = new Hex[width * height * channels]{0};
    }

    static Hex* EndPack() {
        delete[] m_space;

        return m_image;
    }

    static bool PackData_t(int width, int height, Hex* data) {
        for (int channel = 0; channel < m_channels; channel++) {
            // Attempt to insert at each channel
            int optimal = TryInsert(channel, width, height);
            if (optimal == -1) continue; // Could not fit anywhere inside

            // Remove and re add the old/new empty spaces.
            auto target = m_space[channel][optimal];
            m_space[channel].erase(m_space[channel].begin() + optimal);
            auto [sm, lg] = target.insert(width, height);
            m_space[channel].insert(m_space[channel].end(), {sm, lg});

            // Write data to channel
            Write2dSubData(channel, target.x, target.y, width, height, data);
            return true;
        }
        return false;
    }

private:
    static int TryInsert(int channel, int width, int height) {
        int min = -1, minArea = 2147483647;
        for (int i = 0; i < m_space[channel].size(); i++) {
            auto rect = m_space[channel][i];
            if (!rect.canFit(width, height)) continue;

            auto [sm, lg] = rect.insert(width, height);
            int area = sm.area();
            if (area < minArea) {
                min = i; minArea = area;
            }
        }
        return min;
    }

//    static void Write2dSubData(int channel, int dx, int dy, int dw, int dh, const Hex* data) {
//        for (int y = 0; y < dh; y++) {
//            for (int x = 0; x < dw; x++) {
//                //                  row loc                    col loc    channels     offset
//                int imageIndex = (((dy + y) * m_totalWidth) + (dx + x)) * m_channels + channel;
//                int dataIndex  = (y * dw) + x;
//                m_image[imageIndex] = data[dataIndex];
//            }
//        }
//    }
    static void Write2dSubData(int channel, int x, int y, int w, int h, const unsigned char* data) {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                m_image[(((y + i) * m_totalWidth) + (x + j)) * m_channels + channel] = data[(i * w) + j];
            }
        }
    }
};

// Vars
int GuillotinePack::m_channels;
int GuillotinePack::m_totalWidth;
std::vector<RectSpace>* GuillotinePack::m_space;
Hex* GuillotinePack::m_image;


