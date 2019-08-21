// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef GTFS2GRAPH_CONFIG_GTFS2GEOCONFIG_H_
#define GTFS2GRAPH_CONFIG_GTFS2GEOCONFIG_H_

#include <string>

namespace gtfs2graph {
namespace config {

struct Config {
  std::string projectionString;
  std::string inputFeedPath;

  bool ignoreGtfsDistances;
  bool ignoreDirections;

  size_t stationAggrLevel;

  double maxAggrDistance;
  double stationAggrDistance;

  uint8_t useMots;
};

}  // namespace config
}  // namespace gtfs2graph

#endif  // GTFS2GRAPH_CONFIG_GTFS2TOPOCONFIG_H_