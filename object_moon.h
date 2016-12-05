#pragma once

// todo, need to do the difference phases of the moon etc.
class Moon : public SpriteObject {
public:
    Moon();
    ~Moon() {}

    void Simulate();

protected:
private:
    unsigned int cycle_;
};