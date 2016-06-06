#ifndef _RESOURCEASYNC_H_
#define _RESOURCEASYNC_H_

#include <Windows.h>
#include <mutex>
#include <queue>

#include "resource.h"

#include "gfx.h"
#include "window.h"

size_t ResourceQueueCount();

class ResourceTask
{
public:
    virtual void Execute() = 0;
};

template<typename T>
class ReadResourceTask : public ResourceTask
{
public:
    ReadResourceTask(ResHdl<T> handle, std::string resource_name)
        : handle(handle), resource_name(resource_name)
    {

    }

    void Execute()
    {
        if (ResourceAsync::IsInitialized())
            GFXSetThreadingContext(true);
        T data = T::Create();
        if (Resource<T>::ReadFile(resource_name, data))
        {
            ResHdl<T>::Replace(handle, data);
        }
        if (ResourceAsync::IsInitialized())
            GFXSetThreadingContext(false);
    }
protected:
    ResHdl<T> handle;
    std::string resource_name;
};

class ResourceAsync
{
public:
    static bool Init()
    {
        thread_handle = CreateThread(0, 0, ResourceAsync::ThreadProc, 0, 0, &thread_index);
        initialized = true;
        return true;
    }
    static void Cleanup()
    {
        TerminateThread(thread_handle, 0);
    }
    static bool IsInitialized()
    {
        return initialized;
    }
    static bool IsAlive()
    {
        DWORD result = WaitForSingleObject(thread_handle, 0);
        if (result == WAIT_OBJECT_0)
            return false;
        else
            return true;
    }
	static size_t QueueCount()
	{
		return queuedResourceCount;
	}
    static void PrintDebugInfo()
    {
        std::cout << "ResourceAsync thread status: " << IsAlive() << std::endl;
        std::cout << "Queue len: " << task_queue.size() << std::endl;
    }
    template<typename T>
    static void Post(ReadResourceTask<T> task)
    {
        std::lock_guard<std::recursive_mutex> lock(sync_queue);
        task_queue.push(new ReadResourceTask<T>(task));
		queuedResourceCount++;
        //ResumeThread(thread_handle);
    }
private:
    static HDC deviceContext;
    static HGLRC renderingContext;
    static bool initialized;
    static std::queue<ResourceTask*> task_queue;
    static std::recursive_mutex sync_queue;
    static DWORD thread_index;
    static HANDLE thread_handle;
	static int queuedResourceCount;

    static DWORD WINAPI ThreadProc(LPVOID lpParameter)
    {
        while (true)
        {
            if (task_queue.size() == 0)
                continue;

			ResourceTask* task = 0;

            {
                std::lock_guard<std::recursive_mutex> lock(sync_queue);
                
                task = task_queue.front();
                task_queue.pop();

				queuedResourceCount--;
            }

			if (task)
			{
				task->Execute();
				delete task;
			}
        }
        return 0;
    }
};

#endif