// RBotFirmware
// Rob Dobson 2017

#pragma once

#include "FileManager.h"

class WorkManager;
class WorkItem;

class EvaluatorFiles
{
public:
    EvaluatorFiles(FileManager& fileManager);

    // Config
    void setConfig(const char* configStr);
    const char* getConfig();

    // Is Busy
    bool isBusy();
    
    // Check valid
    bool isValid(WorkItem& workItem);

    // Process WorkItem
    bool execWorkItem(WorkItem& workItem);

    // Call frequently
    void service(WorkManager* pWorkManager);

    // Control
    void stop();

    // File types
    enum {
        FILE_TYPE_UNKNOWN,
        FILE_TYPE_GCODE,
        FILE_TYPE_THETA_RHO
    };
    
private:
    // Filename in progress
    bool _inProgress;

    // File manager
    FileManager& _fileManager;

    // File type
    int _fileType;

    // Start of file handling
    bool _firstValidLineProcessed;

private:
    int getFileTypeFromExtension(String& fileName);

};
