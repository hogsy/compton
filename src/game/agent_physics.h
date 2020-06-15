
#pragma once

#include "agent_sprite.h"

class PhysicsAgent : public SpriteAgent {
 public:
  IMPLEMENT_FACTORY(PhysicsAgent)

  void SetupProperties(const AgentDefinitionData& adf_loader) override;

  void Tick() override;
  void Draw() override;

  virtual void Impact(Agent* agent);

  PLVector2 GetVelocity() { return velocity_; }
  virtual void SetVelocity(PLVector2 vel) {
    old_velocity_ = velocity_;
    velocity_ = vel;
  }

 protected:
  PLVector2 velocity_, old_velocity_;

 private:
  void TickPhysics();

  PLVector2 bounds_{0, 0};

  float elasticity{0};

  bool use_physics_{false};
  bool is_grounded_{false};
};
