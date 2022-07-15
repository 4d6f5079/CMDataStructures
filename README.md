# CDataStructure++

Various data structures implementations in C++. This is just an exercise for me to understand how raw pointers work in C++ and get acquainted with C++ bloated ecosystem ( don't ask me why :) ).

## POSSIBLE OPTIMIZATIONS  
- [ ] the code is now unnecessarily complex for the `removeNode` function. This is because changing the `currNode` pointer does not change the actual pointer but only the `currNode` pointer in the scope of the function (since `currNode` value is passed by value and not by reference). To fix this, the pointers need to be passed by reference.  
- [ ] Instead of pointers, use references to objects for better readability since the code becomes less complex. NOTE: check first whether this will cause a hit in performance.
