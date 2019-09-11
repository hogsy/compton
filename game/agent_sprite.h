
#pragma once

#include "agent.h"

class SpriteAgent : public Agent {
 public:
  IMPLEMENT_FACTORY(SpriteAgent)

  virtual void SetupProperties(const AgentDefinitionData& adf_loader);

  virtual void Tick();
  void Draw() override;

 private:
  Sprite* sprite_{nullptr};
};
