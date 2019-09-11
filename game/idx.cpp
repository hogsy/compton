
#include <sstream>
#include "../shared.h"
#include "idx.h"

namespace vc {
IdxLoader::IdxLoader(const std::string& path) {
  std::ifstream fp(path);
  if (!fp) {
    PRINT_ERROR("Failed to open IDX file, \"%s\"!\n", path.c_str());
  }

  std::string line;
  while (std::getline(fp, line)) {
    list_.push_back(line);
  }
}

IdxLoader::~IdxLoader() = default;
}
