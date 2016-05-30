#ifndef _RESOURCEASYNC_H_
#define _RESOURCEASYNC_H_

#include <Windows.h>
#include <mutex>
#include <queue>

#include "resource.h"

#include "gfx.h"
#include "window.h"

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
    static bool Init(Window& win)
    {
        DWORD thread_index;
        thread_handle = CreateThread(0, 0, ResourceAsync::ThreadProc, 0, CREATE_SUSPENDED, &thread_index);
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
    template<typename T>
    static void Post(ReadResourceTask<T> task)
    {
        std::lock_guard<std::recursive_mutex> lock(sync_queue);
        task_queue.push(new ReadResourceTask<T>(task));
        ResumeThread(thread_handle);
    }
private:
    static HDC deviceContext;
    static HGLRC renderingContext;
    static bool initialized;
    static std::queue<ResourceTask*> task_queue;
    static std::recursive_mutex sync_queue;
    static HANDLE thread_handle;

    static DWORD WINAPI ThreadProc(LPVOID lpParameter)
    {
        while (true)
        {
            {
                std::lock_guard<std::recursive_mutex> lock(sync_queue);

                task_queue.front()->Execute();
                delete task_queue.front();
                task_queue.pop();
            }

            if (task_queue.size() == 0)
                SuspendThread(thread_handle);
        }
        return 0;
    }
};

#endif