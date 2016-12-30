// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#if 0
#include <opennn/opennn.h>

#include "Shared.h"

#include "CreatureBrain.h"

class CreatureBrain {
public:
    CreatureBrain(std::string path);
    ~CreatureBrain() {}

    void Load() {}
    void Save();

protected:
private:
    OpenNN::DataSet dataset_;
};

CreatureBrain::CreatureBrain(std::string path) {
    dataset_.set_file_type("csv");
    dataset_.set_data_file_name(path);   // Eventually this should be based on name/id?

    dataset_.load_data();

    OpenNN::NeuralNetwork neuralnet(1, 2, 3);
}

void CreatureBrain::Save() {
    dataset_.save_data();
}
#endif