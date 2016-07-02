#include "gfxs_atom.h"

#define ATOM_MAP(...) \
    std::map<std::string, Atom*> Atom::name_to_atom = \
    { \
        FOR_EACH_ARG(ATOM_, __VA_ARGS__) \
        { "#None", 0 } \
    }

#define ATOM_(ID, ARG) { #ARG, new ARG },
    

namespace GFXS
{
    ATOM_MAP
    (
        Texture2D,
        MatrixModel,
        MatrixView,
        MatrixPerspective,

        Position,
        Normal,
        UV,

        Position3D,
        TextureColor2D
    );

    Atom* Atom::GetPtr(std::string name)
    {
        return name_to_atom[name];
    }
}