
#include "../shared.h"

#include "agent.h"
#include "agent_sprite.h"
#include "agent_toy.h"

Agent::Agent() = default;
Agent::~Agent() = default;

void Agent::SetupProperties(const AgentDefinitionData& adf_loader) {
  if(adf_loader.GetProperty("physics") == "true") {
    use_physics_ = true;
  }
}

void Agent::Restore() {

}

void Agent::Save() {

}

void Agent::TickPhysics() {
  if(!use_physics_) {
    return;
  }


  // gravity
  velocity_.y += 0.085f;

  if (velocity_.x > 0) {
    velocity_.x -= 0.05f;
  } else if (velocity_.x < 0) {
    velocity_.x += 0.05f;
  }

  if (is_grounded_) {
    if (velocity_.x < 0.05f && velocity_.x > -0.05f) {
      velocity_.x = 0;
    }
  }

  if ((position_.x <= 0) || (position_.x >= 63)) {
    velocity_.x *= -1;
  }

  position_ += velocity_;

  if (position_.y < 0) {
    position_.y = 0;
    velocity_.y *= -1;
  }

  if (position_.y >= (128 + bounds_.x - 2)) {
    // TODO: uncomment for bouncy balls!
    //velocity_.y *= -1;
    position_.y = (128 + bounds_.y - 2);
    is_grounded_ = true;
  } else {
    is_grounded_ = false;
  }

  if (position_.x < 0) {
    position_.x = 0;
    //  velocity_.x *= -1;
  } else if (position_.x > DISPLAY_WIDTH - 1) {
    position_.x = DISPLAY_WIDTH - 1;
    //  velocity_.x *= -1;
  }
}

/////////////////////////////////////////////////

AgentFactory::AgentFactory() {
  // register all the base classes
  Register("Sprite", SpriteAgent::Create);
  Register("Toy", ToyAgent::Create);
}

AgentFactory::~AgentFactory() {
  registered_.clear();
}

void AgentFactory::Register(const std::string& name, AgentSpawnFunction func) {
  registered_.insert(std::pair<std::string, AgentClassData>(name, AgentClassData(func, AgentDefinitionData())));
}

void AgentFactory::Register(const std::string& name, const std::string& baseclass, const AgentDefinitionData& data) {
  auto it = registered_.find(baseclass);
  if(it == registered_.end()) {
    PRINT_WARN("Failed to find baseclass, \"%s\"!\n", baseclass.c_str());
    return;
  }

  registered_.insert(std::pair<std::string, AgentClassData>(name, AgentClassData(
      it->second.spawn_function, data
      )));
}

Agent* AgentFactory::Create(const std::string& name) {
  auto it = registered_.find(name);
  if(it == registered_.end()) {
    return nullptr;
  }

  Agent* agent = it->second.spawn_function();
  agent->SetupProperties(it->second.spawn_data);
  agents_.push_back(agent);
  return agent;
}

void AgentFactory::RegisterScript(const char* path) {
  AgentDefinitionData adf_loader(path);

  std::string classname = adf_loader.GetProperty("classname");
  if(classname.empty()) {
    PRINT_WARN("Invalid classname, unable to register \"%s\"!\n", path);
    return;
  }

  std::string baseclass = adf_loader.GetProperty("baseclass");
  if(baseclass.empty()) {
    PRINT_WARN("Invalid baseclass, unable to register \"%s\"!\n", path);
    return;
  }

  Get()->Register(classname, baseclass, adf_loader);
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

void AgentFactory::RegisterScripts() {
  // now load in all of our agents (these all depend on the base classes)
  plScanDirectory("./agents", "adf", AgentFactory::RegisterScript, true);
}
