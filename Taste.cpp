#include "Stdio.h"

#include "SymbolTable.h"

#include "Parser.h"
#include "Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>

using namespace Four20;

int main (int argc, char *argv[]) {

	if (argc == 2) {
		wchar_t *fileName = coco_string_create(argv[1]);
		Four20::Scanner *scanner = new Four20::Scanner(fileName);
		Four20::Parser *parser = new Four20::Parser(scanner);
		parser->tab = new Four20::SymbolTable(parser);
		parser->gen = new Four20::CodeGenerator();
		parser->Parse();
		if (parser->errors->count == 0) {
			parser->gen->Decode();
			parser->gen->Interpret("Taste.IN");
		}

		coco_string_delete(fileName);
		delete parser->gen;
		delete parser->tab;
		delete parser;
		delete scanner;
	} else
		printf("-- No source file specified\n");

	return 0;

}
