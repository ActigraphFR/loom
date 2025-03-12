// Copyright 2017, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef SHARED_LINEGRAPH_LINE_H_
#define SHARED_LINEGRAPH_LINE_H_

#include <string>
#include <vector>

namespace shared {
namespace linegraph {

class Line {
 public:
  Line(const std::string& id, const std::string& label,
       const std::string& color, const std::string& typeLine = "normal")
      : _id(id), _label(label), _color(color), _typeLine(typeLine) {}

  const std::string& id() const { return _id; }
  const std::string& label() const { return _label; }
  const std::string& color() const { return _color; }
  const std::string& typeLine() const { return _typeLine; } // Getter pour typeLine

  void setColor(const std::string& c) { _color = c; }
  void setTypeLine(const std::string& t) { _typeLine = t; } // Setter pour typeLine

 private:
  std::string _id, _label, _color, _typeLine; // Ajout de _typeLine
};
}
}

#endif  // SHARED_LINEGRAPH_LINE_H_