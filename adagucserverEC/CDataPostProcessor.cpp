#include "CDataPostProcessor.h"


void writeLogFileLocal(const char * msg){
  char * logfile=getenv("ADAGUC_LOGFILE");
  if(logfile!=NULL){
    FILE * pFile = NULL;
    pFile = fopen (logfile , "a" );
    if(pFile != NULL){
 //     setvbuf(pFile, NULL, _IONBF, 0);
      fputs  (msg, pFile );
      if(strncmp(msg,"[D:",3)==0||strncmp(msg,"[W:",3)==0||strncmp(msg,"[E:",3)==0){
        time_t myTime = time(NULL);
        tm *myUsableTime = localtime(&myTime);
        char szTemp[128];
        snprintf(szTemp,127,"%.4d-%.2d-%.2dT%.2d:%.2d:%.2dZ ",
                myUsableTime->tm_year+1900,myUsableTime->tm_mon+1,myUsableTime->tm_mday,
                myUsableTime->tm_hour,myUsableTime->tm_min,myUsableTime->tm_sec
                );
        fputs  (szTemp, pFile );
      }
      fclose (pFile);
    }else fprintf(stderr,"Unable to write logfile %s\n",logfile);
  }
}

/************************/
/*      CDPPAXplusB     */
/************************/
const char *CDPPAXplusB::className="CDPPAXplusB";

const char *CDPPAXplusB::getId(){
  return "AX+B";
}
int CDPPAXplusB::isApplicable(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource){
  if(proc->attr.algorithm.equals("ax+b")){
    return CDATAPOSTPROCESSOR_RUNBEFOREREADING;
  }
  return CDATAPOSTPROCESSOR_NOTAPPLICABLE;
}
int CDPPAXplusB::execute(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource,int mode){
  if(isApplicable(proc,dataSource)!=CDATAPOSTPROCESSOR_RUNBEFOREREADING){
    return -1;
  }
  CDBDebug("Applying ax+b");
  for(size_t varNr=0;varNr<dataSource->getNumDataObjects();varNr++){
    dataSource->getDataObject(varNr)->hasScaleOffset=true;
    dataSource->getDataObject(varNr)->cdfVariable->setType(CDF_DOUBLE);
    CDF::Attribute *scale_factor = dataSource->getDataObject(varNr)->cdfVariable->getAttributeNE("scale_factor");
    CDF::Attribute *add_offset =  dataSource->getDataObject(varNr)->cdfVariable->getAttributeNE("add_offset");
    //Apply offset
    if(proc->attr.b.empty()==false){
      CT::string b;
      b.copy(proc->attr.b.c_str());
      if(add_offset==NULL){
        dataSource->getDataObject(varNr)->dfadd_offset=b.toDouble();
      }else{
        dataSource->getDataObject(varNr)->dfadd_offset+=b.toDouble();
      }
    }
    //Apply scale
    if(proc->attr.a.empty()==false){
      CT::string a;
      a.copy(proc->attr.a.c_str());
      if(scale_factor==NULL){
        dataSource->getDataObject(varNr)->dfscale_factor=a.toDouble();
      }else{
        dataSource->getDataObject(varNr)->dfscale_factor*=a.toDouble();
      }
    }
    if(proc->attr.units.empty()==false){
      dataSource->getDataObject(varNr)->setUnits(proc->attr.units.c_str());
    }
  }
  return 0;
}


/************************/
/*CDPPMSGCPPVisibleMask */
/************************/
const char *CDPPMSGCPPVisibleMask::className="CDPPMSGCPPVisibleMask";

const char *CDPPMSGCPPVisibleMask::getId(){
  return "MSGCPP_VISIBLEMASK";
}
int CDPPMSGCPPVisibleMask::isApplicable(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource){
  if(proc->attr.algorithm.equals("msgcppvisiblemask")){
    if(dataSource->getNumDataObjects()!=2&&dataSource->getNumDataObjects()!=3){
      CDBError("2 variables are needed for msgcppvisiblemask, found %d",dataSource->getNumDataObjects());
      return CDATAPOSTPROCESSOR_CONSTRAINTSNOTMET;
    }
    return CDATAPOSTPROCESSOR_RUNAFTERREADING|CDATAPOSTPROCESSOR_RUNBEFOREREADING;
  }
  return CDATAPOSTPROCESSOR_NOTAPPLICABLE;
}

