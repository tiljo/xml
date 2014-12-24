
xmlParse::xmlParse(){
	map<int,xmlChar*>xtable;
}
xmlParse::~xmlParse(){
	cout<<"deleting.."<<endl;
}
void xmlParse::tableinsert(const int i, xmlChar* data){
	xtable.insert(std::pair<int,xmlChar*>(i,data));
}



static void usage(const char* name){
	assert(name);
	fprintf(stderr, "Usage : %s <xml-file>  <xpath-expr> \n",name);
}

void store_xpath_nodes(xmlNodeSetPtr nodes, xmlParse& abc)
{
	xmlNodePtr cur;
	int size;
	int i;


	size = (nodes)? nodes->nodeNr : 0;

	fprintf(stdout,"Result (%d nodes) : \n",size);
	for(i = 0; i < size; i++){
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

