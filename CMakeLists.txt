# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Here FLAGS can be given to compiler when building in Debug/Release mode
#set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
# set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

project (CDataStructure++ VERSION 1.0)

# Include sub-projects.
#add_subdirectory (${PROJECT_NAME})

# Add libraries of different implemented data structure implementation cpp and h/hpp files
file(GLOB LIB_BST_CPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/BinarySearchTree/*.cpp)
file(GLOB LIB_BST_HS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/BinarySearchTree/*.h)
file(GLOB LIB_BST_HPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/BinarySearchTree/*.hpp)
add_library (
	libbst 
	STATIC 
	${LIB_BST_CPPS}
	${LIB_BST_HS}
	${LIB_BST_HPPS}
)


file(GLOB LIB_HT_CPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/HashTable/*.cpp)
file(GLOB LIB_HT_HS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/HashTable/*.h)
file(GLOB LIB_HT_HPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/HashTable/*.hpp)
add_library (
	libht 
	STATIC 
	${LIB_HT_CPPS}
	${LIB_HT_HS}
	${LIB_HT_HPPS}
)

file(GLOB LIB_LL_CPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/LinkedList/*.cpp)
file(GLOB LIB_LL_HS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/LinkedList/*.h)
file(GLOB LIB_LL_HPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/LinkedList/*.hpp)
add_library (
	libll 
	STATIC 
	${LIB_LL_CPPS}
	${LIB_LL_HS}
	${LIB_LL_HPPS}
)

file(GLOB LIB_TIMER_CPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/Timer/*.cpp)
file(GLOB LIB_TIMER_HS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/Timer/*.h)
file(GLOB LIB_TIMER_HPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/Timer/*.hpp)
add_library (
	libtimer 
	STATIC 
	${LIB_TIMER_CPPS}
	${LIB_TIMER_HS}
	${LIB_TIMER_HPPS}
)

file(GLOB LIB_AVL_TREE_CPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/AVLTree/*.cpp)
file(GLOB LIB_AVL_TREE_HS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/AVLTree/*.h)
file(GLOB LIB_AVL_TREE_HPPS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/AVLTree/*.hpp)
add_library (
	libavl 
	STATIC 
	${LIB_AVL_TREE_CPPS}
	${LIB_AVL_TREE_HS}
	${LIB_AVL_TREE_HPPS}
)

# Including the folder where the header files are located of each added library to let cmake know where to find .h files
# This makes it possible to include the header files / libraries without giving the full relative path
target_include_directories (libbst PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/BinarySearchTree)
target_include_directories (libht PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/HashTable)
target_include_directories (libll PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/LinkedList)
target_include_directories (libtimer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/Timer)
target_include_directories (libavl PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/AVLTree)

# Add source to this project's executable.
add_executable (app main.cpp)

# Link the libraries with the executable
target_link_libraries(app PUBLIC libbst)
target_link_libraries(app PUBLIC libht)
target_link_libraries(app PUBLIC libll)
target_link_libraries(app PUBLIC libtimer)
target_link_libraries(app PUBLIC libavl)
target_link_libraries(libavl PUBLIC libbst)