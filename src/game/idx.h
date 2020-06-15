
#pragma once

namespace vc {
class IdxLoader {
 public:
  explicit IdxLoader(const std::string& path);
  ~IdxLoader();

  std::string GetIndex(unsigned int i) {
    if (i >= list_.size()) {
      return "";
    }

    return list_[i];
  }

  unsigned int GetNumIndices() {
    return list_.size();
  }

 protected:
 private:
  std::vector<std::string> list_;
};
}
