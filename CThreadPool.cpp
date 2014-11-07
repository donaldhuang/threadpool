
#include "CThreadPool.h"

CThreadPool::CThreadPool() 
{ 
    m_MaxNum = 50; 
    m_AvailLow = 5; 
    m_InitNum = m_AvailNum = 10 ;   
    m_AvailHigh = 20;
    m_BusyList.clear();
    m_IdleList.clear();
    for(int i=0;i<m_InitNum;i++)
    { 
        CWorkerThread* thr = new CWorkerThread(); 
        thr->SetThreadPool(this); 
        AppendToIdleList(thr); 
        thr->Start(thr);
//      thr->Run();
    } 
} 
 
CThreadPool::CThreadPool(int initnum) 
{ 
    assert(initnum>=2 && initnum<=30); 
    m_MaxNum   = 15; 
    m_AvailLow = initnum-10>0?initnum-10:1;
    m_InitNum = initnum; 
    m_AvailNum = 0;
    m_AvailHigh = initnum + 3; 
    m_BusyList.clear(); 
    m_IdleList.clear(); 
    for(int i=0;i<m_InitNum;i++)
    {
        CWorkerThread* thr = new CWorkerThread(); 
        AppendToIdleList(thr); 
        thr->SetThreadPool(this); 
        thr->Start(thr);    //begin the thread,the thread wait for job
    } 
} 
CThreadPool::~CThreadPool() 
{ 
   TerminateAll(); 
} 
 
void CThreadPool::TerminateAll() 
{ 
//    for(int i=0;i < m_ThreadList.size();i++) 
//    { 
//        CWorkerThread* thr = m_ThreadList[i]; 
//        thr->Join(); 
//    } 
    return; 
} 
 
CWorkerThread* CThreadPool::GetIdleThread(void) 
{ 
    while(m_IdleList.size() == 0 ) 
        usleep(10);
//        m_IdleCond.Wait(); 
    m_IdleMutex.Lock(); 
    if(m_IdleList.size() > 0 ) 
    { 
        CWorkerThread* thr = (CWorkerThread*)m_IdleList.front(); 
        printf("CThreadPool::GetIdleThread|get an idle thread, id is:%lu\n",thr->GetThreadID());
        m_IdleMutex.Unlock(); 
        return thr; 
    } 
    m_IdleMutex.Unlock(); 
    return NULL; 
} 
 
//add an idle thread to idle list 
void CThreadPool::AppendToIdleList(CWorkerThread* jobthread) 
{ 
    m_IdleMutex.Lock(); 
    m_IdleList.push_back(jobthread); 
    m_AvailNum++;
    printf("CThreadPool::AppendToIdleList|idlethread list number:%d\n",m_AvailNum);
    m_IdleMutex.Unlock(); 
} 
 
//move an idle thread to busy thread 
void CThreadPool::MoveToBusyList(CWorkerThread* idlethread) 
{ 
    m_BusyMutex.Lock(); 
    m_BusyList.push_back(idlethread); 
    m_BusyMutex.Unlock(); 
    printf("CThreadPool::MoveToBusyList|busythread list number:%d\n",GetBusyNum());
    
    m_IdleMutex.Lock(); 
    vector<CWorkerThread*>::iterator pos; 
    pos = find(m_IdleList.begin(),m_IdleList.end(),idlethread); 
    if(pos != m_IdleList.end())
    { 
        m_IdleList.erase(pos);
        m_AvailNum--;  
    }
    m_IdleMutex.Unlock(); 
    printf("CThreadPool::MoveToBusyList|idlethread list number:%d\n",m_AvailNum);

} 
 
void CThreadPool::MoveToIdleList(CWorkerThread* busythread) 
{ 
    m_IdleMutex.Lock(); 
    m_IdleList.push_back(busythread); 
    m_AvailNum++; 
    m_IdleMutex.Unlock(); 
    printf("CThreadPool::MoveToIdleList|idlethread list number:%d\n",m_AvailNum);
         
    m_BusyMutex.Lock(); 
    vector<CWorkerThread*>::iterator pos; 
    pos = find(m_BusyList.begin(),m_BusyList.end(),busythread); 
    if(pos!=m_BusyList.end()) 
        m_BusyList.erase(pos); 
    m_BusyMutex.Unlock(); 
    printf("CThreadPool::MoveToIdleList|busythread list number:%d\n",GetBusyNum());
    
    if(GetActualAvailNum() > GetAvailHighNum()) 
        DeleteIdleThread(GetActualAvailNum() - GetAvailHighNum());
} 

//create num idle thread and put them to idlelist 
void CThreadPool::CreateIdleThread(int num) 
{ 
    printf("CThreadPool::CreateIdleThread|create %d idle threads for need\n",num);
    for(int i=0;i<num;i++)
    { 
        CWorkerThread* thr = new CWorkerThread(); 
        thr->SetThreadPool(this); 
        AppendToIdleList(thr); 
        thr->Start(thr); //begin the thread,the thread wait for job        
    } 
} 

void CThreadPool::DeleteIdleThread(int num) 
{ 
    printf("CThreadPool::DeleteIdleThread|delete %d idle threads for no need\n",num);
    for(int i=0;i<num;i++)
    { 
        CWorkerThread* thr = GetIdleThread(); 
        m_IdleMutex.Lock(); 
        vector<CWorkerThread*>::iterator pos; 
        pos = find(m_IdleList.begin(),m_IdleList.end(),thr); 
        if(pos!=m_IdleList.end())
        { 
            m_IdleList.erase(pos);
            m_AvailNum--;
        }
        m_IdleMutex.Unlock();
    } 
    
} 
 
void CThreadPool::Run(CJob* job,void* jobdata) 
{ 
    assert(job!=NULL); 
    //if the busy thread num adds to m_MaxNum,so we should wait 
    while(GetBusyNum() == m_MaxNum)  
        usleep(10);
//        m_MaxNumCond.Wait(); 
    if(m_IdleList.size()<m_AvailLow && GetAllNum() < m_MaxNum)
    { 
        printf("CThreadPool::Run|begin to create more thread.\n");
        CreateIdleThread(m_AvailLow-m_IdleList.size());
    }
    CWorkerThread*  idlethr = GetIdleThread(); 
    if(idlethr != NULL) 
    { 
        MoveToBusyList(idlethr); 
        idlethr->SetThreadPool(this); 
        job->SetWorkThread(idlethr); 
        printf("CThreadPool::Run|Job is set to thread:%lu\n", idlethr->GetThreadID()); 
        idlethr->SetJob(job,jobdata); 
    } 
} 

