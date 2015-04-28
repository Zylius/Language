#if !defined(TASTE_ADT_H__)
#define TASTE_ADT_H__

#include "Scanner.h"
#include <vector>
using namespace std;
namespace Four20 {
	extern int indent;

	void addLevels();

	class Parser;
	class Stat;

	enum Type { UNDEF, INT, BOOL, CHAR, STRING, VAR, DEF };
	static const char * TypeStrings[] = { "UNDEF", "INT", "BOOL", "CHAR", "STRING", "VAR", "DEF"};
	const char * getTextForType(int enumVal);

	enum Operator { EQU, LSS, GTR, ADD, SUB, MUL, DIV, NEQ };
	static const char * OperatorStrings[] = { "EQU", "LSS", "GTR", "ADD", "SUB", "MUL", "DIV", "NEQ" };
	const char * getTextForOperator(int enumVal);

	class Node
	{
	public:
		virtual void dump() {};
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
		void dump() { printf("%s:%S\n", getTextForType(this->type), this->name); };
	};

	class Procedure : public Obj { // procedure (also used for the main program)
	public:
		vector<Obj*> locals;      // local objects declared in this procedures
		vector<Stat*> statements;      // local objects declared in this procedures
		vector<Obj*> definitions;      // local objects declared in this procedures
		int nextAdr;	         // next free address in this procedure
		Procedure* program = NULL;	         // link to the Proc node of the main program or null
		Parser* parser;         // for error messages

		Procedure(wchar_t* name, int type, Procedure* program, Parser* parser) : Obj(name, type) {
			this->program = program;
			this->parser = parser;
		}

		void add(Obj* obj);

		void addStat(Stat* statement)
		{
			statements.push_back(statement);
		}

		void addDefinition(Obj* obj)
		{
			definitions.push_back(obj);
		}

		Obj* find(wchar_t* name);

		void dump();
	};

	// --- EXPRESIONS

	class Expr : public Node  {};

	class BinExpr : public Expr {
	public:
		Operator op;
		Expr* left, *right;
		BinExpr(Expr* e1, Operator o, Expr* e2) { op = o; left = e1; right = e2; }
		void dump() { left->dump(); printf(" BINEXPR: %s \n", getTextForOperator(op)); indent++; right->dump();  }
	};

	class Ident : public Expr {
	public:
		Obj* obj;
		Ident(Obj* o) { obj = o; }
		void dump() { addLevels(); printf("IDENT: %S", obj->name); }
	};

	class VarCon : public Expr {
	public:
		Var* val;
		VarCon(Var* x) { val = x; }
		void dump() { addLevels();  printf("VAR: %S", val->name); }
	};

	class IntCon : public Expr {
	public:
		wchar_t* val;
		IntCon(wchar_t* x) { val = x; }
		void dump() { addLevels();  printf("INT: %S", val); }
	};

	class CharCon : public Expr {
	public:
		wchar_t* val;
		CharCon(wchar_t* x) { val = x; }
		void dump() { addLevels();  printf("CHAR: %S", val); }
	};

	class StringCon : public Expr {
	public:
		wchar_t* val;
		StringCon(wchar_t* x) { val = x; }
		void dump() { addLevels();  printf("STRING: %S", val); }
	};

	class BoolCon : public Expr {
	public:
		bool val;
		BoolCon(bool x) { val = x; }
		void dump() { addLevels();  printf("BOOL: %i", val); }
	};

	class DefineCon : public Expr {
	public:
		wchar_t* val;
		DefineCon(wchar_t* x) { val = x; }
		void dump() { addLevels();  printf("DEFINE: %S", val); }
	};

	//------------- Statements -----------------------------
	class Stat : public Node {
	public:

		virtual void dump() {
			addLevels();
		};
	};

	class Assignment : public  Stat {
	public:
		Obj*  left;
		Expr* right;
		Assignment(Obj* o, Expr* e)
		{
			left = o; right = e;
		}
		void dump() { 
			Stat::dump(); 
			printf("ASSIGN %S:\n", left->name);
			int oldIndent = indent;
			indent++;
			right->dump();
			indent = oldIndent;
			printf("\n"); 
		}
	};

	class Call : public Stat {
	public:
		Obj* proc;
		vector<Obj*> parameters;
		Call(Obj* o) { proc = o; }
		void dump() {
			Stat::dump(); printf("CALL:\n");
			indent++;
			Stat::dump(); printf("NAME: %S\n", proc->name);
			Stat::dump(); printf("PARAMETERS:\n");
			indent++;
			for each (Obj* var in parameters)
			{
				Stat::dump(); var->dump();
			}
			indent-=2;
		}
	};

	class If : public Stat {
	public:
		Expr* cond;
		Stat* stat;
		If(Expr* e, Stat* s) { cond = e; stat = s; }
		void dump() {
			addLevels();
			printf("if\n");
			indent++;
			addLevels(); printf("CONDITION:\n");
			indent++;
			int oldIndent = indent;
			cond->dump();
			indent = oldIndent;
			indent--;
			printf("\n");  addLevels(); printf("BODY: \n");
			indent++; stat->dump();
			indent--; 
			indent--;
		}
	};

	class IfElse: public Stat {
	public:
		Stat* ifPart;
		Stat* elsePart;
		IfElse(Stat* i, Stat* e) { ifPart = i; elsePart = e; }
		void dump() { ifPart->dump(); addLevels(); printf("else \n");  indent++; elsePart->dump();  indent--; }
	};

	class For : public Stat {
	public:
		Assignment* start;
		Expr* toExpr;
		Stat* endStat;
		Stat* doStat;
		For(Assignment* start, Expr* toExpr, Stat* endStat, Stat* doStat) { this->start = start; this->toExpr = toExpr; this->endStat = endStat; this->doStat = doStat;}
		void dump() {
			addLevels();
			printf("for\n");
			indent++;

			addLevels(); printf("START ASSIGNMENT:\n");
			int oldIndent = indent;
			indent++;
			start->dump();
			indent = oldIndent;

			addLevels(); printf("TO CONDITION:\n");
			oldIndent = indent;
			indent++;
			toExpr->dump();
			indent = oldIndent;

			printf("\n"); addLevels(); printf("STEP:\n");
			oldIndent = indent;
			indent++;
			endStat->dump();
			indent = oldIndent;

			addLevels(); printf("BODY:\n");
			oldIndent = indent;
			indent++;
			doStat->dump();
			indent = oldIndent;

			indent--;
		}
	};

	class Write : public Stat {
	public:
		Expr* e;
		Write(Expr* x) { e = x; }
		void dump() { 
			Stat::dump(); printf("WRITE:\n"); 
			indent++; Stat::dump(); printf("EXPRESSION: \n"); indent++; e->dump(); indent-=2;
			printf("\n");
		}
	};
};
#endif
