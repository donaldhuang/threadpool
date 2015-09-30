#ifndef CJOB_H
#define CJOB_H

#include "CThread.h"
#include <cstddef>
#include <cstdio>
using namespace std;

class CJob 
{ 
private: 
//    int      m_JobNo;        //The num was assigned to the job 
//    char*    m_JobName;      //The job name 
    CThread  *m_pWorkThread;     //The thread associated with the job 
public: 
    CJob( void ){}
    //析构函数定义为虚函数，则是在多态的时候，基类的析构是先析构派生类，再析构父类
    virtual ~CJob(){}
    
//    int      GetJobNo(void) const { return m_JobNo; } 
//    void     SetJobNo(int jobno){ m_JobNo = jobno;} 
//    
//    char*    GetJobName(void) const { return m_JobName; } 
//    void     SetJobName(char* jobname); 
    
    CThread *GetWorkThread(void){ return m_pWorkThread; }
    void     SetWorkThread ( CThread *pWorkThread ){ m_pWorkThread = pWorkThread; }
     
    virtual void Run ( void *ptr ) = 0; 
}; 

#endif

