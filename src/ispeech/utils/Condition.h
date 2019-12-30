#ifndef _ISPEECH_CONDITION_H
#define _ISPEECH_CONDITION_H


namespace vfx_ispeech
{
#ifdef OS_WIN32
#include <windows.h>
  class Condition {
  protected:
    int waitersCount;
    CRITICAL_SECTION conditionLock;
    CRITICAL_SECTION waitersCountLock;
    HANDLE event;

  public:
    Condition() {
      InitializeCriticalSection(&conditionLock);
      InitializeCriticalSection(&waitersCountLock);
      event = CreateEvent (NULL,  // no security
			   FALSE, // auto-reset event
			   FALSE, // non-signaled initially
			   NULL); // unnamed
      waitersCount = 0;
    }

    void lock()   { EnterCriticalSection(&conditionLock); }
    void unlock() { LeaveCriticalSection(&conditionLock); }

    void wait() {
      EnterCriticalSection(&waitersCountLock);
      waitersCount++;
      LeaveCriticalSection(&waitersCountLock);

      LeaveCriticalSection(&conditionLock);

      int result = WaitForSingleObject(event, INFINITE);

      EnterCriticalSection(&waitersCountLock);
      waitersCount--;
      LeaveCriticalSection(&waitersCountLock);

      EnterCriticalSection(&conditionLock);
    }

    void signal() {
      // Avoid race conditions.
      EnterCriticalSection(&waitersCountLock);
      bool haveWaiters = waitersCount > 0;
      LeaveCriticalSection(&waitersCountLock);

      if (haveWaiters)
	SetEvent(event);
    }
};


#else // OS_WIN32

#include <pthread.h>

class Condition {
 protected:
  pthread_mutex_t pthreadMutex;
  pthread_cond_t pthreadCondition;

 public:
  Condition() {
    pthread_mutex_init(&pthreadMutex,0);
    pthread_cond_init(&pthreadCondition,0);
  }

  void lock()   { pthread_mutex_lock(&pthreadMutex); }
  void unlock() { pthread_mutex_unlock(&pthreadMutex); }
  void wait()   { pthread_cond_wait(&pthreadCondition, &pthreadMutex); }
  void signal() { pthread_cond_signal(&pthreadCondition); }

};

#endif // OS_WIN32

class MutexLocker {
 protected:
  Condition& _mutex;
 public:
  MutexLocker(ForcedMutex& mutex) : _mutex(mutex) { _mutex.lock(); }
  ~MutexLocker() { _mutex.unlock(); }
};
}
#endif /* _ISPEECH_CONDITION_H */
