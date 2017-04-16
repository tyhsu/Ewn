#ifndef Ewnfile_H
#define Ewnfile_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

class Ewnfile {
public:
    Ewnfile ();
    ~Ewnfile ();
    ofstream myfile;
    string file_name;
    void write_to_file(string text, bool newline = true);
    void set_file_name();
};

#endif
