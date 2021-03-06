#include "gfxs_stage.h" 

#include "gfxs_atom.h"

namespace GFXS
{
    Slot& Stage::operator[](const std::string& name)
    {
        std::map<std::string, Slot>::iterator it;
        it = slots.find(name);
        if (it == slots.end())
        {
            slots.insert(std::make_pair(name, Slot()));
            it = slots.find(name);
        }
        
        return it->second;
    }

    void Stage::Link(Stage& next_stage)
    {
        std::map<std::string, Slot>::iterator it;
        for (it = next_stage.slots.begin(); it != next_stage.slots.end(); ++it)
        {
            it->second.Link(*this);
        }
    }

    void Stage::AddUniform(GFXS::Uniform* uni)
    {
        globals.push_back(uni->NewGlobalPtr());
        global_names.push_back(uni->ReferenceName());
    }

    void Stage::AddAttrib(GFXS::Input* attrib)
    {
        attrib_names.push_back(attrib->ReferenceName());
    }

}