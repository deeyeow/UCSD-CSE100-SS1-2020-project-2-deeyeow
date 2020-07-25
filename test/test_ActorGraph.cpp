#include <gtest/gtest.h>

#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

TEST(ActorGraphTests, TwoActorsOneMovie) {
    string from = "Actor 1";
    string to = "Actor 2";
    string through = "Awesome Movie\t2020";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);
}

TEST(ActorGraphTests, OneActorTwoMovies) {
    string from = "Actor 1";
    string to = "Actor 1";
    string through = "Awesome Movie\t2020";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);
}

TEST(ActorGraphTests, Empty) {
    string from = "";
    string to = "";
    string through = "";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);
}

TEST(ActorGraphTests, GetPath) {
    ActorNode* actor1 = new ActorNode("actor1");
    ActorNode* actor2 = new ActorNode("actor2");
    ActorNode* actor3 = new ActorNode("actor3");
    ActorNode* actor4 = new ActorNode("actor4");
    actor4->setParent("actor3", "movie4");
    actor3->setParent("actor2", "movie3");
    actor2->setParent("actor1", "movie2");

    ActorGraph graph;
    unordered_map<string, ActorNode*> nameToActor;
    nameToActor.emplace("actor1", actor1);
    nameToActor.emplace("actor2", actor2);
    nameToActor.emplace("actor3", actor3);
    nameToActor.emplace("actor4", actor4);

    ASSERT_EQ(graph.getPath("actor1", actor4, nameToActor), "actor1--movie2-->actor2--movie3-->actor3--movie4-->actor4");
}
