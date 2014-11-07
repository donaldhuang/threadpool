#ifndef CWORKERTHREAD_H
#define CWORKERTHREAD_H

#include "CThread.h"
#include "CJob.h"
#include <cstdlib>
#include <unistd.h>

using namespace std;

class CThreadPool;

class CWorkerThread : public CThread 
{
private:
    CThreadPool* m_ThreadPool;
    CJob* m_Job;
    void* m_JobData;

protected:
public:
    CWorkerThread();
    virtual ~CWorkerThread();

    void Run();

    void SetJob(CJob* job,void* jobdata);
    CJob* GetJob(void){return m_Job;}

    void SetThreadPool(CThreadPool* thrpool);
    CThreadPool* GetThreadPool(void){return m_ThreadPool;}
};

#endif

