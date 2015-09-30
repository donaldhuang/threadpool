#include "CJob.h"
#include "CThreadManage.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

using namespace std;

class CXJob:public CJob 
{ 
    public: 
        CXJob(){} 
        ~CXJob(){} 
        void Run(void* jobdata)
        { 
            printf("The Job comes from CXJOB, thread id is:%lu\n", pthread_self()); 
            //        sleep(2); 
        } 
};

class CYJob:public CJob 
{ 
    public: 
        CYJob(){} 
        ~CYJob(){} 
        void Run(void* jobdata)
        { 
            printf("The Job comes from CYJob, thread id is:%lu\n", pthread_self());
        } 
}; 

int main(void) 
{ 
    CThreadManage* manage = new CThreadManage(3); 
    //  for(int i=0;i<20;i++)
    // { 
    CXJob* jobx = new CXJob();
    manage->Run(jobx,NULL);
    //     printf("........................deal job: %d....................\n", i+1);
    //} 
    sleep(5); 
    CYJob* job = new CYJob(); 
    manage->Run(job,NULL); 
    manage->TerminateAll(); 
    sleep(10);
    return 0;
}

