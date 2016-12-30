#pragma once

// todo, need to do the difference phases of the moon etc.
class MoonObject : public SpriteObject {
public:
    MoonObject();
    ~MoonObject() {}

    void Simulate();

protected:
private:
    unsigned int cycle_;
};