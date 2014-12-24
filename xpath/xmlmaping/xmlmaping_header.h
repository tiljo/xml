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


class xmlParse{
	map<int,xmlChar*> xtable;

	public:
		xmlParse();
		~xmlParse();
		void tableinsert(const int i, xmlChar* data);
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
