#ifndef WRITEDATATHREAD_H
#define WRITEDATATHREAD_H
#include <QTCore>


class WriteDataThread : public QThread
{

public:


    void writedata(int halftowrite, bool stopwriting, long memory_location);
    void run(int memhalftowrite,bool inputbool,long mem_loc);
    void reset_blocks();

private:


};

#endif // WRITEDATATHREAD_H
