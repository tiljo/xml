#include<iostream>
#include<vector>
#include<map>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#include<libxml/tree.h>
#include<libxml/parser.h>
#include<libxml/xpath.h>
#include<libxml/xpathInternals.h>

#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED)&& \
	defined(LIBXML_OUTPUT_ENABLED) && defined(LIBXML_TREE_ENABLED)


class xmlParse{
	map<int,xmlChar*> xtable;

	public:
		xmlParse();
		~xmlParse();
		void tableinsert(const int i, xmlChar* data);
		int get_size();
		void onpath(void){
			int i=0;
			cout<<"xtable size is.."<<xtable.size()<<endl;
			for (std::map<int,xmlChar*>::iterator it=xtable.begin(); it!=xtable.end(); ++it)
			    std::cout << it->first << " => " << it->second << '\n';
		}
		xmlChar* get_element(int key){
			return (xtable.find(key)->second);
		}
		

};


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







static void print_elements(xmlNode* a_node, vector<xmlChar*>& leaves)
{
	xmlNode* current = NULL;
	xmlChar * temp = NULL;
	

	for(current = a_node; current; current = current->next){
		if((current->type == XML_TEXT_NODE) && !(xmlIsBlankNode(current))){
			temp = (xmlChar*)current->parent->name;
		//	printf("node type: Element, name: %s\n",temp);
			leaves.push_back(temp);
		//	printf("content is : %s>>%s\n",current->content,temp);
		}
		if((leaves.size())<2)
				print_elements(current->children, leaves);
		else if(!(xmlStrEqual(leaves.back(),leaves[0])))
				print_elements(current->children, leaves);
		else return;
	}
	
}


/*static void create_elements(xmlNode* a_node, xmlParse* myclasses,int *index)
{
	xmlNode* current = NULL;
	xmlChar * temp = NULL;
	

	for(current = a_node; current; current = current->next){
		if((current->type == XML_TEXT_NODE) && !(xmlIsBlankNode(current))){
			temp = (xmlChar*)current->parent->name;
			printf("node type: Element, name: %s\n",temp);
			printf("content is : %s>>%s\n",current->content,temp);
		}
				create_elements(current->children, myclasses,index);
	}
	
}*/



static void usage(const char *name);
void store_xpath_nodes(xmlNodeSetPtr nodes,xmlParse& abc);
static int parser(const char *filename, const xmlChar *xpathExpr,xmlParse& abc);





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
	if(argc !=2 ){
		printf("wrong no of argv..\n");
		usage(argv[0]);
		return(-1);
	}
	int i,j,size,tempsize;
	int tablesize=0;
	int index=0;
	xmlChar *temp = NULL;
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	xmlNodePtr n;
	xmlChar *xmlbuff;
	int buffersize;
	vector<xmlChar*> leaves;

	FILE *fp;
	char *filename;
	filename =(char*)"books.csv";
	fp = fopen(filename,"w+");

	xmlInitParser();
	LIBXML_TEST_VERSION

		if((doc = xmlReadFile(argv[1],NULL,0)) == NULL){
			printf("could not parse file\n");
			return (-1);
		}
	xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize,1);

	xmlFree(xmlbuff);
	root_element = xmlDocGetRootElement(doc);

	print_elements(root_element,leaves);

	leaves.pop_back();

	size =leaves.size();

	xmlParse myclasses[size];

	cout<<"size......."<<size<<endl;

	for(i = 0; i < size ; i++){
		tempsize = xmlStrlen(leaves[i]);
		temp = (xmlChar*)calloc(tempsize+3,sizeof(xmlChar));
		xmlStrcat(temp,(xmlChar*)"//");
		xmlStrcat(temp,leaves[i]);
		cout<<"searching for xpath="<<temp<<endl;
		cout<<i<<endl;

		if(parser(argv[1], BAD_CAST temp,myclasses[i])<0){
			printf("error..on parser\n");
			return(-1);
		}

		free(temp);
	}



	//abc.onpath();
	for(i = 0; i < size ; i++){
		myclasses[i].onpath();
	}






	vector< xmlChar*>::iterator v = leaves.begin();
	while(v!=leaves.end()){
		cout<<"in vector.."<< *v<<endl;
		if (v	== leaves.begin())
			fprintf(fp,"%s",*v);
		else
			fprintf(fp,",%s",*v);
		v++;
	}
	fprintf(fp,"\n");
	tablesize = myclasses[0].get_size();

	for(i = 0; i < tablesize; i++){//row wise printing
		for(j = 0; j < size; j++){
			if(j==0)
				fprintf(fp,"%s",myclasses[j].get_element(i));
			else
				fprintf(fp,",%s",myclasses[j].get_element(i));

			cout<<"-----------------"<<myclasses[j].get_element(i)<<endl;
		}
		fprintf(fp,"\n");
		printf("\n");
	}
	fclose(fp);
	//create_elements(root_element,myclasses,&index);




	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}

#else
int main()
{
	fprintf(stderr, "Xpath support not compiled in \n");
	exit(1);
}
#endif
