#ifndef MUSTARD_TODO_H
#define MUSTARD_TODO_H

#include <string>

using namespace std;

struct Todo {
    enum TodoStatus {NEW, TODO, RESOLVE, DONE};

    string text;
    TodoStatus status;
};

#endif //MUSTARD_TODO_H
