#ifndef CServerConfig_H
#define CServerConfig_H
#include "CXMLSerializerInterface.h"


class CServerConfig:public CXMLSerializerInterface{
  public:
    class XMLE_palette: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            int min,max,index,red,green,blue;
        }attr;
        void addAttribute(const char *name,const char *value){
          if(     equals("min",3,name)){attr.min=parseInt(value);return;}
          else if(equals("max",3,name)){attr.max=parseInt(value);return;}
          else if(equals("red",3,name)){attr.red=parseInt(value);return;}
          else if(equals("blue",4,name)){attr.blue=parseInt(value);return;}
          else if(equals("green",5,name)){attr.green=parseInt(value);return;}
          else if(equals("index",5,name)){attr.index=parseInt(value);return;}
          else if(equals("color",5,name)){//Hex color like: #A41D23
            if(value[0]=='#')if(strlen(value)==7){
              attr.red  =((value[1]>64)?value[1]-55:value[1]-48)*16+((value[2]>64)?value[2]-55:value[2]-48);
              attr.green=((value[3]>64)?value[3]-55:value[3]-48)*16+((value[4]>64)?value[4]-55:value[4]-48);
              attr.blue =((value[5]>64)?value[5]-55:value[5]-48)*16+((value[6]>64)?value[6]-55:value[6]-48);
            }
            return;
          }
        }
    };
    class XMLE_Legend: public CXMLObjectInterface{
      public:
        std::vector <XMLE_palette*> palette;
        ~XMLE_Legend(){
          XMLE_DELOBJ(palette);
        }
        class Cattr{
          public:
            CXMLString name,type;
        }attr;
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("palette",7,name)){XMLE_ADDOBJ(palette);}
           
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
        void addAttribute(const char *name,const char *value){
          if(equals("name",4,name)){attr.name.copy(value);return;}
          else if(equals("type",4,name)){attr.type.copy(value);return;}
        }
    };
    class XMLE_Scale: public CXMLObjectInterface{};
    class XMLE_Offset: public CXMLObjectInterface{};
    
    class XMLE_ContourIntervalL: public CXMLObjectInterface{};
    class XMLE_ContourIntervalH: public CXMLObjectInterface{};
    class XMLE_ShadeInterval: public CXMLObjectInterface{};
    class XMLE_SmoothingFilter: public CXMLObjectInterface{};
    
    
    class XMLE_Log: public CXMLObjectInterface{};
    class XMLE_ValueRange: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString min,max;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("min",3,attrname)){attr.min.copy(attrvalue);return;}
          else if(equals("max",3,attrname)){attr.max.copy(attrvalue);return;}
        }
    };
    
    class XMLE_Font: public CXMLObjectInterface{
      public:
      class Cattr{
        public:
        CXMLString location;
      }attr;
      void addAttribute(const char *attrname,const char *attrvalue){
        if(equals("location",8,attrname)){attr.location.copy(attrvalue);return;}
      }
    };
    
    class XMLE_ContourText: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString scale,offset,color;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("scale",5,attrname)){attr.scale.copy(attrvalue);return;}
          else if(equals("offset",6,attrname)){attr.offset.copy(attrvalue);return;}
          else if(equals("color",5,attrname)){attr.color.copy(attrvalue);return;}
        }
    };

    
    class XMLE_RenderMethod: public CXMLObjectInterface{};
    class XMLE_Style: public CXMLObjectInterface{
      public:
        std::vector <XMLE_Legend*> Legend;
        std::vector <XMLE_Scale*> Scale;
        std::vector <XMLE_Offset*> Offset;
        std::vector <XMLE_ContourIntervalL*> ContourIntervalL;
        std::vector <XMLE_ContourIntervalH*> ContourIntervalH;
        std::vector <XMLE_Log*> Log;
        std::vector <XMLE_ValueRange*> ValueRange;
        std::vector <XMLE_RenderMethod*> RenderMethod;
        std::vector <XMLE_ContourText*> ContourText;
        std::vector <XMLE_ShadeInterval*> ShadeInterval;
        std::vector <XMLE_SmoothingFilter*> SmoothingFilter;
        
        ~XMLE_Style(){
          XMLE_DELOBJ(Legend);
          XMLE_DELOBJ(Scale);
          XMLE_DELOBJ(Offset);
          XMLE_DELOBJ(Log);
          XMLE_DELOBJ(ValueRange);
          XMLE_DELOBJ(ContourIntervalL);
          XMLE_DELOBJ(ContourIntervalH);
          XMLE_DELOBJ(RenderMethod);
          XMLE_DELOBJ(ContourText);
          XMLE_DELOBJ(ShadeInterval);
          XMLE_DELOBJ(SmoothingFilter);
        }
        class Cattr{
          public:
            CXMLString name;
        }attr;
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("Legend",6,name)){XMLE_ADDOBJ(Legend);}
            else if(equals("Scale",5,name)){XMLE_ADDOBJ(Scale);}
            else if(equals("Offset",6,name)){XMLE_ADDOBJ(Offset);}
            else if(equals("Log",3,name)){XMLE_ADDOBJ(Log);}
            else if(equals("ValueRange",10,name)){XMLE_ADDOBJ(ValueRange);}
            else if(equals("ContourIntervalL",16,name)){XMLE_ADDOBJ(ContourIntervalL);}
            else if(equals("ContourIntervalH",16,name)){XMLE_ADDOBJ(ContourIntervalH);}
            else if(equals("RenderMethod",12,name)){XMLE_ADDOBJ(RenderMethod);}
            else if(equals("ContourText",11,name)){XMLE_ADDOBJ(ContourText);}
            else if(equals("ShadeInterval",13,name)){XMLE_ADDOBJ(ShadeInterval);}
            else if(equals("SmoothingFilter",15,name)){XMLE_ADDOBJ(SmoothingFilter);}
            
           
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
        void addAttribute(const char *name,const char *value){
          if(equals("name",4,name)){attr.name.copy(value);return;}
        }
    };
    class XMLE_Styles: public CXMLObjectInterface{};
    class XMLE_Title: public CXMLObjectInterface{};
    class XMLE_Name: public CXMLObjectInterface{};
    class XMLE_Abstract: public CXMLObjectInterface{};
    class XMLE_DataBaseTable: public CXMLObjectInterface{};
    class XMLE_Variable: public CXMLObjectInterface{};
    class XMLE_DataReader: public CXMLObjectInterface{};
    class XMLE_FilePath: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString filter;
        }attr;
        void addAttribute(const char *name,const char *value){
          if(equals("filter",6,name)){attr.filter.copy(value);return;}
        }
    };
    class XMLE_ImageText: public CXMLObjectInterface{};
    class XMLE_Group: public CXMLObjectInterface{
      public:
      class Cattr{
        public:
          CXMLString value;
      }attr;
      void addAttribute(const char *name,const char *value){
        if(equals("value",5,name)){attr.value.copy(value);return;}
      }
    };
    class XMLE_LatLonBox: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            float minx,miny,maxx,maxy;
        }attr;
        void addAttribute(const char *name,const char *value){
          if(equals("minx",4,name)){attr.minx=parseFloat(value);return;}
          else if(equals("miny",4,name)){attr.miny=parseFloat(value);return;}
          else if(equals("maxx",4,name)){attr.maxx=parseFloat(value);return;}
          else if(equals("maxy",4,name)){attr.maxy=parseFloat(value);return;}
        }
    };
  
 
    class XMLE_Dimension: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString name,interval,defaultV,units;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("name",4,attrname)){attr.name.copy(attrvalue);return;}
          if(equals("units",5,attrname)){attr.units.copy(attrvalue);return;}
          if(equals("default",7,attrname)){attr.defaultV.copy(attrvalue);return;}
          if(equals("interval",8,attrname)){attr.interval.copy(attrvalue);return;}
        }
    };
    
    class XMLE_Path: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString value;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("value",5,attrname)){attr.value.copy(attrvalue);return;}
        }
    };
    class XMLE_TempDir: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString value;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("value",5,attrname)){attr.value.copy(attrvalue);return;}
        }
    };
    class XMLE_OnlineResource: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString value;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("value",5,attrname)){attr.value.copy(attrvalue);return;}
        }
    };
    class XMLE_DataBase: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString parameters;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("parameters",10,attrname)){attr.parameters.copy(attrvalue);return;}
        }
    };
    class XMLE_Projection: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString id,proj4;
        }attr;
        std::vector <XMLE_LatLonBox*> LatLonBox;
        ~XMLE_Projection(){
          XMLE_DELOBJ(LatLonBox);
        }
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("id",2,attrname)){attr.id.copy(attrvalue);return;}
          if(equals("proj4",5,attrname)){attr.proj4.copy(attrvalue);return;}
        }
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("LatLonBox",9,name)){XMLE_ADDOBJ(LatLonBox);}
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
    };
    class XMLE_Cache: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString enabled;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("enabled",7,attrname)){attr.enabled.copy(attrvalue);return;}
        }
    };
    class XMLE_WCSFormat: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString name,driver,mimetype,options;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("name",4,attrname)){attr.name.copy(attrvalue);return;}
          if(equals("driver",6,attrname)){attr.driver.copy(attrvalue);return;}
          if(equals("mimetype",8,attrname)){attr.mimetype.copy(attrvalue);return;}
          if(equals("options",7,attrname)){attr.options.copy(attrvalue);return;}
        }
    };
    class XMLE_RootLayer: public CXMLObjectInterface{
      public:
        std::vector <XMLE_Name*> Name;
        std::vector <XMLE_Title*> Title;
        std::vector <XMLE_Abstract*> Abstract;
        ~XMLE_RootLayer(){
          XMLE_DELOBJ(Name);
          XMLE_DELOBJ(Title);
          XMLE_DELOBJ(Abstract);
        }
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("Name",4,name)){XMLE_ADDOBJ(Name);}
            else if(equals("Title",5,name)){XMLE_ADDOBJ(Title);}
            else if(equals("Abstract",8,name)){XMLE_ADDOBJ(Abstract);}
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
    };

    class XMLE_WMS: public CXMLObjectInterface{
      public:
        std::vector <XMLE_Title*> Title;
        std::vector <XMLE_Abstract*> Abstract;
        std::vector <XMLE_RootLayer*> RootLayer;
        ~XMLE_WMS(){
          XMLE_DELOBJ(Title);
          XMLE_DELOBJ(Abstract);
          XMLE_DELOBJ(RootLayer);
        }
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("Title",5,name)){XMLE_ADDOBJ(Title);}
            else if(equals("Abstract",8,name)){XMLE_ADDOBJ(Abstract);}
            else if(equals("RootLayer",9,name)){XMLE_ADDOBJ(RootLayer);}
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
    };
  
    class XMLE_WCS: public CXMLObjectInterface{
      public:
        std::vector <XMLE_Name*> Name;
        std::vector <XMLE_Title*> Title;
        std::vector <XMLE_Abstract*> Abstract;
        std::vector <XMLE_WCSFormat*> WCSFormat;
        ~XMLE_WCS(){
          XMLE_DELOBJ(Name);
          XMLE_DELOBJ(Title);
          XMLE_DELOBJ(Abstract);
          XMLE_DELOBJ(WCSFormat);
        }
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("Name",4,name)){XMLE_ADDOBJ(Name);}
            else if(equals("Title",5,name)){XMLE_ADDOBJ(Title);}
            else if(equals("Abstract",8,name)){XMLE_ADDOBJ(Abstract);}
            else if(equals("WCSFormat",9,name)){XMLE_ADDOBJ(WCSFormat);}
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
    };
  
    
    class XMLE_CacheDocs: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString value,cachefile;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("value",5,attrname)){attr.value.copy(attrvalue);return;}
          else if(equals("cachefile",9,attrname)){attr.cachefile.copy(attrvalue);return;}
        }
    };
    class XMLE_MetadataURL: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString value;
        }attr;
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("value",5,attrname)){attr.value.copy(attrvalue);return;}
        }
    };
    
  
    class XMLE_Layer: public CXMLObjectInterface{
      public:
        class Cattr{
          public:
            CXMLString type;
        }attr;
        
        
        std::vector <XMLE_Name*> Name;
        std::vector <XMLE_Group*> Group;
        std::vector <XMLE_Title*> Title;
        std::vector <XMLE_DataBaseTable*> DataBaseTable;
        std::vector <XMLE_Variable*> Variable;
        std::vector <XMLE_FilePath*> FilePath;
        std::vector <XMLE_DataReader*> DataReader;
        std::vector <XMLE_Dimension*> Dimension;
        std::vector <XMLE_Legend*> Legend;
        std::vector <XMLE_Scale*> Scale;
        std::vector <XMLE_Offset*> Offset;
        std::vector <XMLE_Log*> Log;
        std::vector <XMLE_ValueRange*> ValueRange;
        std::vector <XMLE_ImageText*> ImageText;
        std::vector <XMLE_LatLonBox*> LatLonBox;
        std::vector <XMLE_Projection*> Projection;
        std::vector <XMLE_Styles*> Styles;
        std::vector <XMLE_RenderMethod*> RenderMethod;
        std::vector <XMLE_MetadataURL*> MetadataURL;
        std::vector <XMLE_Cache*> Cache;
        
        
        ~XMLE_Layer(){
          XMLE_DELOBJ(Name);
          XMLE_DELOBJ(Group);
          XMLE_DELOBJ(Title);
          XMLE_DELOBJ(DataBaseTable);
          XMLE_DELOBJ(Variable);
          XMLE_DELOBJ(FilePath);
          XMLE_DELOBJ(DataReader);
          XMLE_DELOBJ(Dimension);
          XMLE_DELOBJ(Legend);
          XMLE_DELOBJ(Scale);
          XMLE_DELOBJ(Offset);
          XMLE_DELOBJ(Log);
          XMLE_DELOBJ(ValueRange);
          XMLE_DELOBJ(ImageText);
          XMLE_DELOBJ(LatLonBox);
          XMLE_DELOBJ(Projection);
          XMLE_DELOBJ(Styles);
          XMLE_DELOBJ(RenderMethod);
          XMLE_DELOBJ(MetadataURL);
          XMLE_DELOBJ(Cache);
        }
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("Name",4,name)){XMLE_ADDOBJ(Name);}
            else if(equals("Group",5,name)){XMLE_ADDOBJ(Group);}
            else if(equals("Title",5,name)){XMLE_ADDOBJ(Title);}
            else if(equals("DataBaseTable",13,name)){XMLE_ADDOBJ(DataBaseTable);}
            else if(equals("Variable",8,name)){XMLE_ADDOBJ(Variable);}
            else if(equals("FilePath",8,name)){XMLE_ADDOBJ(FilePath);}
            else if(equals("DataReader",10,name)){XMLE_ADDOBJ(DataReader);}
            else if(equals("Dimension",9,name)){XMLE_ADDOBJ(Dimension);}
            else if(equals("Legend",6,name)){XMLE_ADDOBJ(Legend);}
            else if(equals("Scale",5,name)){XMLE_ADDOBJ(Scale);}
            else if(equals("Offset",6,name)){XMLE_ADDOBJ(Offset);}
            else if(equals("Log",3,name)){XMLE_ADDOBJ(Log);}
            else if(equals("ValueRange",10,name)){XMLE_ADDOBJ(ValueRange);}
            else if(equals("ImageText",9,name)){XMLE_ADDOBJ(ImageText);}
            else if(equals("LatLonBox",9,name)){XMLE_ADDOBJ(LatLonBox);}
            else if(equals("Projection",10,name)){XMLE_ADDOBJ(Projection);}
            else if(equals("Styles",6,name)){XMLE_ADDOBJ(Styles);}
            else if(equals("RenderMethod",12,name)){XMLE_ADDOBJ(RenderMethod);}
            else if(equals("MetadataURL",11,name)){XMLE_ADDOBJ(MetadataURL);} 
            else if(equals("Cache",5,name)){XMLE_ADDOBJ(Cache);}
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
        void addAttribute(const char *attrname,const char *attrvalue){
          if(equals("type",4,attrname)){attr.type.copy(attrvalue);return;}
        }
    };

    class XMLE_Configuration: public CXMLObjectInterface{
      public:
        std::vector <XMLE_Legend*> Legend;
        std::vector <XMLE_WMS*> WMS;
        std::vector <XMLE_WCS*> WCS;
        std::vector <XMLE_Path*> Path;
        std::vector <XMLE_TempDir*> TempDir;
        std::vector <XMLE_OnlineResource*> OnlineResource;
        std::vector <XMLE_DataBase*> DataBase;
        std::vector <XMLE_Projection*> Projection;
        std::vector <XMLE_Layer*> Layer;
        std::vector <XMLE_Style*> Style;
        std::vector <XMLE_CacheDocs*> CacheDocs;
        std::vector <XMLE_Font*> Font;
        
        ~XMLE_Configuration(){
          XMLE_DELOBJ(Legend);
          XMLE_DELOBJ(WMS);
          XMLE_DELOBJ(WCS);
          XMLE_DELOBJ(Path);
          XMLE_DELOBJ(TempDir);
          XMLE_DELOBJ(OnlineResource);
          XMLE_DELOBJ(DataBase);
          XMLE_DELOBJ(Projection);
          XMLE_DELOBJ(Layer);
          XMLE_DELOBJ(Style);
          XMLE_DELOBJ(CacheDocs);
          XMLE_DELOBJ(Font);
        }
        void addElement(CXMLObjectInterface *baseClass,int rc, const char *name,const char *value){
          CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
          base->currentNode=(CXMLObjectInterface*)this;
          if(rc==0)if(value!=NULL)this->value.copy(value);
          if(rc==1){
            pt2Class=NULL;
            if(equals("Legend",6,name)){XMLE_ADDOBJ(Legend);}
            else if(equals("WMS",3,name)){XMLE_ADDOBJ(WMS);}
            else if(equals("WCS",3,name)){XMLE_ADDOBJ(WCS);}
            else if(equals("Path",4,name)){XMLE_ADDOBJ(Path);}
            else if(equals("Font",4,name)){XMLE_ADDOBJ(Font);}
            else if(equals("TempDir",7,name)){XMLE_ADDOBJ(TempDir);}
            else if(equals("OnlineResource",14,name)){XMLE_ADDOBJ(OnlineResource);}
            else if(equals("DataBase",8,name)){XMLE_ADDOBJ(DataBase);}
            else if(equals("Projection",10,name)){XMLE_ADDOBJ(Projection);}
            else if(equals("Layer",5,name)){XMLE_ADDOBJ(Layer);}
            else if(equals("Style",5,name)){XMLE_ADDOBJ(Style);}
            else if(equals("CacheDocs",9,name)){XMLE_ADDOBJ(CacheDocs);}
          }
          if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
        }
    };
    void addElementEntry(int rc,const char *name,const char *value){
      CXMLSerializerInterface * base = (CXMLSerializerInterface*)baseClass;
      base->currentNode=(CXMLObjectInterface*)this;
      if(rc==0){
        pt2Class=NULL;
        if(equals("Configuration",13,name)){XMLE_ADDOBJ(Configuration);}
      }
      if(pt2Class!=NULL)pt2Class->addElement(baseClass,rc-pt2Class->level,name,value);
    }
    void addAttributeEntry(const char *name,const char *value){
      if(currentNode!=NULL){
        currentNode->addAttribute(name,value);
      }
    }
    std::vector <XMLE_Configuration*> Configuration;
    ~CServerConfig(){
      XMLE_DELOBJ(Configuration);
    }
};
#endif
