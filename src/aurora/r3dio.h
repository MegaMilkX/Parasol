#ifndef _R3DIO_H_
#define _R3DIO_H_

#include <vector>
#include <map>

#include <fstream>

#define R3DIO_DEF_ATTR(NAME, TYPE, COUNT) \
    typedef Attribute<TYPE, COUNT, DataType::NAME> NAME;

class R3DIO
{
public:
    #define XFIELDS \
    X(Position, float, 3) \
    X(RGBA, unsigned char, 4) \
    X(RGB, unsigned char, 3) \
    X(ALPHA, unsigned char, 1) \
    X(Normal, float, 3) \
    X(Tangent, float, 3) \
    X(Bitangent, float, 3) \
    X(UV, float, 2) \
    X(Index, unsigned int, 1)

    struct DataType
    {
        enum TYPE
        {
            R3DBEGIN = 99,
            Position = 1,
            RGBA = 2,
            RGB = 3,
            ALPHA = 4,
            Normal = 5,
            Tangent = 6,
            Bitangent = 7,
            UV = 8,
            Index = 9,
            R3DEND = 98
        };
    };

    template<typename T, int I, DataType::TYPE DT>
    struct Attribute
    {
        T data[I];
        static DataType::TYPE Type() { return DT; }
        T& operator[](unsigned int index) { return data[index]; }
    };

    class Block
    {
    public:
        Block() {}
        Block(DataType::TYPE t, void* dta, size_t sz)
            : type(t), data(dta), size(sz) {}
        void Type(unsigned int value) { type = value; }
        unsigned int Type() { return type; }
        void* Data() { return data; }
        size_t Size() { return size; }
    private:
        unsigned int type;
        void* data;
        size_t size;
    };

    #define X(NAME, TYPE, COUNT) \
    R3DIO_DEF_ATTR(NAME, TYPE, COUNT);
    XFIELDS
    #undef X

public:
    template<typename T>
    std::vector<T> Get();
    template<typename T>
    void Set(const std::vector<T>& data);

    void Write(const std::string& filename)
    {
        std::ofstream file(filename, std::ios::binary);

        std::vector<Block> blocks;
        blocks.push_back(Block(DataType::R3DBEGIN, 0, 0));

        std::map<DataType::TYPE, void*>::iterator it = data_map.begin();
        for(it; it != data_map.end(); ++it)
        {
            void* data = 0;
            unsigned int size = 0;
            switch(it->first)
            {
            #define X(NAME, TYPE, COUNT) \
            case DataType::NAME: \
                data = (void*)(((std::vector<NAME>*)(it->second))->data()); \
                size = ((std::vector<NAME>*)(it->second))->size() * sizeof(NAME); \
                break;
            XFIELDS
            #undef X
            }

            blocks.push_back(Block(it->first, data, size));
        }

        blocks.push_back(Block(DataType::R3DEND, 0, 0));

        Block block;
        do
        {
            block = blocks.front();
            unsigned int type = block.Type();
            unsigned int size = block.Size();
            file.write((char*)&type, sizeof(type));
            file.write((char*)&size, sizeof(size));
            if(block.Size() > 0)
            {
                file.write((char*)block.Data(), block.Size());
            }
            blocks.erase(blocks.begin());
        }while(block.Type() != DataType::R3DEND);
        

        file.close();
    }

    void Read(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if(file.read(buffer.data(), size))
        {
            Read(buffer.data(), size);
        }


        file.close();
    }

    void Read(const char* data, size_t size)
    {
        std::vector<Block> blocks;
        unsigned int type = 0;
        unsigned int block_sz = 0;
        unsigned int offset = 0;
        do
        {
            type = *(unsigned int*)(data + offset);
            offset += sizeof(type);
            block_sz = *(unsigned int*)(data + offset);
            offset += sizeof(block_sz);
            if(block_sz > 0)
                blocks.push_back(Block((DataType::TYPE)type, (void*)(data + offset), (size_t)block_sz));
            offset += block_sz;

        }while(type != DataType::R3DEND);

        for(unsigned int i = 0; i < blocks.size(); ++i)
        {
            switch(blocks[i].Type())
            {
            #define X(NAME, TYPE, COUNT) \
            case DataType::NAME: \
            { \
                NAME* data_ptr = (NAME*)blocks[i].Data(); \
                unsigned int length = blocks[i].Size() / sizeof(NAME); \
                std::vector<NAME> data_vec(data_ptr, data_ptr + length); \
                Set(data_vec); \
                break; \
            }
            XFIELDS
            #undef X
            }
        }
    }
private:
    std::map<DataType::TYPE, void*> data_map;
};

template<typename T>
std::vector<T> R3DIO::Get()
{
    if(data_map.find(T::Type()) == data_map.end())
        return std::vector<T>();
    return *((std::vector<T>*)(data_map[T::Type()]));
}

template<typename T>
void R3DIO::Set(const std::vector<T>& data)
{
    std::vector<T>* d = new std::vector<T>();
    *d = data;
    data_map.insert(std::make_pair(T::Type(), (void*)d));
}

#endif