
#include "../shared.h"

#include "agent_sprite.h"

void SpriteAgent::SetupProperties(const AgentDefinitionLoader & adf_loader) {
  Agent::SetupProperties(adf_loader);

  std::string sprite_path = adf_loader.GetProperty("sprite");
  if(!sprite_path.empty()) {
    sprite_ = new Sprite(vc::LoadImage(sprite_path));
  }
}

void SpriteAgent::Tick() {
  Agent::Tick();

  sprite_->position_ = GetPosition();
}

void SpriteAgent::Draw() {
  sprite_->Draw();
}