int CDPPMSGCPPVisibleMask::execute(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource,int mode){
  if((isApplicable(proc,dataSource)&mode)==false){
    return -1;
  }
  if(mode==CDATAPOSTPROCESSOR_RUNBEFOREREADING){

    if(dataSource->getDataObject(0)->cdfVariable->name.equals("mask"))return 0;
    CDBDebug("CDATAPOSTPROCESSOR_RUNBEFOREREADING::Applying msgcpp VISIBLE mask");
    CDF::Variable *varToClone=dataSource->getDataObject(0)->cdfVariable;

    CDataSource::DataObject *newDataObject = new CDataSource::DataObject();

    newDataObject->variableName.copy("mask");

    
    dataSource->getDataObjectsVector()->insert(dataSource->getDataObjectsVector()->begin(),newDataObject);

    
    newDataObject->cdfVariable = new CDF::Variable();
    newDataObject->cdfObject = (CDFObject*)varToClone->getParentCDFObject();
    newDataObject->cdfObject->addVariable(newDataObject->cdfVariable);
    newDataObject->cdfVariable->setName("mask");
    newDataObject->cdfVariable->setType(CDF_SHORT);
    newDataObject->cdfVariable->setSize(dataSource->dWidth*dataSource->dHeight);

    for(size_t j=0;j<varToClone->dimensionlinks.size();j++){
      newDataObject->cdfVariable->dimensionlinks.push_back(varToClone->dimensionlinks[j]);
    }

    for(size_t j=0;j<varToClone->attributes.size();j++){
      newDataObject->cdfVariable->attributes.push_back(new CDF::Attribute(varToClone->attributes[j]));
    }

    newDataObject->cdfVariable->removeAttribute("scale_factor");
    newDataObject->cdfVariable->removeAttribute("add_offset");
    newDataObject->cdfVariable->setAttributeText("standard_name","visible_mask status_flag");
    newDataObject->cdfVariable->setAttributeText("long_name","Visible mask");
    newDataObject->cdfVariable->setAttributeText("units","1");
    newDataObject->cdfVariable->removeAttribute("valid_range");
    newDataObject->cdfVariable->removeAttribute("flag_values");
    newDataObject->cdfVariable->removeAttribute("flag_meanings");
    short attrData[3];
    attrData[0] = -1;
    newDataObject->cdfVariable->setAttribute("_FillValue", newDataObject->cdfVariable->getType(),attrData,1);
    
    attrData[0] = 0;
    attrData[1] = 1;
    newDataObject->cdfVariable->setAttribute("valid_range",newDataObject->cdfVariable->getType(),attrData,2);
    newDataObject->cdfVariable->setAttribute("flag_values",newDataObject->cdfVariable->getType(),attrData,2);
    newDataObject->cdfVariable->setAttributeText("flag_meanings","no yes");
    
    CDF::Variable::CustomMemoryReader*reader = new CDF::Variable::CustomMemoryReader();
    newDataObject->cdfVariable->setCustomReader(reader);
    
    
    //return 0;
  }
  if(mode==CDATAPOSTPROCESSOR_RUNAFTERREADING){
    CDBDebug("Applying msgcppvisiblemask");
    short *mask=(short*)dataSource->getDataObject(0)->cdfVariable->data;
    float *sunz=(float*)dataSource->getDataObject(1)->cdfVariable->data;//sunz
    float *satz=(float*)dataSource->getDataObject(2)->cdfVariable->data;
    short fNosunz=(short)dataSource->getDataObject(0)->dfNodataValue;
    size_t l=(size_t)dataSource->dHeight*(size_t)dataSource->dWidth;
    float fa=72,fb=75; 
    if(proc->attr.b.empty()==false){CT::string b;b.copy(proc->attr.b.c_str());fb=b.toDouble();}
    if(proc->attr.a.empty()==false){CT::string a;a.copy(proc->attr.a.c_str());fa=a.toDouble();}
    for(size_t j=0;j<l;j++){
      if((satz[j]<fa&&sunz[j]<fa)||(satz[j]>fb))mask[j]=fNosunz; else mask[j]=1;
    }
  }
  return 0;
}


