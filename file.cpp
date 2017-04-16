#include "file.h"
using namespace std;

Ewnfile::Ewnfile() {
    cout << "file class" << endl;
}
Ewnfile::~Ewnfile() {
    cout << "delete file class" << endl;
}

void Ewnfile::write_to_file(string text, bool newline) {
 	this->myfile.open("record/" + this->file_name, ios::app);
	this->myfile << text;
	if (newline)
		this->myfile << endl;
	this->myfile.close();
}

void Ewnfile::set_file_name() {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y_%I:%M:%S.txt",timeinfo);
  string time_str(buffer);
  cout << time_str << endl;
  this->file_name = time_str;
}