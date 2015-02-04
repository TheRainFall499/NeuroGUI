#include "cbw.h"
#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include "writedatathread.h"

void WriteDataThread::writedata(int halftowrite,bool stopwriting, long memory_location)
{
    qDebug() << "memory_location: " << memory_location;
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
    extern int Count;
    int end_location = 0;
    if (stopwriting==false)
    {
        if (halftowrite==2)
        {
            extern QFile f;
            QDataStream out(&f);
            out.setByteOrder(QDataStream::LittleEndian);
            //for (int writedataloc=(channels*globalrecfreq); writedataloc < (channels*globalrecfreq*2); writedataloc++)
            for (int writedataloc=(channels*globalrecfreq); writedataloc < Count; writedataloc++)
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
    }
    else
    {
        int test=memory_location/Count;

        int remainder=test%2;
        if (remainder == 0)
        {
            halftowrite = 1;
            end_location = memory_location - (Count*test);
        }
        else
        {
            halftowrite = 2;
            end_location = memory_location - (Count*test) + (channels*globalrecfreq);
        }

        if (halftowrite==2)
        {
            extern QFile f;
            QDataStream out(&f);
            out.setByteOrder(QDataStream::LittleEndian);
            for (int writedataloc=(channels*globalrecfreq); writedataloc < end_location; writedataloc++)
            {
                qDebug() <<"2_"<< writedataloc;
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
            for (int writedataloc=0; writedataloc < end_location; writedataloc++)
            {
                qDebug() <<"1_"<< writedataloc;
                int ENG = cbToEngUnits(BoardNum,BIP10VOLTS,ADData[writedataloc],&v);
                double v_double=(double)v;
                out << v_double;
            }
        }
    }

    blocks_stored=blocks_stored+1;
}

void WriteDataThread::run(int memhalftowrite,bool inputbool, long mem_loc)
{
    writedata(memhalftowrite,inputbool, mem_loc);
}