/************************/
/*CDPPMSGCPPHIWCMask */
/************************/
const char *CDPPMSGCPPHIWCMask::className="CDPPMSGCPPHIWCMask";

const char *CDPPMSGCPPHIWCMask::getId(){
  return "MSGCPP_HIWCMASK";
}

int CDPPMSGCPPHIWCMask::isApplicable(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource){
  if(proc->attr.algorithm.equals("msgcpphiwcmask")){
    if(dataSource->getNumDataObjects()!=4&&dataSource->getNumDataObjects()!=5){
      CDBError("4 variables are needed for msgcpphiwcmask, found %d",dataSource->getNumDataObjects());
      return CDATAPOSTPROCESSOR_CONSTRAINTSNOTMET;
    }
    return CDATAPOSTPROCESSOR_RUNAFTERREADING|CDATAPOSTPROCESSOR_RUNBEFOREREADING;
  }
  return CDATAPOSTPROCESSOR_NOTAPPLICABLE;
}

int CDPPMSGCPPHIWCMask::execute(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource,int mode){
  if((isApplicable(proc,dataSource)&mode)==false){
    return -1;
  }
  if(mode==CDATAPOSTPROCESSOR_RUNBEFOREREADING){

    if(dataSource->getDataObject(0)->cdfVariable->name.equals("hiwc"))return 0;
    CDBDebug("CDATAPOSTPROCESSOR_RUNBEFOREREADING::Applying msgcpp HIWC mask");
    CDF::Variable *varToClone=dataSource->getDataObject(0)->cdfVariable;

    CDataSource::DataObject *newDataObject = new CDataSource::DataObject();

    newDataObject->variableName.copy("hiwc");

    
    dataSource->getDataObjectsVector()->insert(dataSource->getDataObjectsVector()->begin(),newDataObject);

    
    newDataObject->cdfVariable = new CDF::Variable();
    newDataObject->cdfObject = (CDFObject*)varToClone->getParentCDFObject();
    newDataObject->cdfObject->addVariable(newDataObject->cdfVariable);
    newDataObject->cdfVariable->setName("hiwc");
    newDataObject->cdfVariable->setType(CDF_SHORT);
    newDataObject->cdfVariable->setSize(dataSource->dWidth*dataSource->dHeight);

    for(size_t j=0;j<varToClone->dimensionlinks.size();j++){
      newDataObject->cdfVariable->dimensionlinks.push_back(varToClone->dimensionlinks[j]);
    }

    for(size_t j=0;j<varToClone->attributes.size();j++){
      newDataObject->cdfVariable->attributes.push_back(new CDF::Attribute(varToClone->attributes[j]));
    }

    newDataObject->cdfVariable->removeAttribute("scale_factor");
    newDataObject->cdfVariable->removeAttribute("add_offset");
    newDataObject->cdfVariable->setAttributeText("standard_name","high_ice_water_content status_flag");
    newDataObject->cdfVariable->setAttributeText("long_name","High ice water content");
    newDataObject->cdfVariable->setAttributeText("units","1");
    newDataObject->cdfVariable->removeAttribute("valid_range");
    newDataObject->cdfVariable->removeAttribute("flag_values");
    newDataObject->cdfVariable->removeAttribute("flag_meanings");
    short attrData[3];
    attrData[0] = -1;
    newDataObject->cdfVariable->setAttribute("_FillValue", newDataObject->cdfVariable->getType(),attrData,1);
    
    attrData[0] = 0;
    attrData[1] = 1;
    newDataObject->cdfVariable->setAttribute("valid_range",newDataObject->cdfVariable->getType(),attrData,2);
    newDataObject->cdfVariable->setAttribute("flag_values",newDataObject->cdfVariable->getType(),attrData,2);
    newDataObject->cdfVariable->setAttributeText("flag_meanings","no yes");
    
    CDF::Variable::CustomMemoryReader*reader = new CDF::Variable::CustomMemoryReader();
    newDataObject->cdfVariable->setCustomReader(reader);
    
    
    //return 0;
  }
  if(mode==CDATAPOSTPROCESSOR_RUNAFTERREADING){
    CDBDebug("CDATAPOSTPROCESSOR_RUNAFTERREADING::Applying msgcpp HIWC mask");
      size_t l=(size_t)dataSource->dHeight*(size_t)dataSource->dWidth;
    //CDF::allocateData(dataSource->getDataObject(0)->cdfVariable->getType(),&dataSource->getDataObject(0)->cdfVariable->data,l);
    
    short *hiwc=(short*)dataSource->getDataObject(0)->cdfVariable->data;
    float *cph=(float*)dataSource->getDataObject(1)->cdfVariable->data;
    float *cwp=(float*)dataSource->getDataObject(2)->cdfVariable->data;
    float *ctt=(float*)dataSource->getDataObject(3)->cdfVariable->data;
    float *cot=(float*)dataSource->getDataObject(4)->cdfVariable->data;
  
    for(size_t j=0;j<l;j++){
      hiwc[j]=0;
      if(cph[j]==2){
        if(cwp[j]>0.1){
          if(ctt[j]<270){
            if(cot[j]>20){
              hiwc[j]=1;
            }
          }
        }
      }
    }
  }

  //dataSource->eraseDataObject(1);
  return 0;
}




