#include <stdio.h>
#include <cstdlib>

#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TClonesArray.h"

#include "rootalyze.h"
#include "freadin.h"
#include "histogram.h"

//FLAG

static int ndstvar=0;
static char *dstvar[MaxNDSTVar];
static int dstvarref[MaxNDSTVar];

using namespace std;

static RCNPTREE* rootevent;
static TFile *RootFile;
static TTree *tree;
static vector<vector<vector<double> > > temparray(var_subsets); // Array to temporary store the event
//static map<string,vector<double> > data_map;


/** Get variable names
    Parses the DST_VAR in hist.def for all the variable names to be saved in TTree
 */
static int root_write_header(char *comment)
{
	char *name, *p, *d, c;
	char str[256];

	// variables to write
	name = dr_get_sval(DST_VAR);
	ndstvar = 0;
	if(name){
		d = p = name;
		c = 1;  // any number other than 0 is OK
		for(int i=0;c;i++){
			c = *d++;
			if((!c && i) || c==':'){ // seperate by ':'
				if(i>255) i=255;
				strncpy(str, p, i);
				str[i] = 0x00;
				p = d;
				i = -1;
				if(ndstvar>=MaxNDSTVar){ // probably need to increase MaxNDSTVar size
					break;
				}
				dstvar[ndstvar] = strdup(str);
				dr_ref_n(str, &dstvarref[ndstvar++]);
				/* dstvar and dstvarref are the main string and
				   integer lookup-pairs for the DST_VARs */
			}
			if(!c) break;
		}
	}


	return(0);
}

/* Read the data */
int root_write_data()
{
	int ref, min, max;
	int index1, index2;
	double d;							// data value
	const int numbtoken = 11;		// number of *_CHAN variables in rootalyze.h
	int token[numbtoken] = {0};
	int mult, chan;

	int multchan[128];
	for(int i=0; i<128; i++) {
		multchan[i]=0;
	}

	// loop on all the variables
	for(int iv=0; iv<ndstvar; iv++){
		ref = dstvarref[iv]; // get the index of the dst variable (built in header)
		min = dr_min_ref(ref);
		max = dr_max_ref(ref);

		/// Check if current variable is not empty ///
		if(dr_exists(d=dr_get_ref(ref,min))){
			/// Scroll the multiplicity of current variable ///
			for(int i=min; i<max; i++){
				if(dr_exists(d=dr_get_ref(ref,i))){
					rootevent->data[dstvar[iv]].push_back(d);
				}
			}
		}
	}

	tree->Fill();
	rootevent->Clear();

	return(0);
}

/* initialize root objects */
int root_init(int nrun){
	int res;

	char rootname[128];
	sprintf(rootname, "rootfiles/run_%04d.root", nrun);
	RootFile = new TFile(rootname,"RECREATE");
	tree = new TTree("tree","Data Tree");
	rootevent = new RCNPTREE;
	tree->Branch("rcnpevent", &rootevent);



	if((res=root_write_header((char*)NULL))) {
		return(res);
	}
	return(0);
}

/* exit */
int root_exit(){
	if(!RootFile) { return(0); }
	RootFile->Write();
	RootFile->Close();
}
