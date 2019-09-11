
#pragma once

#include <utility>

#include "../engine/sprite.h"
#include "adf.h"

class Agent {
 public:
  Agent();
  virtual ~Agent();

  virtual void Activate() { is_activated_ = true; }
  virtual void Deactivate() { is_activated_ = false; }

  virtual void TickPhysics();
  virtual void Tick() { TickPhysics(); }
  virtual void Draw() {}

  virtual void SetupProperties(const AgentDefinitionData& adf_loader);

  virtual void Restore();
  virtual void Save();

  virtual void SetPosition(PLVector2 pos) {
    old_position_ = position_;
    position_ = pos;
  }

  virtual void SetVelocity(PLVector2 vel) {
    old_velocity_ = velocity_;
    velocity_ = vel;
  }

  PLVector2 GetPosition() { return position_; }
  PLVector2 GetVelocity() { return velocity_; }

  void SetDescriptor(const std::string& desc) {
    description_ = desc;
  }

 protected:
  PLVector2 position_, old_position_;
  PLVector2 velocity_, old_velocity_;

 private:
  std::string description_{"none"};

  PLVector2 bounds_{0, 0};

  bool use_physics_{false};

  bool is_activated_{false};
  bool is_grounded_{false};
};

class AgentFactory {
 private:
  AgentFactory();
  AgentFactory(const AgentFactory&) {}
  AgentFactory& operator=(const AgentFactory&) { return *this; }

  static void RegisterScript(const char* path);

  typedef Agent* (* AgentSpawnFunction)();
  struct AgentClassData {
    AgentClassData(AgentSpawnFunction func, AgentDefinitionData data) :
        spawn_function(func),
        spawn_data(std::move(data)) {}
    AgentSpawnFunction spawn_function{nullptr};
    AgentDefinitionData spawn_data;
  };
  typedef std::map<std::string, AgentClassData> AgentMap;
  AgentMap registered_;

  std::vector<Agent*> agents_;
 public:
  ~AgentFactory();

  static AgentFactory* Get() {
    static AgentFactory instance;
    return &instance;
  }

  void Tick();
  void Draw();

  void RegisterScripts();
  void Register(const std::string& name, AgentSpawnFunction func);
  void Register(const std::string& name, const std::string& baseclass, const AgentDefinitionData& data);
  Agent* Create(const std::string& name);
};

#define IMPLEMENT_FACTORY(a) \
  static Agent* Create() { return new (a)(); }
