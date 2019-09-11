
#pragma once

class AgentDefinitionData {
 public:
  AgentDefinitionData();
  explicit AgentDefinitionData(const std::string& path);

  std::string GetProperty(const std::string& property, const std::string& def = "") const;

 protected:
 private:
  std::map<std::string, std::string> properties_;
};
