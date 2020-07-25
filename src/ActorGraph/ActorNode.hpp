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
    pair<string, string> parent;

  public:
    ActorNode(string _name) {
        name = _name;
        visited = false;
    }

    void addMovie(string movieName) { movies.emplace(movieName); }
    unordered_set<string>* getMovies() { return &movies; }

    string getName() { return name; }

    void markVisited() { visited = true; }
    void markUnvisited() { visited = false; }
    bool isVisited() { return visited; }

    void setParent(string parentName, string movieName) {
        parent.first = parentName;
        parent.second = movieName;
    }
    void resetParent() {
        parent.first = "";
        parent.second = "";
    }
    pair<string, string> getParent() { return parent; }

    // out of scope, no need to delete anything
    ~ActorNode() {}
};

#endif  // ACTORNODE_HPP