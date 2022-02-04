#include <utility>
#include <optional>

#ifndef MUSTARD_TAG_H
#define MUSTARD_TAG_H

using namespace std;

struct Tag {
    Tag ( string name, optional<string> value ) :
        name ( std::move ( name ) ),
        value ( std::move ( value ) ) {}
    const string name;
    const optional<string> value;
};
#endif //MUSTARD_TAG_H
