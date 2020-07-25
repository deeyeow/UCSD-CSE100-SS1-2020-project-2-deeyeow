add_executable (test_ActorGraph.exe test_ActorGraph.cpp)
target_link_libraries(test_ActorGraph.exe PRIVATE gtest_main actorgraph_dep)
add_test(test_ActorGraph test_ActorGraph.exe)
