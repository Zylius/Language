#include "ADT.h"
#include "Parser.h"
namespace Four20
{
	void Procedure::add(Obj* obj) {
		for (int i = 0; i < locals.size(); i++) {
			if (coco_string_equal(locals[i]->name, name)) parser->SemErr(L"name declared twice");
		}
		locals.push_back(obj);
		if (obj->kind == Type::VAR) {
			((Var*)obj)->adr = nextAdr++;
		}
	}

	Obj* Procedure::find(wchar_t* name) {
		for (int i = 0; i < locals.size(); i++) {
			if (coco_string_equal(locals[i]->name, name)) return locals[i];
		}
		if (program != NULL) {
			for (Obj* o : program->locals) {
				if (coco_string_equal(o->name, name))
					return o;
			}
		}
		parser->SemErr(L" undeclared");
		return new Obj(L"_undef", 0); // dummy
	}
}