
#include "../shared.h"
#include "agent_physics.h"

#define DEFAULT_GRAVITY 0.085f

void PhysicsAgent::SetupProperties(const AgentDefinitionData& adf_loader) {
  SpriteAgent::SetupProperties(adf_loader);

  use_physics_ = adf_loader.GetProperty("physics") == "true";
  elasticity = std::strtof(adf_loader.GetProperty("elasticity", "0").c_str(), nullptr);
}

void PhysicsAgent::TickPhysics() {
  if(!use_physics_) {
    return;
  }

  // gravity
  velocity_.y += DEFAULT_GRAVITY;

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
    velocity_.y *= (-1 * elasticity);
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

void PhysicsAgent::Tick() {
  TickPhysics();

  SpriteAgent::Tick();
}

void PhysicsAgent::Draw() {
  SpriteAgent::Draw();
}
