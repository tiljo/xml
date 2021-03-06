#include<iostream>
#include<vector>
#include<map>
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

class xmlParse{
	map<int,xmlChar*> xtable;

	public:
		void tableinsert(const int i, xmlChar* data){
			xtable.insert(std::pair<int,xmlChar*>(i,data));
		}

		virtual void onpath(void){
			int i=0;
			cout<<"xtable size is.."<<xtable.size()<<endl;
			for (std::map<int,xmlChar*>::iterator it=xtable.begin(); it!=xtable.end(); ++it)
				    std::cout << it->first << " => " << it->second << '\n';
		}
};

static void usage(const char *name);
void store_xpath_nodes(xmlNodeSetPtr nodes,xmlParse& abc);
static int parser(const char *filename, const xmlChar *xpathExpr,xmlParse& abc);

static void usage(const char* name){
	assert(name);
	fprintf(stderr, "Usage : %s <xml-file>  <xpath-expr> \n",name);
}

void store_xpath_nodes(xmlNodeSetPtr nodes,xmlParse& abc)
{
	xmlNodePtr cur;
	int size;
	int i;


	size = (nodes)? nodes->nodeNr : 0;

	fprintf(stdout,"Result (%d nodes) : \n",size);
	for(i=0; i < size; i++){
		assert(nodes->nodeTab[i]);
		if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE){
			cur = nodes->nodeTab[i];
			fprintf(stdout,"element is = \"%s\"\n",cur->name);
			xmlChar* data = xmlNodeGetContent(cur);
			abc.tableinsert(i,data);

			if(xmlIsBlankNode(cur->children)){//check for elment node
				printf("This  element node contains children ......\n\n");
			}

		} 
		else{
			cur = nodes->nodeTab[i];
			fprintf(stdout, "= node\"%s\" : type %d\n",cur->name,cur->type);		
		}
	}
}



static int parser(const char *filename, const xmlChar *xpathExpr,xmlParse& abc)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	assert(filename);
	assert(xpathExpr);

	doc = xmlParseFile(filename);
	if(doc == NULL){
		fprintf(stderr, "error: unable to parse file \"%s\"\n",filename);
		return(-1);
	}

	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == 	NULL){
		fprintf(stderr,"error: unable to evaluate xpath expression\"%s\"\n",xpathExpr);
		xmlFreeDoc(doc);
		return(-1);
	}

	xpathObj = xmlXPathEvalExpression(xpathExpr,xpathCtx);
	if(xpathObj == NULL){
		fprintf(stderr,"error: unable to evaluate xpath expression \"%s\"\n",xpathExpr);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return(-1);
	}

	store_xpath_nodes(xpathObj->nodesetval,abc);

	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);
	return(0);
}

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
ijnt main()
{
	fprintf(stderr, "Xpath support not compiled in \n");
	exit(1);
}
#endif
