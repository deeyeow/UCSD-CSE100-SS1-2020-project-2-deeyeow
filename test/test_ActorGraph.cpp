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
