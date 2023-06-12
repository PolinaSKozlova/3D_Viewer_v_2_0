#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

struct Model {
  Model() : vertices{}, faces{}, scaler{} {}
  Model(Model&& other) noexcept
      : vertices(std::move(other.vertices)),
        faces(std::move(other.faces)),
        scaler(other.scaler) {}

  Model(const Model& other) = delete;

  Model& operator=(Model&& other) noexcept {
    if (this != &other) {
      vertices = std::move(other.vertices);
      faces = std::move(other.faces);
      scaler = other.scaler;
    }
    return *this;
  }

  Model& operator=(const Model& other) = delete;

  std::vector<float> vertices;
  std::vector<unsigned int> faces;
  double scaler;
};

class ObjParser {
 public:
  Model Parse(const std::string& filename);

 private:
  Model ParseData(const std::string& filename);
  Model CalculateScaler(Model& model);
};

std::ostream& operator<<(std::ostream& os, const Model& model);

#endif  // SRC_PARSER_H_
