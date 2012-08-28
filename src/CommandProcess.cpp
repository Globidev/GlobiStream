#include "CommandProcess.h"

void CommandProcess::init()
{
    // Starting MS-DOS
    start(WINDOWS_CMD_PROCESS);
    waitForStarted();
}

void CommandProcess::end()
{
    // Exiting MS-DOS
    if(state() == QProcess::Running)
    {
        write(EXIT_COMMAND);
        if(!waitForFinished(EXIT_TIMEOUT)) // Failed to exit properly
            terminate();
    }
    
    if(!waitForFinished(EXIT_TIMEOUT)) // Process is stuck
        kill(); // Force exit
    waitForFinished();
    disconnect();
}

void CommandProcess::waitForComputingFinished()
{
    // Looping while the process computes
    // Calling processEvents inside the loop to make the GUI responsive
    int timeOut(15000);
    while(state() == QProcess::Running && timeOut)
    {
        QApplication::processEvents();
        Sleep(1);
        timeOut --;
    }
}