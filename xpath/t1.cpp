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
void store_xpath_nodes(xmlNodeSetPtr nodes);
static int parser(const char *filename, const xmlChar *xpathExpr);
void store_xpath_nodes(xmlNodeSetPtr nodes)
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

	xpathObj = xmlXPathEvalExpression(xpathExpr,xpathCtx);
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


