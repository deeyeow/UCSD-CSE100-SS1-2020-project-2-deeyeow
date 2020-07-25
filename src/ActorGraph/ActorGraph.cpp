#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
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
        title += "#@" + to_string(year);

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
                     string& shortestPath) {
    // create queue for BFS
    queue<ActorNode*> queue;
    // get actor node
    ActorNode* from = nameToActorNode[fromActor];
    // mark first node visited
    from->markVisited();
    // push onto queue
    queue.push(nameToActorNode[fromActor]);

    // start BFS
    while (!queue.empty()) {
        // pop first element
        ActorNode* curr = queue.front();
        queue.pop();
        // check if found
        if (curr->getName() == toActor) {
            cout << "Target found, now getting path" << endl;
            // found, get path
            shortestPath = getPath(fromActor, curr, nameToActorNode);
            // reset visited and parents
            resetTree();
            // end BFS
            break;
        }
        //cout << "Target not found, going thru connections" << endl;
        // iterate over all movies actor has been in
        for (auto movie : *curr->getMovies()) {
            // iterate over all actors who have been in the same movie
            for (auto actor : *movieToActorSet[movie]) {
                // get connection node
                ActorNode* connection = nameToActorNode[actor];

                // IF FAILS, CHECK HERE
                // could be trying to reset parent pointers of already visited
                // nodes check again for visited to prevent cycles
                if (!connection->isVisited()) {
                    // mark connection as visited
                    connection->markVisited();
                    //cout << "Visiting connection: " << connection->getName()
                         //<< endl;
                    // set connection's parent to curr node
                    connection->setParent(curr->getName(), movie);
                    // push into queue for BFS
                    queue.push(nameToActorNode[actor]);
                }
                //cout << "Already visited: " << connection->getName()
                     //<< ", skipping" << endl;
            }
        }
    }
}

string ActorGraph::getPath(string rootName, ActorNode* curr,
                           unordered_map<string, ActorNode*> nameToActor) {
    stack<string> path;
    // root has no parent, backtrack until at root
    // can also check for root name
    while (curr->getName() != rootName) {
        // push curr actor
        path.push(curr->getName());
        // get parent info
        pair<string, string> parent = curr->getParent();
        // push movie
        path.push(parent.second);
        // process parent
        curr = nameToActor[parent.first];
    }
    // at root, push final actor
    path.push(curr->getName());

    // now print in correct order
    // (elements will be 2*N + 1)
    string ans = "";
    while (path.size() > 1) {
        ans += "(" + path.top() + ")";
        path.pop();
        ans += "--";
        ans += "[" + path.top() + "]";
        path.pop();
        ans += "-->";
    }
    ans += "(" + path.top() + ")";
    return ans;
}

/* TODO */
ActorGraph::~ActorGraph() {
    // delete actor set associated with every movie
    for (auto movie : movieToActorSet) {
        delete movie.second;
    }
    // delete actor node associated with every actor name
    for (auto actor : nameToActorNode) {
        delete actor.second;
    }
}
