#include "circuit.h"
#include "menu.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void HelpMenu(std::string choice);

int main()
{
    std::string choice;
    system(CLEAR);
    std::cout << "welcome\n\n";
    MainMenu();
    while (std::getline(std::cin, choice))
    {
        if (choice[0] == '1' || choice[0] == '2')
        {
            system(CLEAR);
            MainLoop(bool(choice[0] == '2'));
            break;
        }
        std::cout << "No command\n";
    }

    return 0;
}
