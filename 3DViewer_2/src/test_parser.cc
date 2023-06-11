#include <iostream>
#include <string>

#include "parser.h"

int main() {
  std::string path_to_model = "../models/african_head.obj";
  // std::string path_to_model = "../models/teddy.obj";

  ObjParser parser;
  Model model = parser.Parse(path_to_model);
  
  std::cout << model;
  return 0;
}