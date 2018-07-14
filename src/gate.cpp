#include "gate.h"
#include "error.h"

bool Gate::_default_input = true;

Gate::Gate(size_t max_indegree) : _max_indegree(max_indegree) {}

Gate::~Gate() = default;

void Gate::InsertInput(std::string str, bool input)
{
    if (_input_map.size() == _max_indegree)
    {
        if (_input_map.begin()->first == "user_input")
        {
            _input_map.insert({str, _input_map.begin()->second});
            _input_map.erase("user_input");
        }
        else
            throw GateError(" has full inputs");
    }
    else
        _input_map.insert({str, input});
}

void Gate::SetInput(std::string str, bool input)
{
    for (auto pos = _input_map.equal_range(str); pos.first != pos.second; ++pos.first)
        pos.first->second = input;
}

std::multimap<std::string, bool> Gate::GetInput() const
{
    return _input_map;
}

void Gate::RemoveInput(std::string str)
{
    auto pos = _input_map.find(str);
    if (pos == _input_map.end())
        throw GateError("no this input");
    else
        _input_map.erase(pos);
}

OneInputGate::OneInputGate() : Gate(1) {}

TwoInputGate::TwoInputGate() : Gate(2) {}

ManyInputGate::ManyInputGate() : Gate(-1) {}

bool InputGate::GetOutput() const
{
    if (_input_map.empty())
        throw GateError("No input");
    else
        return _input_map.cbegin()->second;
}

std::string InputGate::GetName() const
{
    return "InputGate";
}

InputGate *InputGate::Clone() const
{
    return new InputGate();
}

bool OutputGate::GetOutput() const
{
    if (_input_map.empty())
        throw GateError("No output");
    else
        return _input_map.cbegin()->second;
}

std::string OutputGate::GetName() const
{
    return "OutputGate";
}

OutputGate *OutputGate::Clone() const
{
    return new OutputGate();
}

bool NOTGate::GetOutput() const
{
    if (_input_map.empty())
        return !_default_input;
    else
        return !_input_map.cbegin()->second;
}

std::string NOTGate::GetName() const
{
    return "NOTGate";
}

NOTGate *NOTGate::Clone() const
{
    return new NOTGate();
}

bool ANDGate::GetOutput() const
{
    if (_input_map.empty())
        return _default_input;
    else if (_input_map.size() == 1)
        return _input_map.cbegin()->second & _default_input;
    else
    {
        for (auto &&i : _input_map)
            if (!i.second)
                return false;
        return true;
    }
}

std::string ANDGate::GetName() const
{
    return "ANDGate";
}

ANDGate *ANDGate::Clone() const
{
    return new ANDGate();
}

bool ORGate::GetOutput() const
{
    if (_input_map.empty())
        return _default_input;
    else if (_input_map.size() == 1)
        return _input_map.cbegin()->second | _default_input;
    else
    {
        for (auto &&i : _input_map)
            if (i.second)
                return true;
        return false;
    }
}

std::string ORGate::GetName() const
{
    return "ORGate";
}

ORGate *ORGate::Clone() const
{
    return new ORGate();
}

bool XORGate::GetOutput() const
{
    if (_input_map.empty())
        return false;
    else if (_input_map.size() == 1)
        return _input_map.cbegin()->second != _default_input;
    else
        return _input_map.cbegin()->second != (++_input_map.cbegin())->second;
}

std::string XORGate::GetName() const
{
    return "XORGate";
}

XORGate *XORGate::Clone() const
{
    return new XORGate();
}

bool XNORGate::GetOutput() const
{
    if (_input_map.empty())
        return true;
    else if (_input_map.size() == 1)
        return _input_map.cbegin()->second == _default_input;
    else
        return _input_map.cbegin()->second == (++_input_map.cbegin())->second;
}

std::string XNORGate::GetName() const
{
    return "XNORGate";
}

XNORGate *XNORGate::Clone() const
{
    return new XNORGate();
}

bool NANDGate::GetOutput() const
{
    if (_input_map.empty())
        return !_default_input;
    else if (_input_map.size() == 1)
        return !(_input_map.cbegin()->second & _default_input);
    else
    {
        for (auto &&i : _input_map)
            if (!i.second)
                return true;
        return false;
    }
}

std::string NANDGate::GetName() const
{
    return "NANDGate";
}

NANDGate *NANDGate::Clone() const
{
    return new NANDGate();
}

bool NORGate::GetOutput() const
{
    if (_input_map.empty())
        return !_default_input;
    else if (_input_map.size() == 1)
        return !(_input_map.cbegin()->second | _default_input);
    else
    {
        for (auto &&i : _input_map)
            if (i.second)
                return false;
        return true;
    }
}

std::string NORGate::GetName() const
{
    return "NORGate";
}

NORGate *NORGate::Clone() const
{
    return new NORGate();
}
