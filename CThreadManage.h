#ifndef CTHREADMANAGE_H
#define CTHREADMANAGE_H

#include "CThreadPool.h"
#include "CJob.h"


class CThreadManage 
{ 
private: 
    CThreadPool* m_Pool; 
    int          m_NumOfThread; 
protected: 
public: 
    CThreadManage(); 
    CThreadManage(int num); 
    virtual ~CThreadManage(); 
//    void    SetParallelNum(int num);     
    void    Run(CJob* job,void* jobdata); 
    void    TerminateAll(void); 
};

#endif
