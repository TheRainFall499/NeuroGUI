#ifndef WRITEDATATHREAD_H
#define WRITEDATATHREAD_H
#include <QTCore>


class WriteDataThread : public QThread
{

public:


    void writedata(int halftowrite, bool stopwriting);
    void run(int memhalftowrite,bool inputbool);
    void reset_blocks();

private:


};

#endif // WRITEDATATHREAD_H
