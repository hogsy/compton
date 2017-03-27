// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

class Object;

struct ObjectFactory {
    static Object *Create(const char *name);
};