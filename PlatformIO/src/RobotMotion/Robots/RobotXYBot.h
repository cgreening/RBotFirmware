// RBotFirmware
// Rob Dobson 2017

#pragma once

#include "Utils.h"
#include "RobotBase.h"
#include "math.h"

class AxisFloats;
class AxisPosition;
class MotionHelper;
class AxesParams;
class AxisInt32s;

class RobotXYBot : public RobotBase
{
public:

    RobotXYBot(const char* pRobotTypeName, MotionHelper& motionHelper);

    // Convert a cartesian point to actuator coordinates
    static bool ptToActuator(AxisFloats& targetPt, AxisFloats& outActuator, 
                AxisPosition& curPos, AxesParams& axesParams, bool allowOutOfBounds);

    // Convert actuator values to cartesian point
    static void actuatorToPt(AxisFloats& targetActuator, AxisFloats& outPt,
                AxisPosition& curPos, AxesParams& axesParams);

    // Correct overflow (necessary for continuous rotation robots)
    static void correctStepOverflow(AxisPosition& curPos, AxesParams& axesParams);
};
