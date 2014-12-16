struct contact{
	char *name;
	unsigned long int mobile;
};

static void create(const xmlChar* value,FILE** fp){
		int k;
			k = atoi(value);
			if(k==0 && value[0] != '0'){
				printf("name: %s stored to Contacts.csv\n",value);
				fprintf((*fp),"%s,",value);
			}
			else{
				printf("mobileNumber: %s stored to Contacts.csv\n",value);
				fprintf((*fp),"%s\n",value);
			}
}


static void processNode(xmlTextReaderPtr reader,FILE **fp){
	const xmlChar *name, *value;

	name = xmlTextReaderConstName(reader);
	if(name == NULL)
		name = BAD_CAST "--";

	value = xmlTextReaderConstValue(reader);

	if(value!=NULL	){
		if(xmlStrlen(value)> 40)
			printf(" %.40s...\n",value);
		else{
			if(xmlTextReaderNodeType(reader)==3) create(value,fp);
		}
	}
}

static void streamFile(const char *filename, struct contact **result,FILE **fp){
	xmlTextReaderPtr reader;
	int ret;

	reader = xmlReaderForFile(filename,NULL,0);
	if(reader!=NULL){
		ret = xmlTextReaderRead(reader);
		while(ret == 1){
			processNode(reader,fp);
			ret = xmlTextReaderRead(reader);
		}
		xmlFreeTextReader(reader);
		if(ret!=0){
			fprintf(stderr, "%s : failed to parse\n",filename);
		}
	}else{
		fprintf(stderr,"Unable to open %s\n",filename);
	}
}



