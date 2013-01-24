#include <iostream>
#include <cstdio>
#include <list>
#include <string.h>
#include "dirent.h"
 
using namespace std;

void printMessage();
void matchNameLoop(list<string>* movieList, list<string>* smiList);
void matchName(string* movieName, string* smiName);
string getExtension(string* name);

const char* directory = ".\\";

int main()
{
  printMessage();
  return 0;
}

void printMessage() 
{
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir (directory)) != NULL) {

    list<string>* movieList = new list<string>;
    list<string>* smiList = new list<string>;

    while ((ent = readdir (dir)) != NULL) {
      string d_name = ent->d_name;
      size_t size = d_name.size();

      string extension = getExtension(&d_name);
      if (extension.compare("smi") == 0
          || extension.compare("srt") == 0) {
        smiList->push_back(d_name);
      }
      if (extension.compare("mkv") == 0
          || extension.compare("avi") == 0
          || extension.compare("mp4") == 0
          || extension.compare("flv") == 0) {
        movieList->push_back(d_name);
      }
    }
    closedir (dir);

    matchNameLoop(movieList, smiList);
  } else {
    /* could not open directory */
    perror ("could not open directory");
  }
}

void matchNameLoop(list<string>* movieList, list<string>* smiList) 
{
  list<string>::iterator iterMovie = movieList->begin();
  list<string>::iterator iterMovieEnd = movieList->end();
  list<string>::iterator iterSmi = smiList->begin();
  list<string>::iterator iterSmiEnd = smiList->end();

  while (iterMovie != iterMovieEnd && iterSmi != iterSmiEnd) {
    matchName(&(*iterMovie), &(*iterSmi));
    ++iterMovie;
    ++iterSmi;
  }
}

void matchName(string* movieName, string* smiName)
{
  int movieRightPos = movieName->rfind(".");

  string name = movieName->substr(0, movieRightPos);
  name = name.append(".").append(getExtension(smiName));

  string dir = directory;
  dir = dir.append("\\");
  size_t size = dir.size();

  char* oldName = new char[size + smiName->size() + 10];
  char* newName = new char[size + name.size() + 10];

  dir.copy(oldName, size, 0);
  dir.copy(newName, size, 0);

  strcpy(oldName + size, (char*)smiName->c_str());
  strcpy(newName + size, (char*)name.c_str());

  int result = rename(oldName, newName);

  /*
  cout << "result:" << result << endl;
  cout << *movieName << endl;
  cout << *smiName << endl;
  cout << oldName << endl;
  cout << newName << endl;
  cout << "--------------" << endl;
  */
}

string getExtension(string* name)
{
  int rightPos = name->rfind(".");
  return name->substr(rightPos + 1, 3);
}