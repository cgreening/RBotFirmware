// RBotFirmware
// Rob Dobson 2016-2018

#pragma once

#include "math.h"
#include "AxisValues.h"
#include "../AxesParams.h"

class MotionBlock
{
public:
    // Minimum move distance
    static constexpr double MINIMUM_MOVE_DIST_MM = 0.0001;

    // Number of ticks to accumulate for rate actuation
    static constexpr uint32_t TTICKS_VALUE = 1000000000l;

    // Tick interval in NS
    // 20000NS means max of 25k steps per second (as each step requires 2 entries to ISR - at least)
    // The ISR time is now averaging 1.3uS and max 2.8uS so this could be reduced to 10000 if needed
    static constexpr uint32_t TICK_INTERVAL_NS = 20000;
    static constexpr float TICKS_PER_SEC = (1e9f / TICK_INTERVAL_NS);

    // Number of ns in ms
    static constexpr uint32_t NS_IN_A_MS = 1000000;

public:
    // Max speed for move (maybe reduced by feedrate in a GCode command)
    float _feedrateMMps;
    // Distance (pythagorean) to move considering primary axes only
    float _moveDistPrimaryAxesMM;
    // Computed max entry speed for a block based on max junction deviation calculation
    float _maxEntrySpeedMMps;
    // Computed entry speed for this block
    float _entrySpeedMMps;
    // Computed exit speed for this block
    float _exitSpeedMMps;
    // End-stops to test
    AxisMinMaxBools _endStopsToCheck;
    // Numbered command index - to help keep track of block execution from other processes
    // like homing
    int _numberedCommandIndex;

    // Flags
    struct
    {
        // Flag indicating the block is currently executing
        volatile bool _isExecuting : 1;
        // Flag indicating the block can start executing
        volatile bool _canExecute : 1;
        // Block is followed by others
        bool _blockIsFollowed : 1;
    };

    // Steps to target and before deceleration
    int32_t _stepsTotalMaybeNeg[RobotConsts::MAX_AXES];
    int _axisIdxWithMaxSteps;
    uint32_t _stepsBeforeDecel;

    // Stepping acceleration/deceleration profile
    uint32_t _initialStepRatePerTTicks;
    uint32_t _maxStepRatePerTTicks;
    uint32_t _finalStepRatePerTTicks;
    uint32_t _accStepsPerTTicksPerMS;

public:
    MotionBlock()
    {
        clear();
    }

    void clear()
    {
        // Clear values
        _feedrateMMps = 0;
        _moveDistPrimaryAxesMM = 0;
        _maxEntrySpeedMMps = 0;
        _entrySpeedMMps = 0;
        _exitSpeedMMps = 0;
        _isExecuting = false;
        _canExecute = false;
        _blockIsFollowed = false;
        _axisIdxWithMaxSteps = 0;
        _accStepsPerTTicksPerMS = 0;
        _finalStepRatePerTTicks = 0;
        _initialStepRatePerTTicks = 0;
        _maxStepRatePerTTicks = 0;
        _stepsBeforeDecel = 0;
        _numberedCommandIndex = 0;
        _endStopsToCheck.none();
        for (int axisIdx = 0; axisIdx < RobotConsts::MAX_AXES; axisIdx++)
            _stepsTotalMaybeNeg[axisIdx] = 0;
    }

    void setNumberedCommandIndex(int cmdIdx);
    int IRAM_ATTR getNumberedCommandIndex();
    int32_t getStepsToTarget(int axisIdx);
    int32_t getAbsStepsToTarget(int axisIdx);
    void setStepsToTarget(int axisIdx, int32_t steps);
    uint32_t getExitStepRatePerTTicks();
    static float maxAchievableSpeed(float acceleration, float target_velocity, float distance);
    void forceInBounds(float &val, float lowBound, float highBound);
    void setEndStopsToCheck(AxisMinMaxBools &endStopCheck);

    // The block's entry and exit speed are now known
    // The block can accelerate and decelerate as required as long as these criteria are met
    // We now compute the stepping parameters to make motion happen
    bool prepareForStepping(AxesParams &axesParams);

    // Debug
    void debugShowBlkHead();
    void debugShowBlock(int elemIdx, AxesParams &axesParams);
};
