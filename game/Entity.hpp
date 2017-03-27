// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#pragma once

class Entity {
public:
    Entity() {}
    ~Entity() {}

protected:
private:
    PLVector3D position;
    PLVector3D rotation;

    PLVector3D forward, right, up;
};
