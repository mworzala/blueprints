#pragma once

class IParentComponent;

class IComponent {
private:
    IParentComponent* m_parent;

    float m_x{}, m_y{}, m_z{};
    float m_width{}, m_height{};

protected:
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void setZ(float z) { m_z = z; }

    void setWidth(float width) { m_width = width; }
    void setHeight(float height) { m_height = height; }

public:
    explicit IComponent(IParentComponent* parent) : m_parent(parent) {}
    virtual ~IComponent() = default;

    IParentComponent* getParent() const { return m_parent; }

    virtual float getX() const { return m_z;};
    virtual float getY() const { return m_y; };

    virtual float getWidth() const { return m_width; };
    virtual float getHeight() const { return m_height; };

    virtual float getCanvasX() const;
    virtual float getCanvasY() const;

    virtual void translate() = 0;
    virtual void layout() = 0;
};