// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#pragma once

#include "../../../shared.h"

class CreatureComponent;

class Creature {
public:
    Creature();
    ~Creature();

protected:

private:
    char id_[1024];

    CreatureComponent **components_;
};

class CreatureComponent {
public:
    CreatureComponent(Creature *parent);
    ~CreatureComponent();

    const char *GetIdentification() {
        return id_;
    }

    virtual void SaveState();

protected:
private:
    char id_[1024];

    Creature *parent_;
};

/* SKELETON     */
class CreatureSkeleton : public CreatureComponent {

};

/* RECEPTORS    */
/*
 * optical
 * auditory
 * gustatory (taste)
 * olfactory (smell)
 * somatic (touch)
 * nociception (pain)
 */

class CreatureReceptor : public CreatureComponent {
    CreatureReceptor() {}
};

class CreatureOpticalReceptor : public CreatureReceptor {};
class CreatureAuditoryReceptor : public CreatureReceptor {};
class CreatureGustatoryReceptor : public CreatureReceptor {};
class CreatureOlfactoryReceptor : public CreatureReceptor {};
class CreatureSomaticReceptor : public CreatureReceptor {};
class CreatureNociceptionReceptor : public CreatureReceptor {};

/* ORGANS   */
/* Basic design of this is essentially based
 * on mammals. Trying to support every other
 * type of life would be... complicated
 */

class CreatureOrgan : public CreatureComponent {
public:
    CreatureOrgan();
    ~CreatureOrgan();

protected:
private:
    float size_;    // radial, capacity to organ is handled case-by-case

    CreatureReceptor *receptor_;
};

class CreatureOrganEye : public CreatureOrgan {};           // 0 : SENSORY :
class CreatureOrganToungue : public CreatureOrgan {};       // 0 : SENSORY : <MOUTH
class CreatureOrganNose : public CreatureOrgan {};          // 0 : SENSORY :
class CreatureOrganEar : public CreatureOrgan {};           // 0 : SENSORY :
class CreatureOrganSkin : public CreatureOrgan {};          // 1 : SENSORY :
class CreatureOrganLung : public CreatureOrgan {};          // 1
class CreatureOrganHeart : public CreatureOrgan {};         // 1
class CreatureOrganKidney : public CreatureOrgan {};        // 1
class CreatureOrganLiver : public CreatureOrgan {};         // 1
class CreatureOrganEsophagus : public CreatureOrgan {};     // 1 : >STOMACH
class CreatureOrganStomach : public CreatureOrgan {};       // 1
class CreatureOrganSpleen : public CreatureOrgan {};        // 1

class CreatureOrganBrain : public CreatureOrgan {
public:
    CreatureOrganBrain();
    ~CreatureOrganBrain() {}

    void Save();

protected:
private:
    OpenNN::DataSet dataset_;
};

class CreatureOrganGallbladder : public CreatureOrgan {};   // 0 : LIVER>GALLBLADDER>INTESTINE
class CreatureOrganBladder : public CreatureOrgan {};       // 1
class CreatureOrganLarynx : public CreatureOrgan {};
class CreatureOrganPenis : public CreatureOrgan {};         // 1 /
class CreatureOrganOvary : public CreatureOrgan {};         // 1 /