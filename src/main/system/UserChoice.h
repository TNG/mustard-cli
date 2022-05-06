#ifndef MUSTARD_USERCHOICE_H
#define MUSTARD_USERCHOICE_H

#include <map>
#include <iostream>

using namespace std;

template <typename T>
class UserChoice
{
public:
    UserChoice ( const string &message, map<string, T> choices ) :
        choices ( choices ), message ( message ) {};
    T askUser();
private:
    map<string , T> choices;
    string message;
};

template<typename T>

T UserChoice<T>::askUser()
{
    cout << message << " [";
    bool first = true;
    for ( auto choice : choices ) {
        if ( !first ) {
            cout << "/";
        } else {
            first = false;
        }
        cout << choice.first;
    }
    cout << "]";
    string userChoice;
    cin >> userChoice;
    auto findChoice = choices.find ( userChoice );
    if ( findChoice != choices.end() ) {
        return findChoice->second;
    }
    cout << "Unknown choice. please choose:" << endl;
    return askUser();
}


#endif //MUSTARD_USERCHOICE_H
