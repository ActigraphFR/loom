// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef TRANSITMAP_OPTIM_ILPOPTIMIZER_H_
#define TRANSITMAP_OPTIM_ILPOPTIMIZER_H_

#include <glpk.h>
#include "transitmap/config/TransitMapConfig.h"
#include "transitmap/graph/OrderingConfiguration.h"
#include "transitmap/graph/Route.h"
#include "transitmap/graph/TransitGraph.h"
#include "transitmap/optim/OptGraph.h"
#include "transitmap/optim/Optimizer.h"

using std::exception;
using std::string;

namespace transitmapper {
namespace optim {

using namespace graph;

typedef std::pair<const Route*, const Route*> LinePair;
typedef std::pair<size_t, size_t> PosCom;
typedef std::pair<PosCom, PosCom> PosComPair;
typedef std::pair<OptEdge*, OptEdge*> EdgePair;

struct VariableMatrix {
  std::vector<int> rowNum;
  std::vector<int> colNum;
  std::vector<double> vals;

  void addVar(int row, int col, double val);
  void getGLPKArrs(int** ia, int** ja, double** r) const;
  size_t getNumVars() const { return vals.size(); }
};

class ILPOptimizer : public Optimizer {
 public:
  ILPOptimizer(TransitGraph* g, const config::Config* cfg) : _g(g), _cfg(cfg){};

  int optimize(const Penalties& pens) const;

 protected:
  TransitGraph* _g;
  const config::Config* _cfg;

  virtual glp_prob* createProblem(const OptGraph& g, const Penalties& pens) const;

  void solveProblem(glp_prob* lp) const;
  void preSolveCoinCbc(glp_prob* lp) const;

  virtual void getConfigurationFromSolution(glp_prob* lp, Configuration* c,
                                            const OptGraph& g) const;

  void expandRelatives(Configuration* c, TransitGraph* g) const;

  void expandRelativesFor(Configuration* c, const Route* ref,
                          graph::Edge* start,
                          const std::set<const Route*>& r) const;

  std::string getILPVarName(OptEdge* e, const Route* r, size_t p) const;

  void writeSameSegConstraints(const OptGraph& g, const Penalties& pens, VariableMatrix* vm,
                               glp_prob* lp) const;

  void writeDiffSegConstraints(const OptGraph& g, const Penalties& pens, VariableMatrix* vm,
                               glp_prob* lp) const;

  bool printHumanReadable(glp_prob* lp, const std::string& path) const;
  double getConstraintCoeff(glp_prob* lp, int constraint, int col) const;

  std::vector<LinePair> getLinePairs(OptEdge* segment) const;
  std::vector<LinePair> getLinePairs(OptEdge* segment, bool unique) const;

  /*
   * For a node A, and edge e and a pair or routes return all the
   * edges != e where the route-pair continues in
   */
  std::vector<OptEdge*> getEdgePartners(OptNode* node, OptEdge* segmentA,
                                        const LinePair& linepair) const;
  std::vector<EdgePair> getEdgePartnerPairs(OptNode* node, OptEdge* segmentA,
                                            const LinePair& linepair) const;
  std::vector<PosComPair> getPositionCombinations(OptEdge* a, OptEdge* b) const;
  std::vector<PosCom> getPositionCombinations(OptEdge* a) const;

  bool crosses(OptNode* node, OptEdge* segmentA, OptEdge* segmentB,
               PosComPair postcomb) const;

  bool crosses(OptNode* node, OptEdge* segmentA, EdgePair segments,
               PosCom postcomb) const;

  int getCrossingPenaltySameSeg(const OptNode* n, const Penalties& pens) const;
  int getCrossingPenaltyDiffSeg(const OptNode* n, const Penalties& pens) const;
  int getSplittingPenalty(const OptNode* n, const Penalties& pens) const;

  Point getPos(OptNode* n, OptEdge* segment, size_t p) const;
};
}  // namespace optim
}  // namespace transitmapper

#endif  // TRANSITMAP_OPTIM_ILPOPTIMIZER_H_
