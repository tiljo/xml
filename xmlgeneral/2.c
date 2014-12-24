#include<stdio.h>
#include<libxml/parser.h>
#include<libxml/tree.h>

#ifdef LIBXML_TREE_ENABLED

static void print_elements(xmlNode* a_node)
{
	xmlNode* current = NULL;

	for(current = a_node; current; current = current->next){
		if((current->type == XML_TEXT_NODE) && !(xmlIsBlankNode(current))){
			printf("node type: Element, name: %s\n",current->parent->name);
			printf("content is : %s\n",current->content);
		}
		print_elements(current->children);
	}
	
}
int main(int argc, char**argv)
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	xmlNodePtr n;
	xmlChar* xmlbuff;
	int buffersize;

	if(argc!=2){
		printf("give filename\n");
		return(1);
	}

	LIBXML_TEST_VERSION

	doc = xmlReadFile(argv[1],NULL,0);
	if(doc==NULL){
		printf("could not parse file %s\n",argv[1]);
	}
	xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
//	printf("%s",(char*)xmlbuff);


	xmlFree(xmlbuff);

	root_element = xmlDocGetRootElement(doc);

	print_elements(root_element);

	xmlFreeDoc(doc);

	xmlCleanupParser();

	return 0;
}
#else
int main()
{
	fprintf(stderr,"Tree support not compiled in \n");
	exit(1);
}
#endif



