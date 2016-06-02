#include "vertex_attrib.h"

std::map<int, int> type_to_gltype =
{ { TypeInfo<char>::GetId(), GL_BYTE },
{ TypeInfo<unsigned char>::GetId(), GL_UNSIGNED_BYTE },
{ TypeInfo<short>::GetId(), GL_SHORT },
{ TypeInfo<unsigned short>::GetId(), GL_UNSIGNED_SHORT },
{ TypeInfo<int>::GetId(), GL_INT },
{ TypeInfo<unsigned int>::GetId(), GL_UNSIGNED_INT },
{ TypeInfo<float>::GetId(), GL_FLOAT },
{ TypeInfo<double>::GetId(), GL_DOUBLE } };