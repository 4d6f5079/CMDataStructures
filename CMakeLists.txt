# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("CDataStructure++")

# Include sub-projects.
add_subdirectory ("CDataStructure++")

add_executable (
	CDataStructure++
	"main.cpp" 
	"CDataStructure++/HashTable.cpp" 
	"CDataStructure++/HashTable.h" 
	"CDataStructure++/LinkedList.cpp" 
	"CDataStructure++/LinkedList.h" 
	"CDataStructure++/Node.cpp" 
	"CDataStructure++/Node.h"
  "CDataStructure++/Timer/Timer.h" "CDataStructure++/Timer/Timer.cpp")
