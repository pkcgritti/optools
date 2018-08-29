set(MONO_FILES "Problem.cpp" "Random.cpp" "Result.cpp" "algorithms/PSO.cpp")
add_library(mono ${MONO_FILES})
target_link_libraries(mono pthread)