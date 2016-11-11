// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef TRANSITMAP_GRAPH_TRANSITGRAPH_H_
#define TRANSITMAP_GRAPH_TRANSITGRAPH_H_

#include <proj_api.h>
#include <string>
#include <set>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>

#include "./../util/Geo.h"
#include "./OrderingConfiguration.h"
#include "./Node.h"
#include "./Edge.h"

namespace bg = boost::geometry;

namespace transitmapper {
namespace graph {

class TransitGraph {

 public:
  explicit TransitGraph(const std::string& name, const std::string& projStr);

  ~TransitGraph();

  Node* addNode(Node* n);
  Edge* addEdge(Node* from, Node* to);
  Edge* getEdge(Node* from, Node* to);

  void deleteNode(Node* n);
  void deleteEdge(Node* from, Node* to);
  bool containsNode(Node* n) const;

  const std::set<Node*>& getNodes() const;
  std::set<Node*>* getNodes();

  Node* getNodeByStop(const gtfs::Stop* s) const;
  Node* getNodeByStop(const gtfs::Stop* s, bool getParent) const;

  Node* getNearestNode(const util::geo::Point& p, double maxD) const;

  projPJ getProjection() const;
  const boost::geometry::model::box<util::geo::Point>& getBoundingBox() const;

  double getScore() const;
  double getScore(const Configuration& c) const;

  const Configuration& getConfig() const;
  void setConfig(const Configuration&);

 private:
  std::string _name;
  std::set<Node*> _nodes;

  Configuration _config;

  boost::geometry::model::box<util::geo::Point> _bbox;

  projPJ _proj;
};

}}

#endif  // TRANSITMAP_GRAPH_TRANSITGRAPH_H_