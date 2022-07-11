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
	"CDataStructure++/HashTable/HashTable.cpp" 
	"CDataStructure++/HashTable/HashTable.h" 
	"CDataStructure++/LinkedList/LinkedList.cpp" 
	"CDataStructure++/LinkedList/LinkedList.h" 
	"CDataStructure++/LinkedList/Node.cpp" 
  "CDataStructure++/Timer/Timer.h" "CDataStructure++/Timer/Timer.cpp" "CDataStructure++/BinarySearchTree/BinarySearchTreeNode.h" "CDataStructure++/BinarySearchTree/BinarySearchTree.h")
