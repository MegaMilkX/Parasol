#include "resourceasync.h"

HDC ResourceAsync::deviceContext;
HGLRC ResourceAsync::renderingContext;
bool ResourceAsync::initialized = false;
std::queue<ResourceTask*> ResourceAsync::task_queue;
std::recursive_mutex ResourceAsync::sync_queue;
DWORD ResourceAsync::thread_index;
HANDLE ResourceAsync::thread_handle;