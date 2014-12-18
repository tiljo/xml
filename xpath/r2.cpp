#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include<libxml/tree.h>
#include<libxml/parser.h>
#include<libxml/xpath.h>
#include<libxml/xpathInternals.h>

#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED)&& \
	defined(LIBXML_OUTPUT_ENABLED)

static void usage(const char *name);
static int example4(const char *filename, const xmlChar *xpathExpr,const xmlChar *value);
static void update_xpath_nodes(xmlNodeSetPtr nodes, const xmlChar *value);

int main(int argc, char **argv)
{
	if(argc !=4){
		fprintf(stderr, "ERROR : wrong number of arguments.\n");
		usage(argv[0]);
		return(-1);
	}
	xmlInitParser();
	LIBXML_TEST_VERSION

	if(example4(argv[1], BAD_CAST argv[2], BAD_CAST argv[3])){
		usage(argv[0]);
		return(-1);
	}

	xmlCleanupParser();

	xmlMemoryDump();
	return 0;
}

static void usage (const char *name){
	assert(name);
	fprintf(stderr, "usage: %s <xml-file> <xpath-expr> <value>\n",name);
}

static int example4(const char *filename, const xmlChar* xpathExpr, const xmlChar * value){
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	assert(filename);
	assert(xpathExpr);
	assert(value);
	doc = xmlParseFile(filename);
	if(doc == NULL){
		fprintf(stderr, "error : unable to parse file \"%s\"\n",filename);
		return(-1);
	}

	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == NULL){
		fprintf(stderr, "error : unable to create new  xpath context\n");
		xmlFreeDoc(doc);
		return(-1);
	}

	xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
	if(xpathObj == NULL){
		fprintf(stderr, "error : unable to evaluate xpath expression \"%s\"\n",xpathExpr);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return(-1);
	}

	update_xpath_nodes(xpathObj->nodesetval, value);

	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);

	xmlDocDump(stdout,doc);

	xmlFreeDoc(doc);

	return(0);
}

static void update_xpath_nodes(xmlNodeSetPtr nodes, const xmlChar *value){
	int size;
	int i;
	assert(value);
	size = (nodes) ? nodes->nodeNr : 0;
	cout<<"no of nodes.."<<size<<endl;
	for(i = size -1; i >= 0; i--){
		assert(nodes->nodeTab[i]);
		cout<<nodes->nodeTab[i]->name<<endl;
		xmlNodeSetContent(nodes->nodeTab[i],value);

		if(nodes->nodeTab[i]->type != XML_NAMESPACE_DECL)
			nodes->nodeTab[i] = NULL;
	}
}
#else
int main(void){
	fprintf(stderr, "Xpath support not compiled in \n");
	exit(1);
}
#endif

