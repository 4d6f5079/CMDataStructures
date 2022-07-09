# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("HashTableTesting")

# Include sub-projects.
add_subdirectory ("HashTableTesting")

add_executable (
	HashTable 
	"main.cpp" 
	"HashTableTesting/HashTable.cpp" 
	"HashTableTesting/HashTable.h" 
	"HashTableTesting/LinkedList.cpp" 
	"HashTableTesting/LinkedList.h" 
	"HashTableTesting/Node.cpp" 
	"HashTableTesting/Node.h"
)
