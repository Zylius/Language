#if !defined(TASTE_ADT_H__)
#define TASTE_ADT_H__

#include "Scanner.h"
#include <vector>
using namespace std;
namespace Four20 {
	class Parser;
	enum Type { UNDEF, INT, BOOL, CHAR, STRING, VAR };
	enum Operator { EQU, LSS, GTR, ADD, SUB, MUL, DIV, NEQ };
	class Node
	{
	public:
		void dump() {};
	};

	class Obj : public Node {  // object describing a declared name
	public:
		wchar_t* name;		// name of the object
		int type;		// type of the object (undef for proc)
		int kind;
		Obj(wchar_t* name, int type)
		{
			this->name = name;
			this->type = type;
		}

		Obj() {
			name = NULL;
			type = 0;
		}

		~Obj() {
			coco_string_delete(name);
		}
	};

	class Var : public Obj {
	public:
		Var(wchar_t* name, Type type) {
			this->name = name;
			this->type = type;
		}
		int adr; 
	};

	class Procedure : public Obj { // procedure (also used for the main program)
	public:
		vector<Obj*> locals;      // local objects declared in this procedures
		int nextAdr;	         // next free address in this procedure
		Procedure* program = NULL;	         // link to the Proc node of the main program or null
		Parser* parser;         // for error messages

		Procedure(wchar_t* name, int type, Procedure* program, Parser* parser) : Obj(name, type) {
			this->program = program;
			this->parser = parser;
		}

		void add(Obj* obj);

		Obj* find(wchar_t* name);

		void dump() {
			printf("Proc %s", name); 
			printf("\n", name);
		}
	};

	// --- EXPRESIONS

	class Expr : public Node  {};

	class BinExpr : public Expr {
	public:
		Operator op;
		Expr* left, *right;
		BinExpr(Expr* e1, Operator o, Expr* e2) { op = o; left = e1; right = e2; }
		void dump() { left->dump(); printf(" %i ",  op); right->dump(); }
	};

	class Ident : public Expr {
	public:
		Obj* obj;
		Ident(Obj* o) { obj = o; }
		void dump() { printf("%s", obj->name); }
	};

	class IntCon : public Expr {
	public:
		int val;
		IntCon(int x) { val = x; }
		void dump() { printf("%i", val); }
	};

	class BoolCon : public Expr {
	public:
		bool val;
		BoolCon(bool x) { val = x; }
		void dump() { printf("%i", val); }
	};

	//------------- Statements -----------------------------
	class Stat : public Node {
	public:
		static int indent;
		void dump() { for (int i = 0; i < indent; i++) printf("  "); }
	};

	class Assignment : public  Stat {
	public:
		Obj*  left;
		Expr* right;
		Assignment(Obj* o, Expr* e) { left = o; right = e; }
		void dump() { Stat::dump(); printf("%s %s", left->name, " = "); Stat::dump(); printf("\n"); }
	};

	class Call : public Stat {
	public:
		Obj* proc;
		Call(Obj* o) { proc = o; }
		void dump() { Stat::dump(); printf("call %s", proc->name); }
	};

	class If : public Stat {
	public:
		Expr cond;
		Stat stat;
		If(Expr e, Stat s) { cond = e; stat = s; }
		void dump() { Stat::dump(); printf("if "); cond.dump(); printf("\n"); Stat::indent++; stat.dump(); Stat::indent--; }
	};

	class IfElse: public Stat {
	public:
		Stat* ifPart;
		Stat* elsePart;
		IfElse(Stat* i, Stat* e) { ifPart = i; elsePart = e; }
		void dump() { ifPart->dump(); Stat::dump(); printf("else ");  Stat::indent++; elsePart->dump();  Stat::indent--; }
	};

	class For : public Stat {
	public:
		Expr* cond;
		Expr* code;
		Assignment* start;
		Assignment* end;
		Stat* stat;
		For(Expr* e, Stat* s) { cond = e; stat = s; }
		void dump() { Stat::dump(); printf("for "); start->dump(); printf(" till "); cond->dump(); printf(" do");  end->dump(); code->dump(); printf("\n"); Stat::indent++; Stat::dump(); Stat::indent--; }
	};

	class Write : public Stat {
	public:
		Expr* e;
		Write(Expr* x) { e = x; }
		void dump() { Stat::dump(); printf("write "); e->dump(); printf("\n"); }
	};
};
#endif
