/*
 * GatingSet.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: wjiang2
 */


#include "GatingSet.hpp"
#include <string>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <iostream>
using namespace std;


template <class T>
wsSampleNode getSample(T ws,string sampleID){

		string xpath=ws->xPathSample(sampleID);

		wsNode docRoot(xmlDocGetRootElement(ws->doc));

		xmlXPathObjectPtr res=docRoot.xpathInNode(xpath);
		if(res->nodesetval->nodeNr>1)
		{
			cout<<sampleID<<" is not unique within this group!"<<endl;
			xmlXPathFreeObject(res);
			throw(3);
		}

		wsSampleNode sample(res->nodesetval->nodeTab[0]);
		xmlXPathFreeObject(res);
		return sample;
}



GatingSet::~GatingSet()
{
	delete ws;
//	for_each(ghs.begin(), ghs.end(), this->freeGatingHierarchy);
	typedef map<string,GatingHierarchy *> map_t;
	BOOST_FOREACH(map_t::value_type & it,ghs){
			GatingHierarchy * ghPtr=it.second;
			string sampleName=ghPtr->sampleName;
			delete ghPtr;
			cout<<"GatingHierarchy freed:"<<sampleName<<endl;
	}

}
//read xml file and create the appropriate flowJoWorkspace object
GatingSet::GatingSet(string sFileName)
{

		LIBXML_TEST_VERSION

		/*parse the file and get the DOM */
		xmlDocPtr doc = xmlReadFile(sFileName.c_str(), NULL, 0);
		if (doc == NULL ) {
				fprintf(stderr,"Document not parsed successfully. \n");
				return;
			}
		//validity check
		xmlNodePtr cur = xmlDocGetRootElement(doc);
		if (cur == NULL) {
				fprintf(stderr,"empty document\n");
//				xmlFreeDoc(doc);
				return;
			}
		 if (!xmlStrEqual(cur->name, (const xmlChar *) "Workspace")) {
				fprintf(stderr,"document of the wrong type, root node != Workspace");
//				xmlFreeDoc(doc);
				return;
			}

		//get version info
		 xmlChar * wsVersion=xmlGetProp(cur,(const xmlChar*)"version");

		 if (xmlStrEqual(wsVersion,(const xmlChar *)"1.61")||xmlStrEqual(wsVersion,(const xmlChar *)"1.6"))
			 ws=new winFlowJoWorkspace(doc);
		 else if (xmlStrEqual(wsVersion,(const xmlChar *)"2.0"))
			 ws=new macFlowJoWorkspace(doc);
		 else
		 {
			 xmlFree(wsVersion);
			 throw(1);
		 }
		 xmlFree(wsVersion);

}

void GatingSet::parseWorkspace(unsigned short groupID)
{
	//first get all the sample IDs for given groupID
	vector<string> sampleID=ws->getSampleID(groupID);

	//contruct gating hiearchy for each sampleID
	vector<string>::iterator it;
	for(it=sampleID.begin();it!=sampleID.end();it++)
	{
		cout<<"... start parsing sample: "<<*it<<endl;
		wsSampleNode curSampleNode=getSample(ws,*it);

		GatingHierarchy * curGh=new GatingHierarchy(curSampleNode,ws);

		string sampleName=ws->getSampleName(curSampleNode);

		curGh->sampleName=sampleName;
		ghs[sampleName]=curGh;//add to the map

		sampleList.push_back(sampleName);
		cout<<"Gating hierarchy created: "<<sampleName<<endl;
	}

}


GatingHierarchy * GatingSet::getGatingHierarchy(string sampleName)
{
	return ghs[sampleName];
}

GatingHierarchy * GatingSet::getGatingHierarchy(unsigned index)
{

		return ghs[sampleList.at(index)];
}