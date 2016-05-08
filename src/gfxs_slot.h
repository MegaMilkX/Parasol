#ifndef _GFXS_SLOT_H_
#define _GFXS_SLOT_H_

#include <set>

#include "gfxs_type.h"

namespace GFXS
{
    class Atom;
    class Stage;
    class Type;
    class Slot
    {
    public:
        Slot();
        Slot(const Slot& other);
        ~Slot();
        virtual Slot& operator=(const Atom& atom);
        virtual Slot& operator=(const Slot& other);
        void HideOutputDeclaration(bool value);
        std::string EvaluateOutput(Stage* stage, const std::string& name);
        std::string Evaluate(Stage* stage);
        void Link(Stage& stage);
    protected:
        void SetType(const Type& type);
        Type* type;
        Atom* atom;
        bool declare_output;
    };

    template<typename TYPE>
    class TypedSlot : public Slot
    {
    public:
        TypedSlot()
        {
            type = new TYPE();
        }
        TypedSlot& operator=(const Atom& atom)
        {
            if (this->atom)
                delete this->atom;
            this->atom = atom.Clone();
            return *this;
        }
    };
}

#endif