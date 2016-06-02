#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <map>
#include <vector>

#include <iostream>

#include "datahandle.h"
#include "resourceasync.h"

template<typename T>
class Resource
{
public:
    static void AddSearchPath(std::string path);
    static void SetFallbackData(T data);
    static ResHdl<T> Create(T data, std::string name);
    static ResHdl<T> Get(std::string name);
    
    static bool ReadFile(std::string name, T& data);
private:
    static bool ResourceExists(std::string name){ return resources.find(name) != resources.end(); }
    
    static std::map<std::string, ResHdl<T>> resources;
    static std::vector<std::string> search_paths;

    static std::recursive_mutex sync;
};

template<typename T>
std::map<std::string, ResHdl<T>> Resource<T>::resources;
template<typename T>
std::vector<std::string> Resource<T>::search_paths;
template<typename T>
std::recursive_mutex Resource<T>::sync;

template<typename T>
void Resource<T>::AddSearchPath(std::string path)
{
    CreateDirectoryA(path.c_str(), NULL);
    search_paths.push_back(path);
}

template<typename T>
void Resource<T>::SetFallbackData(T data)
{
    ResHdl<T>::SetFallbackData(data);
}

template<typename T>
ResHdl<T> Resource<T>::Create(T data, std::string name)
{
    if(ResourceExists(name))
    {
        return resources[name];
    }
    else
    {
        ResHdl<T> res = ResHdl<T>::Create(data);
        resources.insert(std::make_pair(name, res));
        return res;
    }
}

template<typename T>
ResHdl<T> Resource<T>::Get(std::string name)
{
    std::lock_guard<std::recursive_mutex> lock(sync);
    if(ResourceExists(name))
    {
        return resources[name];
    }
    else
    {
        if (ResourceAsync::IsInitialized())
        {
            ResHdl<T> hdl = ResHdl<T>::Create(ResHdl<T>::FallbackData());
            ResourceAsync::Post(ReadResourceTask<T>(hdl, name));
            resources.insert(std::make_pair(name, hdl));
            return hdl;
        }
        else
        {
            T data = T::Create();
            if (!ReadFile(name, data))
            {
                std::cout << "Load failed\n";
                return ResHdl<T>();
            }
            ResHdl<T> res = ResHdl<T>::Create(data);
            resources.insert(std::make_pair(name, res));
            return res;
        }
    }
}

template<typename T>
bool Resource<T>::ReadFile(std::string name, T& data)
{
    std::string final_path;
    int reader_id = -1;
    
    for(unsigned int i = 0; i < search_paths.size(); ++i)
    {
        std::string path = search_paths[i] + (search_paths[i][search_paths.size()-1]=='\\'?"":"\\") + name;
        
        for(int j = 0; j < T::ReaderCount(); ++j)
        {
            std::string path_w_extension = path + "." + T::GetReaderExtension(j);
            std::cout << path_w_extension << std::endl;
            if(File::Exists(path_w_extension))
            {
                final_path = path_w_extension;
                reader_id = j;
                break;
            }
        }
        if(reader_id >= 0)
            break;
    }
    
    if(search_paths.size() == 0)
    {
        for(int j = 0; j < T::ReaderCount(); ++j)
        {
            std::string path = name + "." + T::GetReaderExtension(j);
            std::cout << path << std::endl;
            if(File::Exists(path))
            {
                final_path = path;
                reader_id = j;
                break;
            }
        }
    }
    
    if(reader_id < 0)
        return false;
        
    File file = File::Open(final_path, File::READ);
    
    if(!data.Read(reader_id, file))
        return false;
        
    return true;
}

#endif
