
#pragma once

class ScrHandle {

};

class ScrLoader {
 public:
  explicit ScrLoader(const std::string& path);
  ~ScrLoader();

 protected:
 private:
  void ParseLine(const std::string& str);
};