#ifndef GATE_H_
#define GATE_H_
#include <map>

class Gate
{
protected:
  std::multimap<std::string, bool> _input_map;
  size_t _max_indegree;

public:
  Gate(size_t);
  void InsertInput(std::string, bool);
  void SetInput(std::string, bool);
  std::multimap<std::string, bool> GetInput() const;
  bool virtual GetOutput() const = 0;
  void RemoveInput(std::string);
  virtual std::string GetName() const = 0;
  virtual Gate *Clone() const = 0;
  virtual ~Gate();
  static bool _default_input;
};

class OneInputGate : public Gate
{
public:
  OneInputGate();
};

class TwoInputGate : public Gate
{
public:
  TwoInputGate();
};

class ManyInputGate : public Gate
{
public:
  ManyInputGate();
};

class InputGate : public OneInputGate
{
public:
  bool GetOutput() const override;
  std::string GetName() const override;
  InputGate *Clone() const override;
};

class OutputGate : public OneInputGate
{
public:
  bool GetOutput() const override;
  std::string GetName() const override;
  OutputGate *Clone() const override;
};

class NOTGate : public OneInputGate
{
public:
  bool GetOutput() const override;
  NOTGate *Clone() const override;
  std::string GetName() const override;
};

class ANDGate : public ManyInputGate
{
public:
  bool GetOutput() const override;
  ANDGate *Clone() const override;
  std::string GetName() const override;
};
class ORGate : public ManyInputGate
{
public:
  bool GetOutput() const override;
  std::string GetName() const override;
  ORGate *Clone() const override;
};
class XORGate : public TwoInputGate
{
public:
  bool GetOutput() const override;
  std::string GetName() const override;
  XORGate *Clone() const override;
};

class XNORGate : public TwoInputGate
{
public:
  bool GetOutput() const override;
  std::string GetName() const override;
  XNORGate *Clone() const override;
};

class NANDGate : public ManyInputGate
{
public:
  bool GetOutput() const override;
  std::string GetName() const override;
  NANDGate *Clone() const override;
};

class NORGate : public ManyInputGate
{
public:
  bool GetOutput() const override;
  std::string GetName() const override;
  NORGate *Clone() const override;
};

#endif