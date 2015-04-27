#include "Stdio.h"


#include "Parser.h"
#include "Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>


using namespace Four20;
int Stat::indent = 0;

int main(int argc, char *argv[]) {

	if (argc == 2) {
		wchar_t *fileName = coco_string_create(argv[1]);
		Scanner *scanner = new Scanner(fileName);
		Parser *parser = new Parser(scanner);
		parser->Parse();
		if (parser->errors->count == 0) {
			printf("success");
		}

		coco_string_delete(fileName);

		delete parser;
		delete scanner;
	}
	else
		printf("-- No source file specified\n");

	return 0;

}
