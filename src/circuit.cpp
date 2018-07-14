#include "circuit.h"
#include "error.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unistd.h>
#include <vector>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

Circuit::Node::Node(std::shared_ptr<Gate> gate) : _gate(gate), _begin(-1), _end(-1), _color(white) {}

Circuit::Circuit(bool sequential) : _sequential(sequential), _frequency(1000000000000) {}

Circuit::Node::~Node() = default;

void Circuit::AddGate(std::string str, const Gate &gate)
{
    if (_node_map.find(str) != _node_map.end())
        std::cerr << "duplicate name " << str << "\n";
    else
    {
        std::shared_ptr<Node> new_node(new Node(std::shared_ptr<Gate>(gate.Clone())));
        _node_map[str] = new_node;
    }
}

void Circuit::Remove(std::string str)
{
    if (_node_map.find(str) != _node_map.end())
    {
        for (auto &&i : _circuit_multiset_map[str])
            RemoveWire(str, i);
        if (_input_gate_set.find(str) != _input_gate_set.end())
            _input_gate_set.erase(str);
        else
        {
            for (auto &&i : _node_map[str]->_gate->GetInput())
                RemoveWire(i.first, str);
            if (_output_gate_set.find(str) != _output_gate_set.end())
                _output_gate_set.erase(str);
        }
        _node_map.erase(str);
    }
    else
        std::cerr << "no " << str << std::endl;
}

void Circuit::AddInput(std::string str, const Gate &gate)
{

    try
    {
        AddGate(str, gate);
        _input_gate_set.insert(str);
    }
    catch (const CircuitError &error)
    {
        std::cerr << error.what() << '\n';
    }
}

void Circuit::AddOutput(std::string str, const Gate &gate)
{
    try
    {
        AddGate(str, gate);
        _output_gate_set.insert(str);
    }
    catch (const CircuitError &error)
    {
        std::cerr << error.what() << '\n';
    }
}

void Circuit::SetFrequency(double frequency)
{
    _frequency = frequency;
}

void Circuit::Help()
{
    system(CLEAR);
    std::cout << "create [gate] gate_name                   : create a gate   [gate]: ANDGate ORGate NOTGate XORGate XNORGate NANDGate NORGate\n";
    std::cout << "create input input_name                   : create a input\n";
    std::cout << "create output output_name                 : create a output\n";
    std::cout << "wire [gate_name1] [gate_name2]            : wire two gate1's output to gate2's input\n";
    std::cout << "remove_wire [gate_name1] [gate_name2]     : remove a wire\n";
    std::cout << "list                                      : view all gates ,inputs and outputs\n";
    std::cout << "setinput input_name value                 : set your input\n";
    if (_sequential)
    {
        std::cout << "connect output_name [char]                : connect a output to the seven_segment_display   [char]: _a1 _b2 _c3 _d4 _e5 _f6 _g7 ...\n";
    }
    std::cout << "remove [name]                             : remove a gate, input or output\n";
    std::cout << "exec                                      : execute the circuit\n";
    std::cout << "clear                                     : clear the circuit\n";
    std::cout << "import [file_name] as [name]              : import [file_name] named [name]\n";
    std::cout << "draw filename                             : draw the circuit to draw\n";
    std::cout << "quit\n\n";
}

void Circuit::AddWire(std::string lhs, std::string rhs)
{
    if (_node_map.find(lhs) != _node_map.end() && _node_map.find(rhs) != _node_map.end())
    {
        try
        {
            _node_map[rhs]->_gate->InsertInput(lhs, Gate::_default_input);
            _circuit_multiset_map[lhs].insert(rhs);
        }
        catch (const GateError &error)
        {
            std::cerr << rhs << error.what() << '\n';
        }
    }
    else if (_node_map.find(lhs) != _node_map.end())
        std::cerr << "no " << rhs << std::endl;
    else
        std::cerr << "no " << lhs << std::endl;
}

void Circuit::Connect(std::string output, std::string s)
{
    std::set<std::string> string_set = {"_a", "_b", "_c", "_d", "_e", "_f", "_g", "_1", "_2", "_3", "_4", "_5", "_6", "_7", "_8", "_9", "_10", "_11", "_12", "_13", "_14"};
    if (s.size() >= 2 && string_set.find(s.substr(0, 2)) != string_set.end())
    {
        if (_node_map.find(output) != _node_map.end())
            seven_segment_display.SetInput(s, _node_map[output]->_gate);
        else
            std::cerr << "no " << output << std::endl;
    }
    else
        std::cerr << "seven segment display has no " << s << std::endl;
}