/*CPDPPExecutor*/
const char *CDPPExecutor::className="CDPPExecutor";

CDPPExecutor::CDPPExecutor(){
  //CDBDebug("CDPPExecutor");
  dataPostProcessorList = new  CT::PointerList<CDPPInterface*>();
  dataPostProcessorList->push_back(new CDPPAXplusB());
  dataPostProcessorList->push_back(new CDPPMSGCPPVisibleMask());
  dataPostProcessorList->push_back(new CDPPMSGCPPHIWCMask());
  dataPostProcessorList->push_back(new CDPPBeaufort());
  dataPostProcessorList->push_back(new CDPDBZtoRR());
  dataPostProcessorList->push_back(new CDPPAddFeatures());
}

CDPPExecutor::~CDPPExecutor(){
  //CDBDebug("~CDPPExecutor");
  delete dataPostProcessorList;
}

const CT::PointerList<CDPPInterface*> *CDPPExecutor::getPossibleProcessors(){
  return dataPostProcessorList;
}

int CDPPExecutor::executeProcessors( CDataSource *dataSource,int mode){
  //const CT::PointerList<CDPPInterface*> *availableProcs = getPossibleProcessors();
  //CDBDebug("executeProcessors, found %d",dataSource->cfgLayer->DataPostProc.size());
  for(size_t dpi=0;dpi<dataSource->cfgLayer->DataPostProc.size();dpi++){
    CServerConfig::XMLE_DataPostProc * proc = dataSource->cfgLayer->DataPostProc[dpi];
    for(size_t procId = 0;procId<dataPostProcessorList->size();procId++){
      int code = dataPostProcessorList->get(procId)->isApplicable(proc,dataSource);
      
      if(code == CDATAPOSTPROCESSOR_CONSTRAINTSNOTMET){
        CDBError("Constraints for DPP %s are not met",dataPostProcessorList->get(procId)->getId());
      }
      
      /*Will be runned when datasource metadata been loaded */
      if(mode == CDATAPOSTPROCESSOR_RUNBEFOREREADING){
        if(code&CDATAPOSTPROCESSOR_RUNBEFOREREADING){
          int status = dataPostProcessorList->get(procId)->execute(proc,dataSource,CDATAPOSTPROCESSOR_RUNBEFOREREADING);
          if(status != 0){
            CDBError("Processor %s failed execution, statuscode %d",dataPostProcessorList->get(procId)->getId(),status);
          }
        }
      }
      /*Will be runned when datasource data been loaded */
      if(mode == CDATAPOSTPROCESSOR_RUNAFTERREADING){
        if(code&CDATAPOSTPROCESSOR_RUNAFTERREADING){
          int status = dataPostProcessorList->get(procId)->execute(proc,dataSource,CDATAPOSTPROCESSOR_RUNAFTERREADING);
          if(status != 0){
            CDBError("Processor %s failed execution, statuscode %d",dataPostProcessorList->get(procId)->getId(),status);
          }
        }
      }
    }
  }
  return 0;
}


