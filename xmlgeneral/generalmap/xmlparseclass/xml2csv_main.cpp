#include"xml2csv.h"
#include"xml2csv_fun.cpp"
//#include"nm.cpp"
#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED)&& \
	defined(LIBXML_OUTPUT_ENABLED) && defined(LIBXML_TREE_ENABLED)


int main(int argc, char** argv)
{
	if(argc !=2 ){
		printf("wrong no of argv..\n");
		usage(argv[0]);
		return(-1);
	}


	int i,size;
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	vector<xmlChar*> leaves;

	FILE *fp;
	char *filename;
	filename =(char*)"result.csv";
	fp = fopen(filename,"w+");

	xmlInitParser();
	LIBXML_TEST_VERSION

	if((doc = xmlReadFile(argv[1],NULL,0)) == NULL){
		printf("could not parse file\n");
		return (-1);
	}
	root_element = xmlDocGetRootElement(doc);

	find_elements(root_element,leaves);//find  leaf nodes and store to vector leaves

//	leaves.pop_back();//removing repeated node name

	size =leaves.size();//no.of leaf
	cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<size<<leaves[0] <<endl;

	create_table(leaves,&fp);//create table with leaf node name

	xmlParse myclasses[size];//creating instances for leaf node name

	if((evaluate_leaves(myclasses,leaves,argv[1]))<0){//store the leaf values to map of each instances
		fprintf(stderr,"error in evaluate_leaves\n");
		return(-1);
	}

	for(i = 0; i < size ; i++){//print map contents of each instances
		cout<<"map contents in myclasses["<<i<<"]"<<endl;
		myclasses[i].onpath();
	}

	write_table(myclasses,&fp,size);//write contents of leaves to table

	fclose(fp);

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
