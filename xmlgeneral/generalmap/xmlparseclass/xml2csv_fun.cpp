
xmlParse::xmlParse(){
	cout<<"created"<<endl;
}
xmlParse::~xmlParse(){
	cout<<"deleting.."<<endl;
}
void xmlParse::tableinsert(const int i, xmlChar* data){
	xtable.insert(std::pair<int,xmlChar*>(i,data));
}
int xmlParse::get_size(){
	return(xtable.size());
}
xmlChar* xmlParse::get_element(int key){
			return (xtable.find(key)->second);
}







static void find_elements(xmlNode* a_node, vector<xmlChar*>& leaves)//find leaf nodes
{
	xmlNode* current = NULL;
	xmlChar * temp = NULL;
	int tempsize = 0;
	

	for(current = a_node; current; current = current->next){
		if((current->type == XML_TEXT_NODE) && !(xmlIsBlankNode(current))){
			//temp = (xmlChar*)current->parent->name;
			tempsize = xmlStrlen((xmlChar*)current->parent->name);
			temp = (xmlChar*)calloc(tempsize+3,sizeof(xmlChar));
			xmlStrcat(temp,(xmlChar*)"//");
			xmlStrcat(temp,(xmlChar*)current->parent->name);
			cout<<temp<<endl;
				leaves.push_back(temp);
		}
		if((leaves.size())<2){
				 find_elements(current->children, leaves);
		}
		else { 
			if(!(xmlStrEqual(leaves.back(),leaves[0])))
				 return find_elements(current->children, leaves);
			else{
			   	leaves.pop_back();
				return;
			}
		}
	}
	
}

static void usage(const char* name){
	assert(name);
	fprintf(stderr, "Usage : %s <xml-file>  \n",name);
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


void create_table(vector<xmlChar*>& leaves,FILE** fp){//title names are stored in csv file

	vector< xmlChar*>::iterator v = leaves.begin();
	while(v!=leaves.end()){
		cout<<"table contains column..."<< *v<<endl;
		if (v	== leaves.begin()){
			fprintf(*fp,"%s",(*v)+2);
		}
		else
			fprintf(*fp,",%s",(*v)+2);
		v++;
	}
	fprintf(*fp,"\n");
}


void write_table(xmlParse* myclasses, FILE **fp,int size){//elements are wrote under, title in csv file
	int i,j,tablerow = 0;
	tablerow = myclasses[0].get_size();

	for(i = 0; i < tablerow ; i++){
		for(j = 0; j < size; j++){
			if(j == 0)
				fprintf(*fp,"%s",myclasses[j].get_element(i));
			else
				fprintf(*fp,",%s",myclasses[j].get_element(i));
		}
		fprintf(*fp,"\n");
	}
}

int evaluate_leaves(xmlParse *myclasses, vector<xmlChar*>& leaves, char *argv)//xml file is evaluated and stores into maps
{
	int i,size;
	size = leaves.size();

	for(i = 0; i < size ; i++){
		if(parser(argv, BAD_CAST leaves[i],myclasses[i])<0){
			printf("error..on parser\n");
			return(-1);
		}
	}
	return 0;
}

