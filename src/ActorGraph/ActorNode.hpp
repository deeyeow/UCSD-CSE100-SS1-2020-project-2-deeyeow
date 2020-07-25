#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <unordered_set>

using namespace std;

class ActorNode {
  private:
    // TODO: add data structures used in actor graph
    unordered_set<string> movies;
    string name;
    bool visited;

    pair<string,string> parent;

  public:
    ActorNode(string _name) {
        name = _name;
        visited = false;
    }

    void addMovie(string movieName) { movies.emplace(movieName); }

    // out of scope, no need to delete unordered_set
    ~ActorNode() {}
};

#endif  // ACTORNODE_HPP