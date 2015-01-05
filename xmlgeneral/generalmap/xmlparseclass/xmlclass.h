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
	vector<xmlChar*>leaves;
	multimap<xmlChar*,xmlChar*> xtable;
	int size;
	xmlDoc *doc;
	xmlNode *root_element;





