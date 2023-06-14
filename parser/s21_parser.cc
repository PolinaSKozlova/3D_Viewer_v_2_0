#include "s21_parser.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace s21 {

Model ObjParser::Parse(const std::string& filename) {
  std::cout << "file in parser:" << filename << std::endl;
  Model model = ParseData(filename);
  return CalculateScaler(model);
}
Model ObjParser::ParseData(const std::string& filename) {
  using std::getline;
  using std::ifstream;
  using std::istringstream;
  using std::runtime_error;
  using std::string;
  using std::vector;

  ifstream ifs(filename);
  if (!ifs.is_open()) {
    throw runtime_error("Could not open file its me parser ");
  }

  Model model;
  string line;
  while (getline(ifs, line)) {
    istringstream iss(line);
    std::string type{};
    iss >> type;
    if (type == "v") {
      float x{}, y{}, z{};
      if (!(iss >> x >> y >> z)) {
        throw runtime_error("Invalid vertex");
      }
      model.vertices.push_back(x);
      model.vertices.push_back(y);
      model.vertices.push_back(z);
    } else if (type == "f") {
      vector<int> face{};
      string token;
      while (iss >> token) {
        istringstream sub_iss(token);
        string sub_token;
        getline(sub_iss, sub_token, '/');
        if (!sub_token.empty()) {
          face.push_back(std::stoi(sub_token) -
                         1);  // Вот тут эта -1 должна всё исправить
        }
      }
      if (face.size() < 3) {
        throw runtime_error("Invalid face");
      } else if (face.size() > 3) {
        // triangulating faces
        for (size_t i = 1; i < face.size() - 1; ++i) {
          model.faces.push_back(face[0]);
          model.faces.push_back(face[i]);
          model.faces.push_back(face[i + 1]);
        }
      } else {
        model.faces.insert(model.faces.end(), face.begin(), face.end());
      }
    }
  }
  if (model.vertices.empty() || model.faces.empty()) {
    throw runtime_error("No vertices");
  }
  return model;
}

Model ObjParser::CalculateScaler(Model& model) {
  double x_max = model.vertices[0];
  double y_max = model.vertices[1];
  double z_max = model.vertices[2];

  for (size_t i = 0; i < model.vertices.size(); i += 3) {
    if (model.vertices[i] > x_max) {
      x_max = model.vertices[i];
    }
    if (model.vertices[i + 1] > y_max) {
      y_max = model.vertices[i + 1];
    }
    if (model.vertices[i + 2] > z_max) {
      z_max = model.vertices[i + 2];
    }
  }

  double max_val = std::max(std::max(x_max, y_max), z_max);
  model.scaler = 1.0 / max_val;
  return std::move(model);
}

std::ostream& operator<<(std::ostream& os, const Model& model) {
  os << "Vertices:\n";
  for (size_t i = 0; i < model.vertices.size(); i += 3) {
    os << "(" << model.vertices[i] << ", " << model.vertices[i + 1] << ", "
       << model.vertices[i + 2] << ")\n";
  }
  os << "Faces:\n";
  for (size_t i = 0; i < model.faces.size(); i += 3) {
    os << "(" << model.faces[i] << ", " << model.faces[i + 1] << ", "
       << model.faces[i + 2] << ")\n";
  }
  os << "Scaler: " << model.scaler << "\n";
  return os;
}
};  // namespace s21
