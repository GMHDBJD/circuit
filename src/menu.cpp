#include "menu.h"
#include "circuit.h"
#include <iostream>
#include <sstream>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void MainMenu()
{
    std::cout << "input 1 : combinatorial logic circuits\n";
    std::cout << "input 2 : sequential logic circuits\n";
    std::cout << "input quit : quit\n\n";
}

void MainLoop(bool sequential)
{
    Circuit circuit(sequential);
    std::cout << "welcome to the " << (sequential ? "sequential logic circuits" : "logic circuits") << "\ninput help to view command\n\n";
    std::string str;
    std::set<std::string> gate_set = {"ANDGate", "ORGate", "NOTGate", "XORGate", "XNORGate", "NANDGate", "NORGate", "input", "output"};
    while (std::getline(std::cin, str) && str != "quit")
    {
        circuit.Command(str, gate_set, sequential, "");
    }
}
