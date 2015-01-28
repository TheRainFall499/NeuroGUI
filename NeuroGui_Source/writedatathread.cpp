#include "cbw.h"
#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include "writedatathread.h"

void WriteDataThread::writedata(int halftowrite,bool stopwriting)
{
    long CurCount = 10;
    long CurIndex;
    short Status = RUNNING;
    int BoardNum = 0;
    float v;
    extern WORD *ADData;
    extern int channels;
    extern HANDLE MemHandle;
    extern int channels;
    extern int globalrecfreq;
    extern bool triggerchecked;
    extern int blocks_stored;

    //if (stopwriting==false)
    //{
        if (halftowrite==2)
        {
            extern QFile f;
            QDataStream out(&f);
            out.setByteOrder(QDataStream::LittleEndian);
            for (int writedataloc=(channels*globalrecfreq); writedataloc < (channels*globalrecfreq*2); writedataloc++)
            {
                int ENG = cbToEngUnits(BoardNum,BIP10VOLTS,ADData[writedataloc],&v);
                double v_double=(double)v;
                out << v_double;
            }
        }
        if (halftowrite==1)
        {
            extern QFile f;
            QDataStream out(&f);
            out.setByteOrder(QDataStream::LittleEndian); // *** set little endian byte order
            for (int writedataloc=0; writedataloc < (channels*globalrecfreq); writedataloc++)
            {
                int ENG = cbToEngUnits(BoardNum,BIP10VOLTS,ADData[writedataloc],&v);
                double v_double=(double)v;
                out << v_double;
            }
        }
    //}
//    else
//    {
//        extern int globalrecfreq;


//        if (halftowrite==2)
//        {
//            extern QFile f;
//            QDataStream out(&f);
//            out.setByteOrder(QDataStream::LittleEndian);
//            for (int writedataloc=(channels*globalrecfreq*2); writedataloc < end_location; writedataloc++)
//            {
//                int ENG = cbToEngUnits(BoardNum,BIP10VOLTS,ADData[writedataloc],&v);
//                double v_double=(double)v;
//                out << v_double;
//            }
//        }
//        if (halftowrite==1)
//        {
//            extern QFile f;
//            QDataStream out(&f);
//            out.setByteOrder(QDataStream::LittleEndian); // *** set little endian byte order
//            for (int writedataloc=0; writedataloc < end_location; writedataloc++)
//            {
//                int ENG = cbToEngUnits(BoardNum,BIP10VOLTS,ADData[writedataloc],&v);
//                double v_double=(double)v;
//                out << v_double;
//            }
//        }
//    }

    blocks_stored=blocks_stored+1;
}

void WriteDataThread::run(int memhalftowrite,bool inputbool)
{
    writedata(memhalftowrite,inputbool);
}


