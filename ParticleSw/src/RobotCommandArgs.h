// RBotFirmware
// Rob Dobson 2016

#pragma once

#include "application.h"
#include "AxisValues.h"

enum RobotMoveTypeArg
{
  RobotMoveTypeArg_None,
  RobotMoveTypeArg_Absolute,
  RobotMoveTypeArg_Relative
};

class RobotCommandArgs
{
private:
  bool _ptUnitsSteps : 1;
  bool _dontSplitMove : 1;
  bool _extrudeValid : 1;
  bool _feedrateValid : 1;
  bool _moveClockwise : 1;
  bool _moveRapid : 1;
  bool _allowOutOfBounds : 1;
  bool _pause : 1;
  bool _moreMovesComing : 1;
  int _queuedCommands;
  int _numberedCommandIndex;
  AxisFloats _ptInMM;
  AxisInt32s _ptInSteps;
  float _extrudeValue;
  float _feedrateValue;
  RobotMoveTypeArg _moveType;
  AxisMinMaxBools _endstops;

public:
  RobotCommandArgs()
  {
    clear();
  }
  void clear()
  {
    _ptInMM.clear();
    _ptInSteps.clear();
    _endstops.none();
    _numberedCommandIndex = RobotConsts::NUMBERED_COMMAND_NONE;
    _dontSplitMove = false;
    _extrudeValid  = false;
    _feedrateValid = false;
    _moveClockwise = false;
    _moveRapid = false;
    _moreMovesComing = false;
    _ptUnitsSteps  = false;
    _allowOutOfBounds = false;
    _pause = false;
    _extrudeValue  = 0.0;
    _feedrateValue = 0.0;
    _moveType      = RobotMoveTypeArg_None;
    _queuedCommands = 0;
  }
private:
  void copy(const RobotCommandArgs& copyFrom)
  {
    clear();
    _ptInMM        = copyFrom._ptInMM;
    _ptInSteps     = copyFrom._ptInSteps;
    _extrudeValid  = copyFrom._extrudeValid;
    _extrudeValue  = copyFrom._extrudeValue;
    _feedrateValid = copyFrom._feedrateValid;
    _feedrateValue = copyFrom._feedrateValue;
    _endstops       = copyFrom._endstops;
    _moveClockwise = copyFrom._moveClockwise;
    _moveRapid     = copyFrom._moveRapid;
    _moreMovesComing = copyFrom._moreMovesComing;
    _pause         = copyFrom._pause;
    _dontSplitMove = copyFrom._dontSplitMove;
    _allowOutOfBounds = copyFrom._allowOutOfBounds;
    _numberedCommandIndex = copyFrom._numberedCommandIndex;
    _moveType      = copyFrom._moveType;
    _queuedCommands = copyFrom._queuedCommands;
  }
public:
  RobotCommandArgs(const RobotCommandArgs& other)
  {
    copy(other);
  }
  RobotCommandArgs& operator=(const RobotCommandArgs& other)
  {
    copy(other);
    return *this;
  }
  void setAxisValMM(int axisIdx, float value, bool isValid)
  {
    if (axisIdx >= 0 && axisIdx < RobotConsts::MAX_AXES)
    {
      _ptInMM.setVal(axisIdx, value);
      _ptInMM.setValid(axisIdx, isValid);
      _ptUnitsSteps = false;
    }
  }
  void setAxisSteps(int axisIdx, int32_t value, bool isValid)
  {
    if (axisIdx >= 0 && axisIdx < RobotConsts::MAX_AXES)
    {
      _ptInSteps.setVal(axisIdx, value);
      _ptInMM.setValid(axisIdx, isValid);
      _ptUnitsSteps = true;
    }
  }
  bool isValid(int axisIdx)
  {
    return _ptInMM.isValid(axisIdx);
  }
  bool anyValid()
  {
    return _ptInMM.anyValid();
  }
  bool isStepwise()
  {
    return _ptUnitsSteps;
  }
  // Indicate that all axes need to be homed
  void setAllAxesNeedHoming()
  {
    for (int axisIdx = 0; axisIdx < RobotConsts::MAX_AXES; axisIdx++)
      setAxisValMM(axisIdx, 0, true);
  }
  inline float getValNoCkMM(int axisIdx)
  {
    return _ptInMM.getValNoCk(axisIdx);
  }
  float getValMM(int axisIdx)
  {
    return _ptInMM.getVal(axisIdx);
  }
  AxisFloats& getPointMM()
  {
    return _ptInMM;
  }
  void setPointMM(AxisFloats& ptInMM)
  {
    _ptInMM = ptInMM;
  }
  void setPointSteps(AxisInt32s& ptInSteps)
  {
    _ptInSteps = ptInSteps;
  }
  AxisInt32s& getPointSteps()
  {
    return _ptInSteps;
  }
  void setMoveType(RobotMoveTypeArg moveType)
  {
    _moveType = moveType;
  }
  RobotMoveTypeArg getMoveType()
  {
    return _moveType;
  }
  void setMoveRapid(bool moveRapid)
  {
    _moveRapid = moveRapid;
  }
  void setMoreMovesComing(bool moreMovesComing)
  {
    _moreMovesComing = moreMovesComing;
  }
  bool getMoreMovesComing()
  {
    return _moreMovesComing;
  }
  void setFeedrate(float feedrate)
  {
    _feedrateValue = feedrate;
    _feedrateValid = true;
  }
  bool isFeedrateValid()
  {
    return _feedrateValid;
  }
  float getFeedrate()
  {
    return _feedrateValue;
  }
  void setExtrude(float extrude)
  {
    _extrudeValue = extrude;
    _extrudeValid = true;
  }
  bool isExtrudeValid()
  {
    return _extrudeValid;
  }
  float getExtrude()
  {
    return _extrudeValue;
  }
  void setEndStops(AxisMinMaxBools endstops)
  {
    _endstops = endstops;
  }
  void setTestAllEndStops()
  {
    _endstops.all();
    Log.info("Test all endstops");
  }