void Circuit::RemoveWire(std::string lhs, std::string rhs)
{
    if (_node_map.find(lhs) != _node_map.end() && _node_map.find(rhs) != _node_map.end())
    {
        auto pos = _circuit_multiset_map[lhs].find(rhs);
        if (pos == _circuit_multiset_map[lhs].end())
            std::cerr << "there is no wire between " << lhs << " and "
                      << rhs << std::endl;
        else
        {
            _node_map[rhs]->_gate->RemoveInput(lhs);
            _circuit_multiset_map[lhs].erase(pos);
        }
    }
    else if (_node_map.find(lhs) != _node_map.end())
        std::cerr << "no " << rhs << std::endl;
    else
        std::cerr << "no " << lhs << std::endl;
}

void Circuit::SetInput(std::string str, bool input, bool user)
{
    if (_input_gate_set.find(str) == _input_gate_set.end())
        std::cerr << "no input named " << str << std::endl;
    else
    {
        auto map = _node_map[str]->_gate->GetInput();
        if (!user)
        {
            if (map.empty())
                _node_map[str]->_gate->InsertInput("input", input);
            else if (map.find("input") != map.end())
                _node_map[str]->_gate->SetInput("input", input);
        }
        else
        {
            if (map.empty())
                _node_map[str]->_gate->InsertInput("user_input", input);
            else
                _node_map[str]->_gate->SetInput("user_input", input);
        }
    }
}

void Circuit::DFS()
{
    size_t t = -1;
    for (auto &&i : _node_map)
        i.second->_color = white;
    for (auto &&i : _node_map)
    {
        if (i.second->_color == white)
            DFSAux(i.first, &t);
    }
}

void Circuit::DFSAux(std::string current, size_t *t)
{
    ++*t;
    _node_map[current]->_color = gray;
    _node_map[current]->_begin = *t;
    for (auto &&i : _circuit_multiset_map[current])
        if (_node_map[i]->_color == white)
            DFSAux(i, t);
        else if (!_sequential && _node_map[i]->_color == gray)
            throw std::logic_error("your circuit exist circle");
    ++*t;
    _node_map[current]->_color = black;
    _node_map[current]->_end = *t;
}

void Circuit::TopologicalSorting(std::string *sort_arr)
{
    DFS();
    for (size_t i = 0; i < _node_map.size() * 2; ++i)
        sort_arr[i] = "";
    for (auto &&i : _node_map)
        sort_arr[i.second->_end] = i.first;
}

void Circuit::Exec()
{
    std::string *sort_arr = new std::string[_node_map.size() * 2];
    try
    {
        TopologicalSorting(sort_arr);
    }
    catch (const std::logic_error &error)
    {
        std::cout << error.what() << std::endl;
    }
    if (!_sequential)
    {
        try
        {
            for (int i = 2 * _node_map.size() - 1; i >= 0; --i)
                if (sort_arr[i] != "")
                {
                    bool output = _node_map[sort_arr[i]]->_gate->GetOutput();
                    for (auto &&j : _circuit_multiset_map[sort_arr[i]])
                        _node_map[j]->_gate->SetInput(sort_arr[i], output);
                }
            std::cout << "\ninput   ";
            for (auto &&i : _input_gate_set)
            {
                std::cout << i << " : " << _node_map[i]->_gate->GetOutput() << "   ";
            }
            std::cout << std::endl
                      << "output    ";
            for (auto &&i : _output_gate_set)
                std::cout << i << " : " << _node_map[i]->_gate->GetOutput() << "   ";
            std::cout << std::endl
                      << std::endl;
        }
        catch (const GateError &error)
        {
            std::cerr << error.what() << "\n";
        }
        catch (const std::logic_error &error)
        {
            std::cerr << error.what() << '\n';
            _sequential = true;
            StrongConnected();
            _sequential = false;
        }
    }
    else
    {
        int index = 0;
        bool arr[2] = {false, true};
        for (auto &&i : _node_map)
            i.second->_color = white;
        try
        {
            for (auto &&i = _input_gate_set.rbegin(); i != _input_gate_set.rend(); ++i)
            {
                SetInput(*i, arr[index % 2], false);
            }
            for (int i = 2 * _node_map.size() - 1; i >= 0; --i)
                if (sort_arr[i] != "" && _node_map[sort_arr[i]]->_color != black)
                {
                    SequentialAux(sort_arr, sort_arr[i]);
                }
            seven_segment_display.Show();
        }
        catch (const GateError &error)
        {
            std::cerr << error.what() << "\n";
        }
        catch (const std::logic_error &error)
        {
            std::cerr << error.what() << '\n';
        }
        for (auto &&i : _input_gate_set)
        {
            if (i.find("CLR") != -1 || i.find("PRE") != -1)
                SetInput(i, 1, true);
        }
        while (true)
        {
            ++index;
            try
            {
                for (auto &&i = _input_gate_set.rbegin(); i != _input_gate_set.rend(); ++i)
                {
                    SetInput(*i, arr[index % 2], false);
                    //SetInput(*i, rand() % 2, false);
                }
                for (auto &&i = _input_gate_set.begin(); i != _input_gate_set.end(); ++i)
                {
                    if (_node_map[*i]->_gate->GetInput().find("input") != _node_map[*i]->_gate->GetInput().end())
                        SequentialAux(sort_arr, *i);
                }
                seven_segment_display.Show();
            }
            catch (const GateError &error)
            {
                std::cerr << error.what() << "\n";
            }
            catch (const std::logic_error &error)
            {
                std::cerr << error.what() << '\n';
            }
        }
    }
    delete[] sort_arr;
}

