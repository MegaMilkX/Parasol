#ifndef _GFXS_ATOM_H_
#define _GFXS_ATOM_H_

#include "macro/macro_narg.h"
#include "macro/macro_typed_expr.h"
#include "macro/macro_for_each_arg.h"

#include "gfxs_stage.h"

#define DEF_SHADER_UNIFORM(TYPE, C_TYPE, NAME) \
    class NAME : public Uniform \
    { \
    public: \
        NAME(unsigned int id = 0) : input_id(id), global(0) {} \
        ~NAME() { delete global; } \
        NAME* Clone() const { return new NAME(*this); } \
        TYPE* CloneTypeInstance() const { return new TYPE(); } \
        std::string Evaluate(Stage* stage) \
        { \
            std::string eval; \
            eval += ReferenceName(); \
            stage->AddGlobalLine(Declaration() + ";"); \
            stage->AddUniform(this); \
            return eval; \
        } \
        std::string Declaration() \
        { \
            return std::string("uniform ") + TYPE().InternalName() + " " + ReferenceName(); \
        } \
        IGFXGlobal* NewGlobalPtr() \
        { \
            return new GFXGlobal<C_TYPE>(GFXGlobal<C_TYPE>::Get(ReferenceName())); \
        } \
        void SetParameter(std::string name, std::string value) \
        { \
            if(name == "id") \
            { \
                try \
                { input_id = std::stoi(value); } \
                catch (...) {} \
            } \
        } \
    protected: \
        std::string OriginalRefName() \
        { \
            return std::string(#NAME) + std::to_string(input_id); \
        } \
        unsigned int input_id; \
        IGFXGlobal* global; \
    }

#define DEF_SHADER_INPUT(TYPE, NAME) \
    class NAME : public Input \
    { \
    public: \
        NAME(unsigned int id = 0) : input_id(id) {} \
        NAME* Clone() const { return new NAME(*this); } \
        TYPE* CloneTypeInstance() const { return new TYPE(); } \
        std::string Evaluate(Stage* stage) \
        { \
            std::string eval; \
            eval += ReferenceName(); \
            stage->AddGlobalLine(Declaration() + ";"); \
            stage->AddAttrib(this); \
            return eval; \
        } \
        void Link(Stage& stage) \
        { \
            std::string new_output_name = ReferenceName() + "_linked"; \
            stage[new_output_name] = *this; \
            Rename(new_output_name); \
        } \
        std::string Declaration() \
        { \
            return std::string("in ") + TYPE().InternalName() + " " + ReferenceName(); \
        } \
        void SetParameter(std::string name, std::string value) \
        { \
            if(name == "id") \
            { \
                try \
                { input_id = std::stoi(value); } \
                catch (...) {} \
            } \
        } \
    protected: \
        std::string OriginalRefName() \
        { \
            return std::string(#NAME) + std::to_string(input_id); \
        } \
        unsigned int input_id; \
    }

#define DEF_SHADER_ATOM_VERTEX(TYPE, NAME, SOURCE, INPUTS, UNIFORMS, ARGS) \
    DEF_SHADER_ATOM(TYPE, NAME, VertexAtom, SOURCE, INPUTS, UNIFORMS, ARGS)

#define DEF_SHADER_ATOM_PIXEL(TYPE, NAME, SOURCE, INPUTS, UNIFORMS, ARGS) \
    DEF_SHADER_ATOM(TYPE, NAME, PixelAtom, SOURCE, INPUTS, UNIFORMS, ARGS)

#define SHADER_ATOM_BODY(TYPE, NAME, SOURCE) \
public: \
	NAME* Clone() const { return new NAME(*this); } \
	TYPE* CloneTypeInstance() const { return new TYPE(); } \
	std::string Evaluate(Stage* stage) \
    { \
        std::string eval; \
        eval += ReferenceName(); \
        eval += "("; \
        for(int i = 0; i < ArgumentCount(); ++i) \
        { \
            if (i != 0) eval += ", "; \
            eval += ArgumentSlot(i)->Evaluate(stage); \
        } \
        eval += ")"; \
        stage->AddGlobalLine(Declaration()); \
        EvalUniforms(stage); \
        EvalInputs(stage); \
        return eval; \
    } \
	void Link(Stage& stage) \
    { \
        for(int i = 0; i < ArgumentCount(); ++i) \
        { \
            ArgumentSlot(i)->Link(stage); \
        } \
    } \
    std::string Declaration() \
    { \
        std::string func; \
        func = TYPE().InternalName() + " " + ReferenceName() + ArgumentListDecl() + "{" + SOURCE + "}"; \
        return func; \
    } \
protected: \
    std::string OriginalRefName() \
    { \
        return #NAME; \
    }

#define DEF_SHADER_ATOM(TYPE, NAME, BASE_ATOM, SOURCE, INPUTS, UNIFORMS, ARGS) \
    class NAME : public BASE_ATOM \
    { \
    public: \
        NAME* Clone() const { return new NAME(*this); } \
        TYPE* CloneTypeInstance() const { return new TYPE(); } \
        std::string Evaluate(Stage* stage) \
        { \
            std::string eval; \
            eval += ReferenceName(); \
            eval += "("; \
            for(int i = 0; i < ArgumentCount(); ++i) \
            { \
                if (i != 0) eval += ", "; \
                eval += ArgumentSlot(i)->Evaluate(stage); \
            } \
            eval += ")"; \
            stage->AddGlobalLine(Declaration()); \
            EvalUniforms(stage); \
            EvalInputs(stage); \
            return eval; \
        } \
        void Link(Stage& stage) \
        { \
            for(int i = 0; i < ArgumentCount(); ++i) \
            { \
                ArgumentSlot(i)->Link(stage); \
            } \
        } \
        std::string Declaration() \
        { \
            std::string func; \
            func = TYPE().InternalName() + " " + ReferenceName() + ArgumentListDecl() + "{" + SOURCE + "}"; \
            return func; \
        } \
        ARGS \
        UNIFORMS \
        INPUTS \
    protected: \
        std::string OriginalRefName() \
        { \
            return #NAME; \
        } \
    }

#define DEF_ATOM_ARGS(...) \
	public: \
    FOR_EACH_ARG(ARG_SLOT_DEF, __VA_ARGS__) \
    int ArgumentCount() const { return PP_NARGS(__VA_ARGS__); } \
    Slot* ArgumentSlot(int id) \
    { \
        Slot* slot = 0; \
        id = ArgumentCount() - id - 1; \
        switch(id) \
        { \
        FOR_EACH_ARG(ARG_SLOT_CASE, __VA_ARGS__) \
        } \
        return slot; \
    } \
    std::string ArgumentListDecl() \
    { \
        std::string args = "("; \
        std::string temp; \
        FOR_EACH_ARG(ARG_SLOT_NAME, __VA_ARGS__) \
        args.erase(args.find_last_of(", ") - 1); \
        args += ")"; \
        return args; \
    }

#define ARG_SLOT_DEF(ID, ARG) TypedSlot<TYPEOF(ARG)> STRIP(ARG);
#define ARG_SLOT_CASE(ID, ARG) case ID: slot = & STRIP(ARG); break;
#define ARG_SLOT_NAME(ID, ARG) \
    temp = #ARG; \
    temp.erase(temp.begin() + temp.find_first_of("("), temp.begin() + temp.find_last_of(")") + 1); \
    args += "in " + TYPEOF(ARG)().InternalName() + " " + temp + ", ";

#define DEF_ATOM_INPUTS(...) \
    void EvalInputs(Stage* stage) \
    { \
        FOR_EACH_ARG(ADD_INPUT, __VA_ARGS__) \
    }

#define DEF_ATOM_UNIFORMS(...) \
    void EvalUniforms(Stage* stage) \
    { \
        FOR_EACH_ARG(ADD_UNIFORM, __VA_ARGS__) \
    }

#define ADD_INPUT(ID, ARG) stage->AddGlobalLine(ARG.Declaration() + ";"); \
    stage->AddAttrib(& ARG);
#define ADD_UNIFORM(ID, ARG) stage->AddGlobalLine(ARG.Declaration() + ";"); \
    stage->AddUniform(& ARG);

#define NO_INPUTS(...)
#define NO_UNIFORMS(...)
#define NO_ARGS(...)

namespace GFXS
{
    class Atom
    {
    public:
        static Atom* GetPtr(std::string name);

        Atom() : ref_name(""){}
        virtual ~Atom() {}
        virtual Atom* Clone() const = 0;
        virtual Type* CloneTypeInstance() const = 0;
        virtual std::string Declaration() = 0;
        virtual std::string Evaluate(Stage* stage) = 0;
        
        virtual int ArgumentCount() const { return 0; }
        virtual Slot* ArgumentSlot(int id) { return 0; }

        virtual std::string ArgumentListDecl() { return "()"; }

        virtual void EvalUniforms(Stage* stage){}
        virtual void EvalInputs(Stage* stage){}

        virtual void Link(Stage& stage){ }

        virtual void SetParameter(std::string name, std::string value) {}

        virtual void Rename(const std::string& name)
        {
            ref_name = name;
        }
        std::string ReferenceName()
        {
            if (ref_name.size() > 0)
                return ref_name;
            else
                return OriginalRefName();
        }
    protected:
        virtual std::string OriginalRefName() = 0;

        std::string ref_name;

        static std::map<std::string, Atom*> name_to_atom;
    };

    class Uniform : public Atom
    {
    public:
        virtual IGFXGlobal* NewGlobalPtr() = 0;
        
    };

    class Input : public Atom
    {

    };

    class VertexAtom : public Atom {};
    class PixelAtom : public Atom {};

    // =======================
    // Uniforms
    // =======================

    DEF_SHADER_UNIFORM(Sampler2D, int, Texture2D);
    DEF_SHADER_UNIFORM(Mat4, mat4f, MatrixModel);
    DEF_SHADER_UNIFORM(Mat4, mat4f, MatrixView);
    DEF_SHADER_UNIFORM(Mat4, mat4f, MatrixPerspective);

	// =======================
	// Inputs
	// =======================

	DEF_SHADER_INPUT(Vec3, Position);
	DEF_SHADER_INPUT(Vec4, RGBA);
	DEF_SHADER_INPUT(Vec3, Normal);
	DEF_SHADER_INPUT(Vec2, UV);

    // =======================
    // Functions
    // =======================

	class MultiplyVec4Vec4 : public Atom
	{
	public:
		MultiplyVec4Vec4() {}
		MultiplyVec4Vec4(Atom& a, Atom& b)
		{
			this->a = a;
			this->b = b;
		}

		SHADER_ATOM_BODY(Vec4, MultiplyVec4Vec4, "return a * b;")
		DEF_ATOM_ARGS((Vec4)a, (Vec4)b)
	};

	class TextureColor2D : public PixelAtom
	{
	public:
		TextureColor2D() {}
		TextureColor2D(Texture2D& sampler, Atom& uv)
		{
			texture_sampler = sampler;
			this->uv = uv;
		}

		SHADER_ATOM_BODY(Vec4, TextureColor2D, "return texture2D(texture_sampler, uv);")
		DEF_ATOM_ARGS((Sampler2D)texture_sampler, (Vec2)uv)
	};

	class TextureColor2DRedAlpha : public PixelAtom
	{
	public:
		TextureColor2DRedAlpha() {}
		TextureColor2DRedAlpha(Texture2D& sampler, Atom& uv)
		{
			texture_sampler = sampler;
			this->uv = uv;
		}

		SHADER_ATOM_BODY(Vec4, TextureColor2DRedAlpha, "vec4 c = texture2D(texture_sampler, uv); return vec4(1.0, 1.0, 1.0, c.r);")
		DEF_ATOM_ARGS((Sampler2D)texture_sampler, (Vec2)uv)
	};
	/*
    DEF_SHADER_ATOM_PIXEL(
        Vec4,
        TextureColor2D,
        "return texture2D(texture_sampler, uv);",
        NO_INPUTS(),
        NO_UNIFORMS(),
        DEF_ATOM_ARGS((Sampler2D)texture_sampler, (Vec2)uv)
    );*/
    DEF_SHADER_ATOM_PIXEL(
        Vec4,
        UV_TO_RGBA,
        "return vec4(uv.x, uv.y, 0.0, 1.0);",
        NO_INPUTS(),
        NO_UNIFORMS(),
        DEF_ATOM_ARGS((Vec2)uv)
    );
    DEF_SHADER_ATOM_PIXEL(
        Vec4,
        Texture2DScreen,
        "return texture2D(texture_sampler, gl_FragCoord.st * 100.0);",
        NO_INPUTS(),
        NO_UNIFORMS(),
        DEF_ATOM_ARGS((Sampler2D)texture_sampler, (Float)scale)
    );
    DEF_SHADER_ATOM_PIXEL(
        Vec4,
        Multiply3f,
        "return vec4(a, 1.0);",
        NO_INPUTS(),
        NO_UNIFORMS(),
        DEF_ATOM_ARGS((Vec3)a, (Vec3)b)
    );
    DEF_SHADER_ATOM_VERTEX(
        Vec4,
        Position3D,
        "return MatrixPerspective0 * MatrixView0 * MatrixModel0 * vec4(pos, 1.0);",
        NO_INPUTS(),
        DEF_ATOM_UNIFORMS(MatrixModel(), MatrixView(), MatrixPerspective()),
        DEF_ATOM_ARGS((Vec3)pos)
    );

    

    
}

#endif