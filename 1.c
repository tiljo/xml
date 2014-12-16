#include<stdio.h>
#include<libxml/parser.h>
#include<libxml/tree.h>


static void myParser(const char* filename){
	xmlDocPtr doc;

	doc = xmlReadFile(filename,NULL,0);
	if(doc==NULL){
		fprintf(stderr, "Failed to parse %s\n",filename);
		return;
	}
	xmlFreeDoc(doc);
}

int main(int argc, char** argv){
	if(argc!=2){
		printf("give filename\n");
		return(1);
	}

	LIBXML_TEST_VERSION

	myParser(argv[1]);

	xmlCleanupParser();

	xmlMemoryDump();
	return(0);

}
