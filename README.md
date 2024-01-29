# CDataStructure++

Various data structures implementations in C++. This is just an exercise for me to understand how raw pointers work in C++ and get acquainted with C++ bloated ecosystem :) 

Implemented Features:
- AVLTree
- BinarySearchTree
- HashMap
- LinkedList

Principles followed:
- RAII for the encapsulation of memory management
- Templating for generic types

Majority of the code is implemented in header files in order to compile templated classes (I am aware that this can also be done using cpp files, for now header files are chosen for simplicity).

# *WARNING*  
The data structures implemented should by no means be used in practice. This repo is created with the sole purpose of experimentating with C++ by implementing data structures. Other libraries offer better and more efficient implementations of what this repo contains. Use at your own risk!

## OPTIMIZATIONS AND TODOS

- [ ] the code is now unnecessarily complex for the `removeNode` function. This is because changing the `currNode` pointer does not change the actual pointer but only the `currNode` pointer in the scope of the function (since `currNode` value is passed by value and not by reference). To fix this, the pointers need to be passed by reference. (AVL TREE AND BST)
- [ ] Instead of pointers, use references to objects for better readability since the code becomes less complex and reduce risk for mem. leak.
- [ ] Use safe pointer from <memory> as much as possible. 
- [ ] Code is a bit complex now with many checks (that might be unnecessary if done differently). Refactor code and make it simpler (KISS concept)
- [ ] Make CMakeLists.txt less cluttered with operations (is there a way to do the same, but in a shorter and cleaner way? Porbably).
- [ ] In AVL tree and BST: pointers are passed by value to functions. Any change to the passed pointer in the function itself won't be reflected outside of it since it is not passed by reference. See if passing the nodes to function by reference is better and whether that has implications on the performance.
