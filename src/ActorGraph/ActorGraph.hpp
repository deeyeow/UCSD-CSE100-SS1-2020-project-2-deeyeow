#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

#include "ActorNode.hpp"

using namespace std;

class ActorGraph {
  protected:
    // TODO: add data structures used in actor graph
    unordered_map<string, unordered_set<string>*> movieToActorSet;
    unordered_map<string, ActorNode*> nameToActorNode;

  public:
    /* TODO */
    ActorGraph();

    /* TODO */
    bool buildGraph(istream& is);

    /* TODO */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    // double BFS
    string getPath(string fromName, string toName, string movie,
                   ActorNode* connection, ActorNode* curr,
                   unordered_map<string, ActorNode*> nameToActor, bool isFrom);

    /*
    // single BFS
    string getPath(string root, ActorNode* curr,
                   unordered_map<string, ActorNode*> nameToActor);
    */

    void resetTree() {
        for (auto node : nameToActorNode) {
            node.second->markUnvisited();
            node.second->resetParent();
        }
    }

    /* TODO */
    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
