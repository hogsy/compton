
#include "../../shared.h"
#include "brain.h"

namespace vc {
Drive::Drive(const std::string& name) :
    description_(name) {
}
Drive::~Drive() = default;

Brain::Brain() = default;
Brain::~Brain() = default;

void Brain::Tick() {

}
}
