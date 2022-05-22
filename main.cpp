#include "domino.h"
#include <signal.h>
#include <iostream>
#include <fstream>

Game *party;
void my_terminate (int param)
{
  delete party;
  std::cout << "Ending game\n";
  exit(1);
}

void ask();

int main()
{
   signal(SIGINT, my_terminate);
   party = new Game();
   ask();
   my_terminate(0);
}

void ask()
{
  clearing(false);
  std::cout << "Do you wish the history to be saved?\n";
  char yn;
  std::cout << "Please give name of save file (no name - no save): ";
  std::cin.get(yn);
  if (yn!='\n')
  {
     std::cin.putback(yn);
     std::string filename;
     std::cin >> filename;
     std::ofstream file(filename.c_str());
     party->print(file);
     file.close();
  }
}
