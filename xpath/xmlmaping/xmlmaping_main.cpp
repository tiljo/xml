#include"xmlmaping_header.h"
#include"xmlmaping_fun.cpp"



#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED)&& \
	defined(LIBXML_OUTPUT_ENABLED)

int main(int argc, char** argv)
{
	if(argc !=3 ){
		printf("wrong no of argv..\n");
		usage(argv[0]);
		return(-1);
	}
	xmlInitParser();
	LIBXML_TEST_VERSION

	xmlParse abc;

	if(parser(argv[1], BAD_CAST argv[2],abc)<0){
		printf("error...\n");
		return(-1);
	}

	abc.onpath();

	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}

#else
int main()
{
	fprintf(stderr, "Xpath support not compiled in \n");
	exit(1);
}
#endif
