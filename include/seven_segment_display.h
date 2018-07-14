#ifndef SEVEN_SEGMENT_DISPLAY_H_
#define SEVEN_SEGMENT_DISPLAY_H_
#include "gate.h"
#include <map>
#include <memory>

class SevenSegmentDisplay
{
  int _x;
  int _y;
  std::map<std::string, std::shared_ptr<Gate>> _input_map[14];
  std::map<std::string, std::shared_ptr<Gate>> _position_map;

public:
  void SetPosition(int, const std::shared_ptr<Gate> &gate);
  SevenSegmentDisplay();
  void Show();
  void SetInput(std::string c, const std::shared_ptr<Gate> &gate);
};

#endif