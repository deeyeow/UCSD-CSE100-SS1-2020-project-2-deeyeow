#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include "ActorNode.hpp"

using namespace std;

class ActorGraph {
  protected:
    // TODO: add data structures used in actor graph
    unordered_map<string, unordered_set<string>*> movieToActorSet;
    unordered_map<string, ActorNode*> nameToActorNode;
    int totalNodes;
    int visitedNodes;


  public:
    /* TODO */
    ActorGraph();

    /* TODO */
    bool buildGraph(istream& is);

    /* TODO */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* TODO */
    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
