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
	vector<xmlChar*> vec;
	map<int,xmlChar*> xtable;

	public:
		void vectorpush(xmlChar* data){
			vec.push_back(data);
		}

		virtual void onpath(void){
			int i=0;
			vector<xmlChar*>::iterator v = vec.begin();
			while(v!=vec.end()){
				//cout<< "value is..="<< *v << endl;

				xtable.insert(std::pair<int,xmlChar*>(i++,*v));
				v++;

			}
			cout<<"xtable size is.."<<xtable.size()<<endl;
			for (std::map<int,xmlChar*>::iterator it=xtable.begin(); it!=xtable.end(); ++it)
				    std::cout << it->first << " => " << it->second << '\n';
		}
	};

static void usage(const char *name);
void store_xpath_nodes(xmlNodeSetPtr nodes);
static int parser(const char *filename, const xmlChar *xpathExpr);

static void usage(const char* name){
	assert(name);
	fprintf(stderr, "Usage : %s <xml-file>  <xpath-expr> \n",name);
}

void store_xpath_nodes(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur;
	int size;
	int i;
	xmlParse abc;

	size = (nodes)? nodes->nodeNr : 0;

	fprintf(stdout,"Result (%d nodes) : \n",size);
	for(i=0; i < size; i++){
		assert(nodes->nodeTab[i]);
		if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE){
			cur = nodes->nodeTab[i];
			fprintf(stdout,"element is = \"%s\"\n",cur->name);

			abc.vectorpush(xmlNodeGetContent(cur));//pushing the element value into vector

			if(xmlIsBlankNode(cur->children)){//check for elment node
				printf("This is element node........\n");
			}

		} 
		else{
			cur = nodes->nodeTab[i];
			fprintf(stdout, "= node\"%s\" : type %d\n",cur->name,cur->type);		
		}
	}
			abc.onpath();//print the entire datas in the node
}



static int parser(const char *filename, const xmlChar *xpathExpr)
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

	xpathObj = xmlXPathEvalExpression(xpathExpr,xpathCtx);//evaluating the xpath
	if(xpathObj == NULL){
		fprintf(stderr,"error: unable to evaluate xpath expression \"%s\"\n",xpathExpr);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return(-1);
	}

	store_xpath_nodes(xpathObj->nodesetval);

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

	if(parser(argv[1], BAD_CAST argv[2])<0){
		printf("error...\n");
		return(-1);
	}
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