void Circuit::SequentialAux(std::string *sort_arr, std::string str)
{
    bool output;
    std::queue<std::string> temp_queue;
    temp_queue.push(str);
    std::string current;
    while (!temp_queue.empty())
    {
        current = temp_queue.front();
        temp_queue.pop();
        output = _node_map[current]->_gate->GetOutput();
        for (size_t i = 0; i < _node_map.size() * 2; ++i)
        {
            if (sort_arr[i] != "" && _circuit_multiset_map[current].find(sort_arr[i]) != _circuit_multiset_map[current].end())
                if (_node_map[sort_arr[i]]->_gate->GetInput().find(current)->second != output)
                {
                    _node_map[sort_arr[i]]->_gate->SetInput(current, output);
                    temp_queue.push(sort_arr[i]);
                }
        }
    }
}

void Circuit::StrongConnected()
{
    std::map<std::string, std::set<std::string>> transpose_circuit_set_map;
    std::string *sort_arr = new std::string[_node_map.size() * 2];
    for (auto &&i : _circuit_multiset_map)
        for (auto &&j : i.second)
            transpose_circuit_set_map[j].insert(i.first);
    TopologicalSorting(sort_arr);
    for (auto &&i : _node_map)
        i.second->_color = white;
    for (int i = 2 * _node_map.size() - 1; i >= 0; --i)
        if (sort_arr[i] != "")
        {
            std::string current = sort_arr[i];
            if (_node_map[current]->_color == white)
            {
                std::vector<std::string> temp_vector;
                StrongConnectedAux(current, transpose_circuit_set_map, temp_vector);
                if (temp_vector.size() >= 2 || _circuit_multiset_map[temp_vector[0]].find(temp_vector[0]) != _circuit_multiset_map[temp_vector[0]].end())
                {
                    std::cout << "circuit : ";
                    for (auto &&i : temp_vector)
                        std::cout << i << " ";
                    std::cout << std::endl;
                }
            }
        }
}

