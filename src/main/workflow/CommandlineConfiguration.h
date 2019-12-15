#ifndef MUSTARD_COMMANDLINECONFIGURATION_H
#define MUSTARD_COMMANDLINECONFIGURATION_H


class CommandlineConfiguration
{
public:
    void parse ( const int argc, const char **argv );
    bool isDebug();
private:
    bool debug = false;

};


#endif //MUSTARD_COMMANDLINECONFIGURATION_H
