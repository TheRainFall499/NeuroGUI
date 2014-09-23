#ifndef ULAI06_H
#define ULAI06_H

/*ULAI06.C****************************************************************

File:                         ULAI06.C

Library Call Demonstrated:    cbAInScan(), continuous BACKGROUND mode

Purpose:                      Scans a range of A/D Input Channels
                              in the background.

Demonstration:                Continuously collect data on Channel 0
                              until a key is pressed.

Other Library Calls:          cbGetStatus()
                              cbStopBackground()
                              cbErrHandling()

Special Requirements:         Board 0 must have an A/D converter.
                              Analog signals on an input channel.


Copyright (c) 1995-2002, Measurement Computing Corp.
All Rights Reserved.
***************************************************************************/


/* Include files */
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "cbw.h"


class Test {

public:
    void DisplayData();

private:
    void ClearScreen();
    void GetTextCursor(int *x, int *y);
    void MoveCursor(int x, int y);

};

#endif // ULAI06_H
