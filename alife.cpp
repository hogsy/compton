// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "shared.h"

#include "alife.h"

// Neural Networks

namespace ALIFE
{
	struct Neuron
	{
		Neuron(unsigned int numinputs);

		// Number of inputs into the neuron.
		unsigned int inputs;

		// Weights of each input
		std::vector<double> weight;
	};

	struct NeuronLayer
	{
		NeuronLayer(unsigned int numneurons, unsigned int inperneuron);

		unsigned int num_neurons;

		std::vector<Neuron> neurons;
	};
}

ALIFE::Neuron::Neuron(unsigned int numinputs) : inputs(numinputs)
{
	// Need an additional weight for the bias hence the +1.
//	for(unsigned int i = 0; i < numinputs + 1; ++i)
//		weight.push_back()
}

class Creature
{
public:
protected:
private:
};