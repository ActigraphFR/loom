// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <float.h>
#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <string>
#include "gtfs2graph/config/ConfigReader.h"
#include "util/log/Log.h"
#include "util/String.h"
#include "ad/cppgtfs/gtfs/flat/Route.h"

using gtfs2graph::config::ConfigReader;
namespace opts = boost::program_options;

using std::string;
using std::exception;
using std::vector;

// _____________________________________________________________________________
ConfigReader::ConfigReader() {}

// _____________________________________________________________________________
void ConfigReader::read(Config* cfg, int argc, char** argv) const {
  string VERSION_STR = " - unversioned - ";

  opts::options_description generic("General");
  generic.add_options()("version", "output version")(
      "help,?", "show this message")("verbose,v", "verbosity level");

  std::string motStr;

  opts::options_description config("Output");
  config.add_options()(
      "projection",
      opts::value<std::string>(&(cfg->projectionString))
          ->default_value("+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 "
                          "+lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m "
                          "+nadgrids=@null +wktext  +no_defs"),
      "map projection as proj4 string")(
      "ignore-gtfs-distances",
      opts::bool_switch(&(cfg->ignoreGtfsDistances))->default_value(false),
      "ignore the distance values in GTFS feed, useful for buggy feeds")(
      "station-aggregation-level",
      opts::value<size_t>(&(cfg->stationAggrLevel))->default_value(2),
      "2 = aggregate based on distance, 1 = aggregate based on feed, 0 = no "
      "aggr")(
      "max-station-aggr-distance",
      opts::value<double>(&(cfg->stationAggrDistance))->default_value(100),
      "maximum aggregation distance between stops (if aggr level > 1)")(
      "max-aggr-distance,d",
      opts::value<double>(&(cfg->maxAggrDistance))->default_value(30),
      "maximum aggregation distance between shared segments")(
      "use-mots,m", opts::value<string>(&motStr)->default_value("all"),
      "used mots");

  opts::options_description positional("Positional arguments");
  positional.add_options()("input-feed",
                           opts::value<std::string>(&(cfg->inputFeedPath)),
                           "path to an (unzipped) GTFS feed");

  opts::positional_options_description positionalOptions;
  positionalOptions.add("input-feed", 1);

  opts::options_description cmdlineOptions;
  cmdlineOptions.add(config).add(generic).add(positional);

  opts::options_description visibleDesc("Allowed options");
  visibleDesc.add(generic).add(config);
  opts::variables_map vm;

  try {
    opts::store(opts::command_line_parser(argc, argv)
                    .options(cmdlineOptions)
                    .positional(positionalOptions)
                    .run(),
                vm);
    opts::notify(vm);
  } catch (exception e) {
    LOG(ERROR) << e.what() << std::endl;
    std::cout << visibleDesc << "\n";
    exit(1);
  }

  for (auto sMotStr : util::split(motStr, ',')) {
    for (auto mot : ad::cppgtfs::gtfs::flat::Route::getTypesFromString(sMotStr)) {
      cfg->useMots.insert(mot);
    }
  }

  if (vm.count("help")) {
    std::cout << argv[0] << " [options] <input-feed>\n"
              << VERSION_STR << "\n\n"
              << visibleDesc << "\n";
    exit(0);
  }

  if (vm.count("version")) {
    std::cout << "\n" << VERSION_STR << "\n";
    exit(0);
  }
}
