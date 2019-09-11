
#pragma once

#include "../engine/sprite.h"

class Agent {
 public:
  Agent();
  virtual ~Agent();

  virtual void Tick() {};
  virtual void Draw() {};

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

 protected:
 private:
  std::string description_{"none"};

  PLVector2 position_, old_position_;
  PLVector2 velocity_, old_velocity_;

  Sprite* sprite_{nullptr};
};

class AgentFactory {
 private:
  AgentFactory();
  AgentFactory(const AgentFactory&) {}
  AgentFactory &operator=(const AgentFactory&) { return *this; }

  static void RegisterScript(const char* path);

  typedef Agent*(*AgentSpawnFunction)();
  typedef std::map<std::string, AgentSpawnFunction> AgentMap;
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

  void Register(const std::string& name, AgentSpawnFunction func);
  Agent* Create(const std::string& name);
};

#define IMPLEMENT_FACTORY(a) \
  static Agent* Create() { return new (a)(); }