/************************/
/*      CDPPBeaufort     */
/************************/
const char *CDPPBeaufort::className="CDPPBeaufort";

const char *CDPPBeaufort::getId(){
  return "beaufort";
}
int CDPPBeaufort::isApplicable(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource){
  if(proc->attr.algorithm.equals("beaufort")){
   if(dataSource->getNumDataObjects()!=1&&dataSource->getNumDataObjects()!=2){
      CDBError("1 or 2 variables are needed for beaufort, found %d",dataSource->getNumDataObjects());
      return CDATAPOSTPROCESSOR_CONSTRAINTSNOTMET;
    }
    return CDATAPOSTPROCESSOR_RUNAFTERREADING;
  }
  return CDATAPOSTPROCESSOR_NOTAPPLICABLE;
}

float CDPPBeaufort::getBeaufort(float speed) {
  int bft;
  if (speed<0.3) {
    bft=0;
  } else if (speed<1.6) {
    bft=1;
  } else if (speed<3.4) {
    bft=2;
  } else if (speed<5.5) {
    bft=3;
  } else if (speed<8.0) {
    bft=4;
  } else if (speed<10.8) {
    bft=5;
  } else if (speed<13.9) {
    bft=6;
  } else if (speed<17.2) {
    bft=7;
  } else if (speed<20.8) {
    bft=8;
  } else if (speed<24.5) {
    bft=9;
  } else if (speed<28.5) {
    bft=10;
  } else if (speed<32.6) {
    bft=11;
  } else {
    bft=12;
  }
//  CDBDebug("bft(%f)=%d", speed, bft);
  return bft;
}
int CDPPBeaufort::execute(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource,int mode){
  if((isApplicable(proc,dataSource)&mode)==false){
    return -1;
  }
  CDBDebug("Applying beaufort %d", mode==CDATAPOSTPROCESSOR_RUNAFTERREADING);
  float factor=1;
  if(mode==CDATAPOSTPROCESSOR_RUNAFTERREADING){  
    if (dataSource->getNumDataObjects()==1) {
      CDBDebug("Applying beaufort for 1 element");
      if (dataSource->getDataObject(0)->getUnits().equals("knot")||dataSource->getDataObject(0)->getUnits().equals("kt")) {
        factor=1852./3600;
      }
      CDBDebug("Applying beaufort for 1 element with factor %f", factor);
      dataSource->getDataObject(0)->setUnits("bft");
      size_t l=(size_t)dataSource->dHeight*(size_t)dataSource->dWidth;
      float *src=(float*)dataSource->getDataObject(0)->cdfVariable->data;
      float noDataValue=dataSource->getDataObject(0)->dfNodataValue;
      for (size_t cnt=0; cnt<l; cnt++) {
        float speed = *src;
        if (speed==speed) {
          if (speed!=noDataValue) {
            *src=getBeaufort(factor*speed);
          }
        }
        src++;
      } 
      // Convert point data if needed 
      size_t nrPoints=dataSource->getDataObject(0)->points.size();
      CDBDebug("(1): %d points", nrPoints);

      for (size_t pointNo=0;pointNo<nrPoints;pointNo++){
        float speed=(float)dataSource->getDataObject(0)->points[pointNo].v;
        if (speed==speed) {
          if (speed!=noDataValue) {
            dataSource->getDataObject(0)->points[pointNo].v=getBeaufort(factor*speed);
          }
        }
      }
    }
    if (dataSource->getNumDataObjects()==2) {
      CDBDebug("Applying beaufort for 2 elements %s %s",dataSource->getDataObject(0)->getUnits().c_str(),dataSource->getDataObject(1)->getUnits().c_str()  );
      if ((dataSource->getDataObject(0)->getUnits().equals("m/s")||dataSource->getDataObject(0)->getUnits().equals("m s-1"))&&dataSource->getDataObject(1)->getUnits().equals("degree")) {
        //This is a (wind speed,direction) pair
        dataSource->getDataObject(0)->setUnits("bft");
        size_t l=(size_t)dataSource->dHeight*(size_t)dataSource->dWidth;
        float *src=(float*)dataSource->getDataObject(0)->cdfVariable->data;
        float noDataValue=dataSource->getDataObject(0)->dfNodataValue;
        for (size_t cnt=0; cnt<l; cnt++) {
          float speed=*src;
          if (speed==speed) {
            if (speed!=noDataValue) {
              *src=getBeaufort(factor * speed);
            }
          }
          src++;
        }  
        // Convert point data if needed 
        size_t nrPoints=dataSource->getDataObject(0)->points.size();
        CDBDebug("(2): %d points", nrPoints);
        for (size_t pointNo=0;pointNo<nrPoints;pointNo++){
          float speed=dataSource->getDataObject(0)->points[pointNo].v;
          if (speed==speed) {
            if (speed!=noDataValue) {
              dataSource->getDataObject(0)->points[pointNo].v=getBeaufort(factor*speed);
            }
          }
        }
      }
      if ((dataSource->getDataObject(0)->getUnits().equals("m/s")||dataSource->getDataObject(0)->getUnits().equals("m s-1"))&&
          (dataSource->getDataObject(1)->getUnits().equals("m/s")||dataSource->getDataObject(1)->getUnits().equals("m s-1"))) {
        //This is a (u,v) pair
        dataSource->getDataObject(0)->setUnits("bft");
        
        size_t l=(size_t)dataSource->dHeight*(size_t)dataSource->dWidth;
        float *srcu=(float*)dataSource->getDataObject(0)->cdfVariable->data;
        float *srcv=(float*)dataSource->getDataObject(1)->cdfVariable->data;
        float noDataValue=dataSource->getDataObject(0)->dfNodataValue;
        float speed;
        float speedu;
        float speedv;
        for (size_t cnt=0; cnt<l; cnt++) {
          speedu=*srcu;
          speedv=*srcv;
          if ((speedu==speedu) &&(speedv==speedv)){
            if ((speedu!=noDataValue)&&(speedv!=noDataValue)) {
              speed=factor * hypot(speedu, speedv);
              *srcu=getBeaufort(speed);
            } else {
              *srcu=noDataValue;
            }
          }
          srcu++;srcv++;
        }  
        // Convert point data if needed 
        size_t nrPoints=dataSource->getDataObject(0)->points.size();
        CDBDebug("(2): %d points", nrPoints);
        for (size_t pointNo=0;pointNo<nrPoints;pointNo++){
          speedu=dataSource->getDataObject(0)->points[pointNo].v;
          speedv=dataSource->getDataObject(1)->points[pointNo].v;
          if ((speedu==speedu)&&(speedv==speedv)) {
            if ((speedu!=noDataValue) && (speedv!=noDataValue)) {
              speed=factor * hypot(speedu, speedv);
              dataSource->getDataObject(0)->points[pointNo].v=getBeaufort(speed);
            } else {
              dataSource->getDataObject(0)->points[pointNo].v=noDataValue;
            }
          }
        }
        CDBDebug("Deleting dataObject(1))");
        delete(dataSource->getDataObject(1));
        dataSource->getDataObjectsVector()->erase(dataSource->getDataObjectsVector()->begin()+1); //Remove second element
      }
    }
    
  }
  return 0;
}
/************************/
/*      CDPDBZtoRR     */
/************************/
const char *CDPDBZtoRR::className="CDPDBZtoRR";

