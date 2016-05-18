#include "TObject.h"

#include <vector>
#include <string>
#include <map>

using namespace std;

/**************  RCNPTREE CLASS ****************/
/* All tree variables are referenced by string e.g. "GR_ADC" */

class RCNPTREE : public TObject {             // This indicates that the RCNPTREE class inherits from the TObject class
	                                      // (this inheritence is needed for writing this object to a tree)
  public :
	RCNPTREE() {;}                        // This is the class "constructor" it is run when an object of the class is created
	virtual ~RCNPTREE() {;}               // This is the class "destructor" it is run when an object of the class is destroyed
	                                      // (virtual indicates that it overrides the destructor of the parent TObject class)

        map<int,vector<double> > data;     // The underlying map from string to vector of data
	void Clear() {data.clear();}

	ClassDef(RCNPTREE,1);                 // This is a ROOT macro that is needed for creating a ROOT dictionary
                                              // # corresponds to version number, if = 0, events will NOT be saved
};

class DSTMap : public TObject {

  map<string,int> strtoint;
  map<int,string> inttostr;

  public:
        DSTMap() {;}
        virtual ~DSTMap() {;}

        int operator()(const string& var) {return strtoint.at(var);}
        string operator()(const int& varnum) {return inttostr.at(varnum);}
  //int Get(const string& var) {return strtoint.at(var);}
  //string Get(const int& varnum) {return inttostr.at(varnum);}
        void Set(const int& varnum, const string& var) {
	  strtoint[var] = varnum;
	  inttostr[varnum] = var;
	}

        ClassDef(DSTMap,1);
};
