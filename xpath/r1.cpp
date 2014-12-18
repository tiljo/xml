#include<stdio.h>
#include<iostream>
#include<string.h>
#include<assert.h>

using namespace std;

#include<libxml/tree.h>
#include<libxml/parser.h>
#include<libxml/xpath.h>
#include<libxml/xpathInternals.h>

#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED)

static void usage(const char *name)
{
	assert(name);
	
	fprintf(stderr, "Usage : %s <xml-file>  <xpath-expr> [<known-ns-list>]\n",name);
	fprintf(stderr,"where <known-ns-list> is a list of known namespaces\n");
	fprintf(stderr, "in \"<prefix1>=<href1> <prefix2>=href2...\" format \n");
}

	
	
void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output){
	xmlNodePtr cur;
	int size;
	int i;

	assert(output);
	size = (nodes) ? nodes->nodeNr : 0;

	fprintf(output, "Result (%d nodes) :\n",size);
	for(i=0; i< size; ++i){
		assert(nodes->nodeTab[i]);

		if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL){
			xmlNsPtr ns;

			ns = (xmlNsPtr)nodes->nodeTab[i];
			cur = (xmlNodePtr)ns->next;
			if(cur->ns){
				fprintf(output, "= namespace \"%s\"=\"%s\"for node %s:%s\n",ns->prefix,ns->href,cur->ns->href, cur->name);
			}else{
				fprintf(output, "= nmaespace \"%s\"=\"%s\"for node %s\n",ns->prefix,ns->href,cur->name);
			}
		}else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE){
			cur = nodes->nodeTab[i];
			if(cur->ns){
				fprintf(output, "= element node \"%s:%s\"\n",cur->ns->href, cur->name);
			}else{
				fprintf(output, "= element node \"%s\"\n",cur->name);
			}
		}else{
			cur = nodes->nodeTab[i];
			fprintf(output,"= node \"%s\" : type %d\n",cur->name,cur->type);
		}
	}
}


int execute_xpath_expression(const char* filename, const xmlChar* xpathExpr)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	assert(filename);
	assert(xpathExpr);

	doc = xmlParseFile(filename);
	if(doc==NULL){
		fprintf(stderr, "error: unable to parse file \"%s\"\n",filename);
		return(-1);
	}

	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == NULL){
		fprintf(stderr,"error: unable to evaluate xpath expression \"%s\"\n",xpathExpr);
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
	cout<<"...."<<(xpathObj->nodesetval->nodeNr)<<endl;
	print_xpath_nodes(xpathObj->nodesetval,stdout);

//	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);
	return(0);
}

int main(int argc, char **argv)
{
	if((argc <3) || (argc>4)){
		fprintf(stderr,"error: wrong no.of arguments.\n");
		usage(argv[0]);
		return(-1);
	}

	xmlInitParser();
	LIBXML_TEST_VERSION

	if(execute_xpath_expression(argv[1], BAD_CAST argv[2])<0){
		usage(argv[0]);
		return(-1);
	}

	xmlCleanupParser();

	xmlMemoryDump();
	return 0;
}

#else
int main()
{
	fprintf(stderr, "Xpath support not compiled in\n");
	exit(1);
}
#endif


