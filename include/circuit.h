#ifndef CIRCUIGate_H_
#define CIRCUIGate_H_
#include "gate.h"
#include "seven_segment_display.h"
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <vector>

enum Color
{
    white,
    gray,
    black
};

class Circuit
{
  private:
    void DFSAux(std::string, size_t *);
    bool _sequential;
    SevenSegmentDisplay seven_segment_display;
    double _frequency;

  public:
    struct Node
    {
        Node(std::shared_ptr<Gate>);
        std::shared_ptr<Gate> _gate;
        std::size_t _begin;
        std::size_t _end;
        Color _color;
        ~Node();
    };
    Circuit(bool);
    std::map<std::string, std::shared_ptr<Node>> _node_map;
    std::set<std::string> _input_gate_set;
    std::set<std::string> _output_gate_set;
    std::map<std::string, std::multiset<std::string>> _circuit_multiset_map;
    std::map<std::string, std::set<std::string>> _circuit_set_map;
    void DFS();
    void AddGate(std::string, const Gate &gate);
    void AddInput(std::string, const Gate &gate);
    void AddOutput(std::string, const Gate &gate);
    void SetInput(std::string, bool, bool);
    void List();
    void AddWire(std::string, std::string);
    void RemoveWire(std::string, std::string);
    void StrongConnected();
    void StrongConnectedAux(std::string, std::map<std::string, std::set<std::string>> &, std::vector<std::string> &);
    void Remove(std::string);
    void TopologicalSorting(std::string *);
    void Clear();
    void Draw(std::string);
    void Exec();
    void SetFrequency(double);
    void Connect(std::string, std::string);
    void Import(std::string, std::string);
    void Command(std::string str, std::set<std::string> &gate_set, bool sequential, std::string file_name);
    void Help();
    void SequentialAux(std::string *, std::string);
    void SetSevenSegmentDisplayPosition();
};

#endif