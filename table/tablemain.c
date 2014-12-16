#include"table.h"
#include"tablefun.c"
#ifdef LIBXML_READER_ENABLED
int main(int argc, char **argv)
{
	if(argc!=2){
		printf("give file name\n");
		return (1);
	}
	struct contact *result[10];
	FILE *fp;
	char* filename;

	filename = "Contacts.csv";
	fp = fopen(filename,"w+");
	fprintf(fp,"Name,mobileNumber\n");

	LIBXML_TEST_VERSION

	streamFile(argv[1],result,&fp);
	fclose(fp);
	xmlCleanupParser();
	xmlMemoryDump();
	return(0);
}
#else
int main()
{
	fprintf(stderr,"XInclude support not compiled  in\n");
	exit(1);
}
#endif


