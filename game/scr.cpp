
#include "../shared.h"

#include "scr.h"

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

ScrLoader::ScrLoader(const std::string& path) {
  std::ifstream fp(path);
  if(!fp) {
    PRINT_ERROR("Failed to open SCR file, \"%s\"!\n", path.c_str());
  }

  std::string ln;
  while(std::getline(fp, ln)) {
    ParseLine(ln);
  }
}

ScrLoader::~ScrLoader() = default;

void ScrLoader::ParseLine(const std::string& str) {

}
