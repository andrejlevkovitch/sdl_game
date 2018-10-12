// triangles_parser.cpp

#include "triangles_parser.hpp"

#include <sstream>
#include <tinyxml.h>

bool levi::triangles_parser(const std::string &file_name,
                            std::vector<vertex> &vertices,
                            std::vector<uint32_t> &indexes) {

  ::TiXmlDocument xml_doc;

  xml_doc.LoadFile(file_name);

  auto root = xml_doc.RootElement();
  if (!root) {
    return false;
  }

  auto vertex_array = root->FirstChildElement();
  if (!vertex_array || vertex_array->Value() != std::string{"vertices"}) {
    return false;
  }

  for (auto i = vertex_array->FirstChildElement(); i != nullptr;
       i = i->NextSiblingElement()) {
    if (i->Value() == std::string{"vertex"}) {
      double x{};
      double y{};
      double r{};
      double g{};
      double b{};

      i->Attribute("x", &x);
      i->Attribute("y", &y);
      i->Attribute("r", &r);
      i->Attribute("g", &g);
      i->Attribute("b", &b);

      vertices.push_back(vertex(x, y, r, g, b));
    }
  }

  auto index_array = vertex_array->NextSiblingElement();
  if (!index_array || index_array->Value() != std::string{"indexes"}) {
    return false;
  }

  std::stringstream ss;
  const char *array = index_array->Attribute("array");
  if (!array) {
    return false;
  }
  ss << array;

  uint32_t index{};
  while (ss >> index) {
    indexes.push_back(index);
  }

  return true;
}
