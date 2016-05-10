#ifndef _RESOURCEREADER_H_
#define _RESOURCEREADER_H_

class ResourceReader
{
public:
    typedef bool (*read_func)(std::string, void*);
    
    ResourceReader(std::string ext, read_func func)
    {
        extension = ext;
        read = func;
    }
    virtual ~ResourceReader(){}
    bool Read(std::string path, void* data_out)
    {
        if(!read)
            return false;
        return read(path, data_out);
    }
    std::string Extension()
    {
        return extension;
    }
private:
    read_func read;
    std::string extension;
};

#endif