  void setTestNoEndStops()
  {
    _endstops.none();
  }

  void setTestEndStopsDefault()
  {
    _endstops.none();
  }

  void setTestEndStop(int axisIdx, int endStopIdx, AxisMinMaxBools::AxisMinMaxEnum checkType)
  {
    _endstops.set(axisIdx, endStopIdx, checkType);
  }

  inline AxisMinMaxBools& getEndstopCheck()
  {
    return _endstops;
  }

  void setAllowOutOfBounds(bool allowOutOfBounds = true)
  {
    _allowOutOfBounds = allowOutOfBounds;
  }
  bool getAllowOutOfBounds()
  {
    return _allowOutOfBounds;
  }
  void setDontSplitMove(bool dontSplitMove = true)
  {
    _dontSplitMove = dontSplitMove;
  }
  bool getDontSplitMove()
  {
    return _dontSplitMove;
  }
  void setNumberedCommandIndex(int cmdIdx)
  {
    _numberedCommandIndex = cmdIdx;
  }
  int getNumberedCommandIndex()
  {
    return _numberedCommandIndex;
  }
  void setNumQueued(int numQueued)
  {
    _queuedCommands = numQueued;
  }
  void setPause(bool pause)
  {
    _pause = pause;
  }
  String toJSON()
  {
    String jsonStr;
    jsonStr = "{\"XYZ\":" + _ptInMM.toJSON();
    jsonStr += ",\"ABC\":" + _ptInSteps.toJSON();
    if (_feedrateValid)
    {
      String feedrateStr = String::format("%0.2f", _feedrateValue);
      jsonStr += ", \"F\":" + feedrateStr;
    }
    if (_extrudeValid)
    {
      String extrudeStr = String::format("%0.2f", _extrudeValue);
      jsonStr += ", \"E\":" + extrudeStr;
    }
    jsonStr += ", \"mv\":";
    if (_moveType == RobotMoveTypeArg_Relative)
      jsonStr += "\"rel\"";
    else
      jsonStr += "\"abs\"";
    jsonStr += ",\"end\":" + _endstops.toJSON();
    jsonStr += ",\"OoB\":" + String(_allowOutOfBounds ? "\"Y\"" : "\"N\"");
    String numberedCmdStr = String::format("%d", _numberedCommandIndex);
    jsonStr += ", \"num\":" + numberedCmdStr;
    String queuedCommandsStr = String::format("%d", _queuedCommands);
    jsonStr += ", \"Qd\":" + queuedCommandsStr;
    jsonStr += String(", \"pause\":") + (_pause ? "1" : "0");
    jsonStr += "}";
    return jsonStr;
  }

};
