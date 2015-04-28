#include "ADT.h"
#include "Parser.h"
namespace Four20
{
	int indent = 0;
	const char * getTextForOperator(int enumVal)
	{
		return OperatorStrings[enumVal];
	}

	const char * getTextForType(int enumVal)
	{
		return TypeStrings[enumVal];
	}

	void addLevels() {
		for (int i = 0; i < indent; i++) printf("---");
	}

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

	void Procedure::dump() {
		printf("PROC: %S\n", name);
		indent++;

		if (this->program == NULL) {
			addLevels(); printf("DEFINITIOS: \n");
			indent++;
			for each (Obj* var in definitions)
			{
				var->dump();
			}
			indent--;
		}


		addLevels(); printf("STATEMENTS: \n");
		indent++;
		for each (Stat* var in statements)
		{
			var->dump();
		}
		indent--;

		addLevels(); printf("VARIABLES: \n");
		indent++;
		for each (Procedure* obj in locals)
		{
			addLevels(); obj->dump();
		}

		indent-=2;

	}
}