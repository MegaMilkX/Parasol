#include "gfxs_atom.h"

#include "gfxs_slot.h"

namespace GFXS
{
    Slot::Slot() : type(0), atom(0), declare_output(true)
    {
        
    }

    Slot::Slot(const Slot& other) : type(0), atom(0)
    {
        if (type) delete type;
        if (atom) delete atom;
        if (other.type) type = other.type->Clone();
        if (other.atom) atom = other.atom->Clone();
        declare_output = other.declare_output;
    }

    Slot::~Slot()
    {
        if (type)
            delete type;
        if (atom)
            delete atom;
    }

    Slot& Slot::operator=(const Atom& atom)
    {
        if (this->atom)
            delete this->atom;
        this->atom = atom.Clone();
        if (type)
            delete type;
        type = atom.CloneTypeInstance();
        return *this;
    }

    Slot& Slot::operator=(const Slot& other)
    {
        Slot::Slot(other);
        return *this;
    }

    void Slot::HideOutputDeclaration(bool value)
    {
        declare_output = !value;
    }

    std::string Slot::EvaluateOutput(Stage* stage, const std::string& name)
    {
        if (declare_output) stage->AddGlobalLine(std::string("out ") + type->InternalName() + " " + name + ";");

        if (atom)
            return atom->Evaluate(stage);
        else if (type)
            return type->InitialValue();
        else
            return "NO_TYPE_OR_ATOM";
    }

    std::string Slot::Evaluate(Stage* stage)
    {
        if (atom)
            return atom->Evaluate(stage);
        else if (type)
            return type->InitialValue();
        else
            return "NO_TYPE_OR_ATOM";
    }

    void Slot::Link(Stage& stage)
    {
        if (atom)
            atom->Link(stage);
    }
}