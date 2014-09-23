#include "ULAI06.h"

void Test::DisplayData()
{
    /* Variable Declarations */
    int Row, Col;
    int BoardNum = 0;
    int ULStat = 0;
    int LowChan = 0;
    int HighChan = 0;
    int Gain = BIP5VOLTS;
    short Status = RUNNING;
    long CurCount;
    long CurIndex;
    long Count = 10000;
    long Rate = 390;
    HANDLE MemHandle = 0;
    WORD *ADData;
    DWORD *ADData32;
    unsigned Options;
    float revision = (float)CURRENTREVNUM;
    BOOL HighResAD = FALSE;
    int ADRes;

   /* Declare Revision level of the Universal Library */
    ULStat = cbDeclareRevision(&revision);

    /* Initiate error handling
        Parameters:
            PRINTALL :all warnings and errors encountered will be printed
            DONTSTOP :program will continue even if error occurs.
                     Note that STOPALL and STOPFATAL are only effective in
                     Windows applications, not Console applications.
   */
    cbErrHandling (PRINTALL, DONTSTOP);

    /* Get the resolution of A/D */
    cbGetConfig(BOARDINFO, BoardNum, 0, BIADRES, &ADRes);

    /* check If the resolution of A/D is higher than 16 bit.
       If it is, then the A/D is high resolution. */
    if(ADRes > 16)
        HighResAD = TRUE;

    /*  set aside memory to hold data */
    if(HighResAD)
    {
        MemHandle = cbWinBufAlloc32(Count);
        ADData32 = (DWORD*) MemHandle;
    }
    else
    {
        MemHandle = cbWinBufAlloc(Count);
        ADData = (WORD*) MemHandle;
    }

    if (!MemHandle)    /* Make sure it is a valid pointer */
    {
        printf("\nout of memory\n");
        exit(1);
    }

    /* set up the display screen */
    ClearScreen();
    printf ("Demonstration of cbAInScan()\n\n");
    printf ("Data are being collected in the BACKGROUND, CONTINUOUSLY\n");


    /* Collect the values with cbAInScan() in BACKGROUND mode, CONTINUOUSLY
        Parameters:
             BoardNum    :the number used by CB.CFG to describe this board
             LowChan     :low channel of the scan
             HighChan    :high channel of the scan
             Count       :the total number of A/D samples to collect
             Rate        :sample rate in samples per second
             Gain        :the gain for the board
             ADData[]    :the array for the collected data values
             Options     :data collection options */
    Options = CONVERTDATA + BACKGROUND + CONTINUOUS;
    ULStat = cbAInScan (BoardNum, LowChan, HighChan, Count, &Rate, Gain,
                        MemHandle, Options);

    printf ("--------------------------------------------------------------------------------");
    printf ("| Your program could be doing something useful here while data are collected...|");
    printf ("--------------------------------------------------------------------------------");
    printf ("\nCollecting data...\n\n");
    printf ("Press any key to quit.\n\n");

    GetTextCursor (&Col, &Row);

    /* During the BACKGROUND operation, check the status */
    while (!kbhit() && Status==RUNNING)
    {
        /* Check the status of the current background operation
        Parameters:
            BoardNum  :the number used by CB.CFG to describe this board
            Status    :current status of the operation (IDLE or RUNNING)
            CurCount  :current number of samples collected
            CurIndex  :index to the last data value transferred
            FunctionType: A/D operation (AIFUNCTIOM)*/
        ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex,AIFUNCTION);

        /* check the current status of the background operation */
        if ((Status == RUNNING) && CurCount > 0)
        {
            MoveCursor (Col, Row);
            printf ("Data point: %3ld   ", CurIndex);
            if(HighResAD)
                printf ("  Value: %lu  ",ADData32[CurIndex]);
            else
                printf ("  Value: %hu  ",ADData[CurIndex]);
        }
    }
    printf ("\n");
    MoveCursor (Col, Row + 2);
    printf ("Data collection terminated.");

    /* The BACKGROUND operation must be explicitly stopped
        Parameters:
             BoardNum    :the number used by CB.CFG to describe this board
             FunctionType: A/D operation (AIFUNCTIOM)*/
    ULStat = cbStopBackground (BoardNum,AIFUNCTION);

    cbWinBufFree(MemHandle);
    MoveCursor (1, 22);
    printf ("\n");
}




/***************************************************************************
*
* Name:      ClearScreen
* Arguments: ---
* Returns:   ---
*
* Clears the screen.
*
***************************************************************************/

#define BIOS_VIDEO   0x10

void Test::ClearScreen()
{
    COORD coordOrg = {0, 0};
    DWORD dwWritten = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (INVALID_HANDLE_VALUE != hConsole)
        FillConsoleOutputCharacter(hConsole, ' ', 80 * 50, coordOrg, &dwWritten);

    MoveCursor(0, 0);

    return;
}


/***************************************************************************
*
* Name:      MoveCursor
* Arguments: x,y - screen coordinates of new cursor position
* Returns:   ---
*
* Positions the cursor on screen.
*
***************************************************************************/


void Test::MoveCursor(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (INVALID_HANDLE_VALUE != hConsole)
    {
        COORD coordCursor;
        coordCursor.X = (short)x;
        coordCursor.Y = (short)y;
        SetConsoleCursorPosition(hConsole, coordCursor);
    }

    return;
}


/***************************************************************************
*
* Name:      GetTextCursor
* Arguments: x,y - screen coordinates of new cursor position
* Returns:   *x and *y
*
* Returns the current (text) cursor position.
*
***************************************************************************/

void Test::GetTextCursor(int *x, int *y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    *x = -1;
    *y = -1;
    if (INVALID_HANDLE_VALUE != hConsole)
    {
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        *x = csbi.dwCursorPosition.X;
        *y = csbi.dwCursorPosition.Y;
    }

    return;
}
