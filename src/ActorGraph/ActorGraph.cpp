#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>

using namespace std;

ActorGraph::ActorGraph() {}

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

        // optimized build using find
        // associate actor with movie, put into map for direct access (vertex)
        ActorNode* tempNode;
        if (nameToActorNode.find(actor) == nameToActorNode.end())
            tempNode = new ActorNode(actor);
        else
            tempNode = nameToActorNode[actor];
        tempNode->addMovie(title);
        nameToActorNode[actor] = tempNode;

        // associate movie with actor, put into map for direct access (edge)
        unordered_set<string>* tempSet;
        if (movieToActorSet.find(title) == movieToActorSet.end())
            tempSet = new unordered_set<string>();
        else
            tempSet = movieToActorSet[title];
        tempSet->emplace(actor);
        movieToActorSet[title] = tempSet;

        /*
        // unoptimized build using count
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
        */
    }

    // if failed to read the file, clear the graph and return
    if (!is.eof()) {
        // TODO: delete the graph
        return false;
    }

    return true;
}

// double BFS
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    // create queue for BFS
    queue<ActorNode*> queue;

    // get first actor node
    ActorNode* from = nameToActorNode[fromActor];
    // mark first node visited
    from->markVisitedFrom();
    // push onto queue
    queue.push(nameToActorNode[fromActor]);

    // get last actor node
    ActorNode* to = nameToActorNode[toActor];
    // mark last node visited
    to->markVisitedTo();
    // push onto queue
    queue.push(nameToActorNode[toActor]);

    // start BFS
    while (!queue.empty()) {
        // pop first element
        ActorNode* curr = queue.front();
        queue.pop();
        // get which end of BFS
        bool isFrom = curr->isVisitedFrom();

        // iterate over all movies actor has been in
        for (auto movie : *curr->getMovies()) {
            // iterate over all actors who have been in the same movie
            for (auto actor : *movieToActorSet[movie]) {
                // get connection node
                ActorNode* connection = nameToActorNode[actor];

                // if From end of BFS, check if visited by End
                if (isFrom) {
                    // found node visited from End, return
                    if (connection->isVisitedTo()) {
                        cout << "Target found from From end, now getting path"
                             << endl;
                        // found from From end of BFS, get path
                        shortestPath =
                            getPath(fromActor, toActor, movie, connection, curr,
                                    nameToActorNode, isFrom);
                        // reset visited and parents
                        resetTree();
                        // end BFS
                        goto outOfLoops;
                    }
                    // not found node, queue unvisited nodes
                    if (!connection->isVisitedFrom()) {
                        // mark connection as visited
                        connection->markVisitedFrom();
                        // cout << "Visiting connection: " <<
                        // connection->getName()
                        //<< endl;
                        // set connection's parent to curr node
                        connection->setParent(curr->getName(), movie);
                        // push into queue for BFS
                        queue.push(nameToActorNode[actor]);
                    }
                }
                // if End end of BFS, check if visited by From
                else {
                    // found node visited from From, return
                    if (connection->isVisitedFrom()) {
                        cout << "Target found from End end, now getting path"
                             << endl;
                        // found from From end of BFS, get path
                        shortestPath =
                            getPath(fromActor, toActor, movie, connection, curr,
                                    nameToActorNode, isFrom);
                        // reset visited and parents
                        resetTree();
                        // end BFS
                        goto outOfLoops;
                    }
                    // not found node, queue unvisited nodes
                    if (!connection->isVisitedTo()) {
                        // mark connection as visited
                        connection->markVisitedTo();
                        // cout << "Visiting connection: " <<
                        // connection->getName()
                        //<< endl;
                        // set connection's parent to curr node
                        connection->setParent(curr->getName(), movie);
                        // push into queue for BFS
                        queue.push(nameToActorNode[actor]);
                    }
                }
            }
        }
    }
outOfLoops:;
}

string ActorGraph::getPath(string fromName, string toName, string movie,
                           ActorNode* connection, ActorNode* curr,
                           unordered_map<string, ActorNode*> nameToActor,
                           bool isFrom) {
    stack<string> pathFrom;
    queue<string> pathTo;
    string ans = "";

    ActorNode* temp;

    if (isFrom)
        temp = curr;
    else
        temp = connection;

    // root has no parent, backtrack until at root
    // can also check for root name
    while (temp->getName() != fromName) {
        // push curr actor
        pathFrom.push(temp->getName());
        // get parent info
        pair<string, string> parent = temp->getParent();
        // push movie
        pathFrom.push(parent.second);
        // process parent
        temp = nameToActor[parent.first];
    }
    // at root, push final actor
    pathFrom.push(temp->getName());

    // now print in correct order
    // (elements will be 2*N + 1)
    while (pathFrom.size() > 1) {
        ans += "(" + pathFrom.top() + ")";
        pathFrom.pop();
        ans += "--";
        ans += "[" + pathFrom.top() + "]";
        pathFrom.pop();
        ans += "-->";
    }
    ans += "(" + pathFrom.top() + ")";

    // process other end of BFS
    if (isFrom)
        temp = connection;
    else
        temp = curr;

    ans += "--";
    ans += "[" + movie + "]";
    ans += "-->";

    while (temp->getName() != toName) {
        // get parent info
        pair<string, string> parent = temp->getParent();
        ans += "(" + temp->getName() + ")";
        ans += "--";
        ans += "[" + parent.second + "]";
        ans += "-->";
        temp = nameToActor[parent.first];
    }
    ans += "(" + temp->getName() + ")";
    return ans;
}

/*
// single BFS
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
        // cout << "Target not found, going thru connections" << endl;
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
                    // cout << "Visiting connection: " << connection->getName()
                    //<< endl;
                    // set connection's parent to curr node
                    connection->setParent(curr->getName(), movie);
                    // push into queue for BFS
                    queue.push(nameToActorNode[actor]);
                }
                // cout << "Already visited: " << connection->getName()
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
*/

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
