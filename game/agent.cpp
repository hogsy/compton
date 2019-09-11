
#include "../shared.h"

#include "agent.h"

Agent::Agent() = default;
Agent::~Agent() = default;

/////////////////////////////////////////////////

AgentFactory::AgentFactory() {
  plScanDirectory("./agents/", "adf", AgentFactory::RegisterScript, true);
}

AgentFactory::~AgentFactory() {
  registered_.clear();
}

void AgentFactory::Register(const std::string& name, AgentFactory::AgentSpawnFunction func) {
  registered_[name] = func;
}

Agent* AgentFactory::Create(const std::string& name) {
  auto it = registered_.find(name);
  if(it == registered_.end()) {
    return nullptr;
  }

  return it->second();
}

void AgentFactory::RegisterScript(const char* path) {
  // TODO:
}

void AgentFactory::Tick() {
  for(const auto& agent : agents_) {
    agent->Tick();
  }
}

void AgentFactory::Draw() {
  for(const auto& agent : agents_) {
    agent->Draw();
  }
}
