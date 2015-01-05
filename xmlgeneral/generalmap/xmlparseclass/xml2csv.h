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


class xmlParse{
	map<int,xmlChar*> xtable;

	public:
		xmlParse();
		~xmlParse();
		void tableinsert(const int i, xmlChar* data);
		int get_size();
		void onpath(void){
			for (std::map<int,xmlChar*>::iterator it=xtable.begin(); it!=xtable.end(); ++it)
			    std::cout << it->first << " => " << it->second << '\n';
		}
		xmlChar* get_element(int key);
};


static void find_elements(xmlNode* a_node, vector<xmlChar*>& leaves);
static void usage(const char *name);
void store_xpath_nodes(xmlNodeSetPtr nodes,xmlParse& abc);
static int parser(const char *filename, const xmlChar *xpathExpr,xmlParse& abc);
void create_table(vector<xmlChar*>& leaves,FILE** fp);
void write_table(xmlParse *myclasses, FILE** fp,int size);
int evaluate_leaves(xmlParse *myclasses, vector<xmlChar*>& leaves, char *argv);

