#include "seven_segment_display.h"
#include "error.h"
#include <iostream>
#include <unistd.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

SevenSegmentDisplay::SevenSegmentDisplay() : _x(5), _y() {}

void SevenSegmentDisplay::Show()
{
    char char_arr[15][100];
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 100; ++j)
            char_arr[i][j] = ' ';
    std::string str;
    try
    {
        for (auto &&pos : _position_map)
        {
            std::string str;
            int index = atoi(pos.first.substr(1, pos.first.size() - 1).c_str());
            if (pos.second->GetOutput())
            {
                int space = 0;
                if (index > 4)
                    space = 5 * ((index - 5) / 2 + 1);
                if (_input_map[index].find("_a") != _input_map[index].end() && _input_map[index]["_a"]->GetOutput())
                    for (int i = _x * index + 1 + space; i < _x * index + 4 + space; ++i)
                        char_arr[_y][i] = '_';
                if (_input_map[index].find("_f") != _input_map[index].end() && _input_map[index]["_f"]->GetOutput())
                    char_arr[_y + 1][_x * index + space] = '|';
                if (_input_map[index].find("_g") != _input_map[index].end() && _input_map[index]["_g"]->GetOutput())
                    for (int i = _x * index + 1 + space; i < _x * index + 4 + space; ++i)
                        char_arr[_y + 1][i] = '_';
                if (_input_map[index].find("_b") != _input_map[index].end() && _input_map[index]["_b"]->GetOutput())
                    char_arr[_y + 1][_x * index + 4 + space] = '|';
                if (_input_map[index].find("_e") != _input_map[index].end() && _input_map[index]["_e"]->GetOutput())
                    char_arr[_y + 2][_x * index + space] = '|';
                if (_input_map[index].find("_d") != _input_map[index].end() && _input_map[index]["_d"]->GetOutput())
                    for (int i = _x * index + 1 + space; i < _x * index + 4 + space; ++i)
                        char_arr[_y + 2][i] = '_';
                if (_input_map[index].find("_c") != _input_map[index].end() && _input_map[index]["_c"]->GetOutput())
                    char_arr[_y + 2][_x * index + 4 + space] = '|';
            }
            for (int i = 0; i < 15; ++i)
            {
                for (int j = 0; j < 100; ++j)
                    str += char_arr[i][j];
                str += '\n';
            }
            system(CLEAR);
            std::cout << str << std::endl;
        }
    }
    catch (const GateError &error)
    {
        std::cout << error.what() << std::endl;
    }
}

void SevenSegmentDisplay::SetInput(std::string c, const std::shared_ptr<Gate> &gate)
{
    std::string str = "_1_2_3_4_5_6_7_8_9_10_11_12_13_14";
    if (str.find(c) != std::string::npos)
        _position_map[c] = gate;
    else
    {
        if (c.size() == 3)
            _input_map[atoi(c.substr(2, c.size() - 2).c_str())][c.substr(0, 2)] = gate;
        else
            _input_map[atoi(c.substr(2, c.size() - 2).c_str())][c.substr(0, 2)] = gate;
    }
}
