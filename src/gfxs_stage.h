#ifndef _GFXS_STAGE_H_
#define _GFXS_STAGE_H_

#include <vector>
#include <set>
#include <map>
#include <string>

#include "gfxs_slot.h"

namespace GFXS
{
    class Atom;
    class Stage
    {
    public:
        Slot& operator[](const std::string& name);

        void Link(Stage& next_stage);

        void AddGlobalLine(std::string line) { global_lines.insert(line); }

        std::string Evaluate()
        {
            std::string source;
            std::map<std::string, Slot>::iterator it;

            std::string statements;
            for (it = slots.begin(); it != slots.end(); ++it)
            { 
                statements += "\t" + it->first + " = " + it->second.EvaluateOutput(this, it->first) + ";\n";
            }

            source += JoinStrings(global_lines, std::string("\n"));
            source += "\nvoid main()\n{\n";
            source += statements;
            source += "}\n";

            return source;
        }
    protected:
        std::set<std::string> global_lines;
        std::map<std::string, Slot> slots; // Outputs of a shader stage

        std::string JoinStrings(std::set<std::string>& strings, std::string& delim)
        {
            std::string result;
            std::set<std::string>::iterator it = strings.begin();
            for (it; it != strings.end(); ++it)
            {
                result += *it;
                result += delim;
            }
            return result;
        }
    };
}

#endif