const char *CDPDBZtoRR::getId(){
  return "dbztorr";
}
int CDPDBZtoRR::isApplicable(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource){
  if(proc->attr.algorithm.equals("dbztorr")){
    return CDATAPOSTPROCESSOR_RUNAFTERREADING|CDATAPOSTPROCESSOR_RUNBEFOREREADING;
  }
  return CDATAPOSTPROCESSOR_NOTAPPLICABLE;
}

float CDPDBZtoRR::getRR(float dbZ) {
  return pow((pow(10,dbZ/10.)/200),1/1.6);
}

int CDPDBZtoRR::execute(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource,int mode){
  if((isApplicable(proc,dataSource)&mode)==false){
    return -1;
  }
  if(mode==CDATAPOSTPROCESSOR_RUNBEFOREREADING){  
//    dataSource->getDataObject(0)->cdfVariable->setAttributeText("units","mm/hr");
    dataSource->getDataObject(0)->setUnits("mm/hr");
  }
  if(mode==CDATAPOSTPROCESSOR_RUNAFTERREADING){  
    size_t l=(size_t)dataSource->dHeight*(size_t)dataSource->dWidth;
    float *src=(float*)dataSource->getDataObject(0)->cdfVariable->data;
    float noDataValue=dataSource->getDataObject(0)->dfNodataValue;
    for (size_t cnt=0; cnt<l; cnt++) {
      float dbZ = *src;
      if (dbZ==dbZ) {
        if (dbZ!=noDataValue) {
          *src=getRR(dbZ);
        }
      }
      src++;
    }
  }
  return 0;    
}

