// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "creature.h"

Creature::Creature() {
    memset(id_, 0, sizeof(id_));


}

///////////////////////////////////////////////

CreatureComponent::CreatureComponent(Creature *parent) {
    if(parent == nullptr) {
        // todo, blargh
    }


}