void Circuit::StrongConnectedAux(std::string current, std::map<std::string, std::set<std::string>> &transpose_circuit_set_map, std::vector<std::string> &temp_vector)
{
    _node_map[current]->_color = gray;
    temp_vector.push_back(current);
    for (auto &&i : transpose_circuit_set_map[current])
        if (_node_map[i]->_color == white)
            StrongConnectedAux(i, transpose_circuit_set_map, temp_vector);
    _node_map[current]->_color = black;
}
void Circuit::List()
{
    std::cout << std::endl;
    for (auto &&i : _node_map)
    {
        std::cout << i.first;
        if (!_circuit_multiset_map[i.first].empty())
        {
            std::cout << " : ";
            for (auto &&j : _circuit_multiset_map[i.first])
            {
                std::cout << j << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Circuit::Clear()
{
    _node_map.clear();
    _input_gate_set.clear();
    _output_gate_set.clear();
    _circuit_multiset_map.clear();
}

void Circuit::Draw(std::string filename)
{
    std::ofstream fout("draw/" + filename + ".uml");
    fout << "@startuml" << std::endl;
    for (auto &&i : _node_map)
    {
        fout << "object " << i.first << std::endl;
        fout << i.first << " : "
             << " <img:../image/" << i.second->_gate->GetName() << ".png>" << std::endl;
    }
    for (auto &&i : _node_map)
    {
        if (!_circuit_multiset_map[i.first].empty())
        {
            for (auto &&j : _circuit_multiset_map[i.first])
            {
                fout << i.first << " --> " << j << std::endl;
            }
        }
    }
    fout << "@enduml" << std::endl;
    fout.close();
}

void Circuit::Command(std::string str, std::set<std::string> &gate_set, bool sequential, std::string file_name)
{
    std::istringstream is(str);
    std::string temp_str;
    std::string temp;
    std::vector<std::string> temp_vector;
    while (is >> temp_str)
        temp_vector.push_back(temp_str);
    if (temp_vector.size() == 3)
    {
        if (temp_vector[0] == "create" && gate_set.find(temp_vector[1]) != gate_set.end())
        {
            if (temp_vector[1] == "input")
                AddInput(file_name + temp_vector[2], InputGate());
            else if (temp_vector[1] == "output")
                AddOutput(file_name + temp_vector[2], OutputGate());
            else if (temp_vector[1] == "ANDGate")
                AddGate(file_name + temp_vector[2], ANDGate());
            else if (temp_vector[1] == "ORGate")
                AddGate(file_name + temp_vector[2], ORGate());
            else if (temp_vector[1] == "NOTGate")
                AddGate(file_name + temp_vector[2], NOTGate());
            else if (temp_vector[1] == "XORGate")
                AddGate(file_name + temp_vector[2], XORGate());
            else if (temp_vector[1] == "XNORGate")
                AddGate(file_name + temp_vector[2], XNORGate());
            else if (temp_vector[1] == "NANDGate")
                AddGate(file_name + temp_vector[2], NANDGate());
            else
                AddGate(file_name + temp_vector[2], NORGate());
        }
        else if (temp_vector[0] == "setinput" && (temp_vector[2] == "1" || temp_vector[2] == "0"))
            SetInput(file_name + temp_vector[1], bool(temp_vector[2] == "1"), true);
        else if (temp_vector[0] == "wire")
            AddWire(file_name + temp_vector[1], file_name + temp_vector[2]);
        else if (temp_vector[0] == "remove_wire")
            RemoveWire(file_name + temp_vector[1], file_name + temp_vector[2]);
        else if (temp_vector[0] == "connect")
            Connect(file_name + temp_vector[1], temp_vector[2]);
        else if (file_name.empty())
            std::cout << "\nNo command\n\ninput help to view help\n\n";
    }
    else if (temp_vector.size() == 1 && temp_vector[0] == "list")
        List();
    else if (temp_vector.size() == 1 && temp_vector[0] == "exec")
        Exec();
    else if (temp_vector.size() == 1 && temp_vector[0] == "clear")
        Clear();
    else if (temp_vector.size() == 2 && temp_vector[0] == "draw")
        Draw(temp_vector[1]);
    else if (temp_vector.size() == 2 && temp_vector[0] == "remove")
        Remove(file_name + temp_vector[1]);
    else if (temp_vector.size() == 2 && temp_vector[0] == "setfrequency" && sequential)
        SetFrequency(atof(temp_vector[1].c_str()));
    else if (temp_vector.size() == 4 && temp_vector[0] == "import" && temp_vector[2] == "as")
    {
        std::string s;
        std::ifstream fin("module/" + temp_vector[1] + ".txt");
        if (fin.fail())
            std::cerr << "can't open " << temp_vector[1] << std::endl;
        else
            while (std::getline(fin, s) && s != "quit")
            {
                Command(s, gate_set, sequential, file_name + temp_vector[3] + ".");
            }
    }
    else if (temp_vector.size() == 1 && temp_vector[0] == "help")
        Help();
    else if (file_name.empty())
        std::cout << "\nNo command\n\ninput help to view help\n\n";
}