/************************/
/*      CDPPAddFeatures     */
/************************/
const char *CDPPAddFeatures::className="CDPPAddFeatures";

const char *CDPPAddFeatures::getId(){
  return "addfeatures";
}
int CDPPAddFeatures::isApplicable(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource){
  if(proc->attr.algorithm.equals("addfeatures")){
    return CDATAPOSTPROCESSOR_RUNAFTERREADING|CDATAPOSTPROCESSOR_RUNBEFOREREADING;
  }
  return CDATAPOSTPROCESSOR_NOTAPPLICABLE;
}

int CDPPAddFeatures::execute(CServerConfig::XMLE_DataPostProc* proc, CDataSource* dataSource,int mode){
  if((isApplicable(proc,dataSource)&mode)==false){
    return -1;
  }
  if(mode==CDATAPOSTPROCESSOR_RUNBEFOREREADING){  
//    dataSource->getDataObject(0)->cdfVariable->setAttributeText("units","mm/hr");
//    dataSource->getDataObject(0)->setUnits("mm/hr");
    try {
      if(dataSource->getDataObject(0)->cdfVariable->getAttribute("ADAGUC_GEOJSONPOINT"))return 0;
    } catch(int a){}
    CDBDebug("CDATAPOSTPROCESSOR_RUNBEFOREREADING::Adding features from GEOJson");
    CDF::Variable *varToClone=dataSource->getDataObject(0)->cdfVariable;

    CDataSource::DataObject *newDataObject = new CDataSource::DataObject();

    newDataObject->variableName.copy("indexes");
    dataSource->getDataObjectsVector()->insert(dataSource->getDataObjectsVector()->begin()+1,newDataObject);

    newDataObject->cdfVariable = new CDF::Variable();
    newDataObject->cdfObject = (CDFObject*)varToClone->getParentCDFObject();
    newDataObject->cdfObject->addVariable(newDataObject->cdfVariable);
    newDataObject->cdfVariable->setName("indexes");
    newDataObject->cdfVariable->setType(CDF_USHORT);
    newDataObject->cdfVariable->setSize(dataSource->dWidth*dataSource->dHeight);

    for(size_t j=0;j<varToClone->dimensionlinks.size();j++){
      newDataObject->cdfVariable->dimensionlinks.push_back(varToClone->dimensionlinks[j]);
    }
    newDataObject->cdfVariable->removeAttribute("standard_name");
    newDataObject->cdfVariable->removeAttribute("_FillValue");
    
    newDataObject->cdfVariable->setAttributeText("standard_name","indexes");
    newDataObject->cdfVariable->setAttributeText("long_name","indexes");
    newDataObject->cdfVariable->setAttributeText("units","1");
    newDataObject->cdfVariable->setAttributeText("ADAGUC_GEOJSONPOINT","1");
    dataSource->getDataObject(0)->cdfVariable->setAttributeText("ADAGUC_GEOJSONPOINT","1");
    
    unsigned short sf=65535u;
    newDataObject->cdfVariable->setAttribute("_FillValue", newDataObject->cdfVariable->getType(),&sf,1);

  }
  if(mode==CDATAPOSTPROCESSOR_RUNAFTERREADING){  
    CDataSource featureDataSource;
    if(featureDataSource.setCFGLayer(dataSource->srvParams,dataSource->srvParams->configObj->Configuration[0],dataSource->srvParams->cfg->Layer[0],NULL,0)!=0){
      return 1;
    }    
    featureDataSource.addStep(proc->attr.a.c_str(), NULL);
    CDataReader reader;
    CDBDebug("Opening %s",featureDataSource.getFileName());
    int status  = reader.open(&featureDataSource,CNETCDFREADER_MODE_OPEN_ALL);
    if(status!=0){
      CDBDebug("Can't open file %s", proc->attr.a.c_str());
      return 1;
    }else {
      std::map<std::string, float>valueMap;
      float values[dataSource->getDataObject(0)->points.size()];
      for(size_t j=0;j<dataSource->getDataObject(0)->points.size();j++){
        for (size_t c=0; c<dataSource->getDataObject(0)->points[j].paramList.size(); c++) {
          CKeyValue ckv=dataSource->getDataObject(0)->points[j].paramList[c];
//        CDBDebug("ckv: %s %s", ckv.key.c_str(), ckv.value.c_str());
          if (ckv.key.equals("station")) {
            valueMap[ckv.value.c_str()]=dataSource->getDataObject(0)->points[j].v;
//            CDBDebug("ckv: %s %s %f", ckv.key.c_str(), ckv.value.c_str(),dataSource->getDataObject(0)->points[j].v);
          }
        }
        values[j]=dataSource->getDataObject(0)->points[j].v;
      }

      CDF::allocateData(dataSource->getDataObject(1)->cdfVariable->getType(),&dataSource->getDataObject(1)->cdfVariable->data,dataSource->dWidth*dataSource->dHeight);
      //Copy the gridded values from the geojson grid to the point data's grid
      size_t l=(size_t)dataSource->dHeight*(size_t)dataSource->dWidth;
      unsigned short *src=(unsigned short*)featureDataSource.getDataObject(0)->cdfVariable->data;
      float *dest=(float*)dataSource->getDataObject(0)->cdfVariable->data;
      unsigned short noDataValue=featureDataSource.getDataObject(0)->dfNodataValue;
      float destNoDataValue=dataSource->getDataObject(0)->dfNodataValue;
      unsigned short *indexDest=(unsigned short*)dataSource->getDataObject(1)->cdfVariable->data;
      for (size_t cnt=0; cnt<l; cnt++) {
        unsigned short val = *src;
        if (val!=noDataValue) {
          *dest=values[val];
          *indexDest=val;
        }else {
          *dest=destNoDataValue;
          *indexDest=destNoDataValue;
        }
        src++;
        dest++;
        indexDest++;
      }
    }
  }
  return 0;    
}
