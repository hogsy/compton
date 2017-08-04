// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "creature.h"

#include <jansson.h>

CreatureOrganBrain::CreatureOrganBrain() {
    dataset_.set_file_type("csv");
    dataset_.set_data_file_name("");   // Eventually this should be based on name/id?

    dataset_.load_data();

    char path[PL_SYSTEM_MAX_PATH] = { '\0' };
    snprintf(path, PL_SYSTEM_MAX_PATH, "./c/%s/memory", GetIdentification());

    json_error_t error;
    json_t *root = json_load_file(path, 0, &error);
    if(root == nullptr) {

    }

    OpenNN::NeuralNetwork neuralnet(1, 2, 3);
}

void CreatureOrganBrain::Save() {
    dataset_.save_data();
}