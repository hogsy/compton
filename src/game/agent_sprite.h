
#pragma once

#include "agent.h"

class SpriteAgent : public Agent {
 public:
  IMPLEMENT_FACTORY(SpriteAgent)

  void SetupProperties(const AgentDefinitionData& adf_loader) override;

  void Tick() override;
  void Draw() override;

 protected:
  Sprite* sprite_{nullptr};
};
