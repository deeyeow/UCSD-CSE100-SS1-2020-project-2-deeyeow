#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

ActorGraph::ActorGraph() { totalNodes = visitedNodes = 0; }

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraph(istream& is) {
    bool readHeader = false;

    while (is) {
        string s;
        if (!getline(is, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actor(record[0]);
        string title(record[1]);
        int year = stoi(record[2]);

        // TODO: we have an actor/movie relationship to build the graph
        title += " " + to_string(year);

        // associate actor with movie, put into map for direct access (vertex)
        ActorNode* tempNode;
        if (nameToActorNode.count(actor) == 0)
            tempNode = new ActorNode(actor);
        else
            tempNode = nameToActorNode[actor];
        tempNode->addMovie(title);
        nameToActorNode[actor] = tempNode;

        // associate movie with actor, put into map for direct access (edge)
        unordered_set<string>* tempSet;
        if (movieToActorSet.count(title) == 0)
            tempSet = new unordered_set<string>();
        else
            tempSet = movieToActorSet[title];
        tempSet->emplace(actor);
        movieToActorSet[title] = tempSet;

        /*
        if (movieToActorSet[title]->count(actor) != 0) {
            cout << actor << " played in " << title << endl;
        }
        */
    }

    // if failed to read the file, clear the graph and return
    if (!is.eof()) {
        // TODO: delete the graph
        return false;
    }

    return true;
}

/* TODO */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {}

/* TODO */
ActorGraph::~ActorGraph() {
    for (auto movie : movieToActorSet) {
        delete movie.second;
    }
    for (auto actor : nameToActorNode) {
        delete actor.second;
    }
}
