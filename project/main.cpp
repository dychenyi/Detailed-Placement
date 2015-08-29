#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef WIN32
# include <unistd.h>
#endif /* not WIN32 */
#include "defrReader.hpp"
#include "defiAlias.hpp"

/***********************************************************************************************/
#include "class_define.h"
#include "get_target.h"
#include "result_func.h"
#include "placerow.h"

using namespace std;

auto hash_cell = new unordered_map<string, cell*>;
auto hash_gate = new unordered_map<string, double>;
auto hash_gate_height = new unordered_map<string, double>;
/**************************************************************************************************/
char defaultName[64];
char defaultOut[64];

// Global variables
FILE* fout;
int userData;
int numObjs;
int isSumSet; // to keep track if within SUM
int isProp = 0; // for PROPERTYDEFINITIONS
int begOperand; // to keep track for constraint, to print - as the 1st char
//static double curVer = 0;
static int setSNetWireCbk = 0;



// TX_DIR:TRANSLATION ON

void myLogFunction(const char* errMsg){
    fprintf(fout, "ERROR: found error: %s\n", errMsg);
}

void myWarningLogFunction(const char* errMsg){
    fprintf(fout, "WARNING: found error: %s\n", errMsg);
}

void dataError() {
    //fprintf(fout, "ERROR: returned user data is not correct!\n");
    ;
}

void checkType(defrCallbackType_e c) {
    if (c >= 0 && c <= defrDesignEndCbkType) {
        // OK
    } else {
        fprintf(fout, "ERROR: callback type is out of bounds!\n");
    }
}

int done(defrCallbackType_e c, void* dummy, defiUserData ud) {
    checkType(c);
    if ((long)ud != userData) dataError();
    fprintf(fout, "END DESIGN\n");
    return 0;
}

int endfunc(defrCallbackType_e c, void* dummy, defiUserData ud) {
    checkType(c);
    if ((long)ud != userData) dataError();
    return 0;
}

char* orientStr(int orient) {
    switch (orient) {
        case 0: return ((char*)"N");
        case 1: return ((char*)"W");
        case 2: return ((char*)"S");
        case 3: return ((char*)"E");
        case 4: return ((char*)"FN");
        case 5: return ((char*)"FW");
        case 6: return ((char*)"FS");
        case 7: return ((char*)"FE");
    };
    return ((char*)"BOGUS");
}

int cs(defrCallbackType_e c, int num, defiUserData ud) {
    string name;
    checkType(c);
    if ((long)ud != userData) dataError();
    switch (c) {
        case defrComponentStartCbkType : name="COMPONENTS"; component_num = num; break;
        case defrNetStartCbkType : name="NETS"; net_num = num; break;
        case defrStartPinsCbkType : name="PINS"; pin_num = num; break;
        default : name="NO MATCH!"; return 1;
    }
    return 0;
}

int cls(defrCallbackType_e c, void* cl, defiUserData ud) {
    /*************addition because of net & component begin****************/
    defiNet* net1;
    defiComponent* co;
    int count = 0;
    /*************addition because of net & component end****************/

    defiSite* site; // Site and Canplace and CannotOccupy
    defiBox* box; // DieArea and
    defiPinCap* pc;
    defiPin* pin;
    int i, j;
    defiRow* row;
    defiTrack* track;
    defiGcellGrid* gcg;
    defiVia* via;
    defiRegion* re;
    defiGroup* group;
    defiScanchain* sc;
    defiIOTiming* iot;
    defiFPC* fpc;
    defiTimingDisable* td;
    defiPartition* part;
    defiPinProp* pprop;
    defiBlockage* block;
    defiSlot* slots;
    defiFill* fills;
    defiStyles* styles;
    int xl, yl, xh, yh;
    char *name, *a1, *b1;
    char **inst, **inPin, **outPin;
    int *bits;
    int size;
    int corner, typ;
    const char *itemT;
    char dir;
    defiPinAntennaModel* aModel;
    struct defiPoints points;
    checkType(c);
    if ((long)ud != userData) dataError();
    /****************************这里case选取了DIEAERA/ ROW/ PIN有关的, 又加上NET/ COMPONENT************************/
    switch (c) {
        case defrNetCbkType:{
            net1 = (defiNet*)cl;
            string net_name = net1->defiNet::name();
            net* ptr_net = vec_net + vec_net_counter;
            new (ptr_net) net(net_name);
            
            for (i = 0; i < net1->defiNet::numConnections(); i++) {
                string cell_name = net1->defiNet::instance(i);
                cell* ptr_cell = (*hash_cell)[cell_name];
                if(ptr_cell == nullptr){
                    cell_name = net1->defiNet::pin(i);
                    ptr_cell = (*hash_cell)[cell_name];
                }
                (ptr_net->list_cell).push_back(ptr_cell);
                (ptr_cell->list_net).push_back(ptr_net);
            }
            ++vec_net_counter;
            break;
        }
            
        case defrComponentCbkType:{
            co = (defiComponent*)cl;
            string cell_name = co->defiComponent::id();
            string cell_type = co->defiComponent::name();
            int x_pos, y_pos;
            bool fixed, direction;
            bool is_pin = false;
            double gate_width_d = (*hash_gate)[cell_type];
            double gate_height_d = (*hash_gate_height)[cell_type];
            int gate_width = (gate_width_d + 0.99);
            int gate_height = (gate_height_d + 0.99);
            
            if (co->defiComponent::isFixed()){
                x_pos = co->defiComponent::placementX() / x_step;
                y_pos = co->defiComponent::placementY() / y_step;
                fixed = true;
                direction = true;
            }
            
            if (co->defiComponent::isPlaced()){
                x_pos = co->defiComponent::placementX() / x_step;
                y_pos = co->defiComponent::placementY() / y_step;
                fixed = false;
                direction = true;
            }
            
            new (vec_cell + vec_cell_counter) cell{cell_name, x_pos, y_pos, gate_width, gate_width_d, gate_height, gate_height_d, fixed, false, is_pin};
            (*hash_cell)[cell_name] = (vec_cell + vec_cell_counter);
            subrow[y_pos][x_pos] = vec_cell + vec_cell_counter;
            ++vec_cell_counter;
            
            for(int i = 1 ;i < gate_height; ++i){
            	new (vec_cell + vec_cell_counter) cell{"not used" + cell_name, x_pos, y_pos + i, gate_width, gate_width_d, 1, 1, fixed, true, is_pin};
            	subrow[y_pos+i][x_pos] = vec_cell + vec_cell_counter;
            	++vec_cell_counter;
            }

            break;
        }
            
        case defrDieAreaCbkType :{
            box = (defiBox*)cl;
            x_length = box->defiBox::xh() - box->defiBox::xl();
            y_length = box->defiBox::yh() - box->defiBox::yl();
            break;
        }
            
        case defrRowCbkType :{
            ++row_counter;
            row = (defiRow*)cl;
            if (row->defiRow::hasDo()) {
                column_num = row->defiRow::xNum();
                x_step = row->defiRow::xStep();
            }
            break;
        }
            
        case defrPinCbkType :{
            pin = (defiPin*)cl;
            
            string cell_name = pin->defiPin::pinName();
            string cell_type = "PIN";
            int x_pos, y_pos;
            bool fixed, direction;
            bool is_pin = true;
            char* ss = "INPUT";
            
            if (pin->defiPin::hasDirection()){
                if(!strcmp(pin->defiPin::direction(), ss))
                    direction = true;
                else
                    direction = false;
            }
            fixed = true;
            
            if (pin->defiPin::hasPlacement()) {
                if (pin->defiPin::isFixed()) {
                    x_pos = pin->defiPin::placementX() / x_step;
                    y_pos = pin->defiPin::placementY() / y_step;
                }
            }
            
            new (vec_cell + vec_cell_counter) cell{cell_name, x_pos, y_pos, 0, 0, 0, 0, fixed, true, is_pin};
            (*hash_cell)[cell_name] = (vec_cell + vec_cell_counter);
            //subrow[y_pos][x_pos] = vec_cell + vec_cell_counter;
            ++vec_cell_counter;
            
            break;
        }
    }
    return 0;
}

int units(defrCallbackType_e c, double d, defiUserData ud) {
    checkType(c);
    unit_distance_micro = d;
    if ((long)ud != userData) dataError();
    return 0;
}
/*******************************************************************************************************/








/********************************************************************************/
int main(int argc, char** argv) {
    int num = 1734;
    char* inFile[6];
    char* outFile;
    FILE* f;
    int res;
    int noCalls = 0;
    int retStr = 0;
    int numInFile = 0;
    int fileCt = 0;

    strcpy(defaultName, "def.in");
    strcpy(defaultOut, "list");
    inFile[0] = defaultName;
    outFile = defaultOut;
    fout = stdout;
    userData = 0x01020304;

    argc--;
    argv++;
    while (argc--) {
        if (strcmp(*argv, "-d") == 0) {
            argv++;
            argc--;
            sscanf(*argv, "%d", &num);
            defiSetDebug(num, 1);
        } else if (strcmp(*argv, "-nc") == 0) {
            noCalls = 1;
        } else if (strcmp(*argv, "-o") == 0) {
            argv++;
            argc--;
            outFile = *argv;
            if ((fout = fopen(outFile, "w")) == 0) {
                fprintf(stderr, "ERROR: could not open output file\n");
                return 2;
            }
        } else if (strcmp(*argv, "-verStr") == 0) {
            /* New to set the version callback routine to return a string */
            /* instead of double. */
            retStr = 1;
        } else if (argv[0][0] != '-') {
            if (numInFile >= 6) {
                fprintf(stderr, "ERROR: too many input files, max = 6.\n");
                return 2;
            }
            inFile[numInFile++] = *argv;
        } else if (strcmp(*argv, "-h") == 0) {
            fprintf(stderr, "Usage: defrw [<defFilename>] [-o <outputFilename>]\n");
            return 2;
        } else if (strcmp(*argv, "-setSNetWireCbk") == 0) {
            setSNetWireCbk = 1;
        } else {
            fprintf(stderr, "ERROR: Illegal command line option: '%s'\n", *argv);
            return 2;
        }

        argv++;
    }

    if (noCalls == 0) {
        /*******************************************************/
        defrSetUnitsCbk(units);
        defrSetRowCbk((defrRowCbkFnType)cls);
        defrSetDieAreaCbk((defrBoxCbkFnType)cls);

        defrSetComponentStartCbk(cs);
        defrSetStartPinsCbk(cs);
        defrSetNetStartCbk(cs);
        /*******************************************************/
    }

    defrInit();

    for (fileCt = 0; fileCt < numInFile; fileCt++) {
        defrReset();
        if ((f = fopen(inFile[fileCt],"r")) == 0) {
            fprintf(stderr,"Couldn't open input file '%s'\n", inFile[fileCt]);
            return(2);
        }
        // Set case sensitive to 0 to start with, in History & PropertyDefinition
        // reset it to 1.
        res = defrRead(f, inFile[fileCt], (void*)userData, 1);
        
        if (res)
            fprintf(stderr, "Reader returns bad status.\n", inFile[fileCt]);
        
        (void)defrPrintUnusedCallbacks(fout);
        (void)defrReleaseNResetMemory();
    }

    row_num = row_counter;
    x_step = (double)x_length / column_num;
    y_step = (double)y_length / row_num;

    vec_cell = (cell*) operator new(sizeof(cell) * (component_num + pin_num + 10000));
    vec_net = (net*) operator new(sizeof(net) * net_num);
    subrow.resize(row_num + 1);		//+1 because of pin

    /*******************************read gate_size************************************/
    ifstream iifile;
    string strtmp;
    iifile.open("gate_size_vga");
    while(getline(iifile, strtmp, '\n')){
        string gate_name = strtmp;
        getline(iifile, strtmp, '\n');
        double gate_size = stod(strtmp);
        getline(iifile, strtmp, '\n');
        double gate_height = stod(strtmp);
        (*hash_gate)[gate_name] = gate_size * unit_distance_micro / x_step;
        (*hash_gate_height)[gate_name] = gate_height * unit_distance_micro / y_step;
    }

    /*******************************read gate_size************************************/
    fprintf(stderr,"******x_step: %d****y_step: %d******************************\n", x_step, y_step);

    fprintf(stderr,"row_num: %d  pin_num: %d  component_num: %d  net_num: %d  column_num: %d\n", row_num, pin_num, component_num, net_num, column_num);

    /*****************************************************************************************/
    if (noCalls == 0) {
        defrSetComponentCbk((defrComponentCbkFnType)cls);
        defrSetNetCbk((defrNetCbkFnType)cls);
        defrSetPinCbk((defrPinCbkFnType)cls);
    }

    defrInit();

    for (fileCt = 0; fileCt < numInFile; fileCt++) {
        defrReset();
        if ((f = fopen(inFile[fileCt],"r")) == 0) {
            fprintf(stderr,"Couldn't open input file '%s'\n", inFile[fileCt]);
            return(2);
        }
        // Set case sensitive to 0 to start with, in History & PropertyDefinition
        // reset it to 1.
        res = defrRead(f, inFile[fileCt], (void*)userData, 1);
        
        if (res)
            fprintf(stderr, "Reader returns bad status.\n", inFile[fileCt]);
        
        (void)defrPrintUnusedCallbacks(fout);
        (void)defrReleaseNResetMemory();
    }


    delete hash_cell;
    delete hash_gate;
    delete hash_gate_height;
    /*********************************just for test begin**************************************/
    cout<<endl<<endl;
    cout.precision(12);
    bool legal_before = check_legal();
    double hpwl_before = total_hpwl();
    time_t t=time(0);
    struct tm* now=localtime(&t);
    int old_hour = now->tm_hour;
    int old_min = now->tm_min;
    int old_sec = now->tm_sec;
    cout<<"start time: "<<old_hour<<":"<<old_min<<":"<<old_sec<<endl;
    cout<<"before:\t 1 is legal, 0 is illegal: "<<legal_before<<endl;
    cout<<"before:\t total hpwl: "<<hpwl_before<<endl;
/*
    int size0_count = 0;
    int size1_count = 0;
    int size2_count = 0;
    int size3_count = 0;

    for(int i = 0; i < vec_cell_counter; ++i){
    	if (!(vec_cell + i)->fixed){
        auto vec_target = get_target(vec_cell + i);
        if(vec_target.size() == 0) size0_count++;
        if(vec_target.size() == 1) size1_count++;
        if(vec_target.size() == 2) size2_count++;
        if(vec_target.size() == 3) size3_count++;
        //cout<<i<<endl;


//        	cout<<"cell#"<<i<<"'s target num: "<<vec_target.size()<<endl;
        	//for(auto p: vec_target)
         	// cout<<p<<" ";
        	//cout<<endl;
    	}

    }

    cout<<"size 0 num: "<<size0_count<<endl;
    cout<<"size 1 num: "<<size1_count<<endl;
    cout<<"size 2 num: "<<size2_count<<endl;
    cout<<"size 3 num: "<<size3_count<<endl;

*/

    ffout.open("for_para");
    int inc_count = 0;
    int dec_count = 0;
    auto placer=row_placer(subrow,column_num,row_num-1,x_step);/////////////////////////////////////




    int need_swap=0;
    for(int xxx = 0; xxx < 3; ++xxx){////////////////////////////////////////////////////
    	thresh = 20;
		for(int i = 0; i < vec_cell_counter; ++i){
			if(!(vec_cell + i)->fixed){
				auto vec_target = get_target(vec_cell + i);
				if(vec_target.size()!=0){
						if(vec_target[0].y!=(vec_cell+i)->y_pos){
							int result=placer.swap(vec_target[0].x, vec_target[0].y, vec_cell+i);
						}
						else{
							placer.same_row_swap(vec_target[0].x, vec_target[0].y, vec_cell+i);
						}
						++need_swap;
				}
			}
		}
		cout<<"*************************************"<<endl;
		cout<<"big loop #"<<xxx+1<<"  after strategy 1: "<<endl;
		display_time();
		cout<<"hpwl: "<<total_hpwl()<<endl;
		cout<<"*************************************"<<endl;

		thresh = 200;
		for(int xxxx = 0; xxxx < 1; ++xxxx){////////////////////////////////////////
			for(int i = 0; i < vec_cell_counter; ++i){
				if(!(vec_cell + i)->fixed){
						auto vec_target = get_target_s3(vec_cell + i);
						if(vec_target.size()!=0){
								if(vec_target[0].y!=(vec_cell+i)->y_pos){
									int result=placer.swap(vec_target[0].x, vec_target[0].y, vec_cell+i);
								}
								else{
									placer.same_row_swap(vec_target[0].x, vec_target[0].y, vec_cell+i);
								}
								++need_swap;
						}
				}
			}
		}

		cout<<"*************************************"<<endl;
		cout<<"big loop #"<<xxx+1<<"  after strategy 2: "<<endl;
		display_time();
		cout<<"hpwl: "<<total_hpwl()<<endl;
		cout<<"*************************************"<<endl<<endl;
    }

    		thresh = 0;
    		for(int i = 0; i < vec_cell_counter; ++i){
    			if(!(vec_cell + i)->fixed){
					auto vec_target = get_target(vec_cell + i);
					if(vec_target.size()!=0){
							if(vec_target[0].y!=(vec_cell+i)->y_pos){
								int result=placer.swap(vec_target[0].x, vec_target[0].y, vec_cell+i);
							}
							else{
								placer.same_row_swap(vec_target[0].x, vec_target[0].y, vec_cell+i);
							}
							++need_swap;
					}
    			}
    		}
    cout<<"***********************************"<<endl;
    cout<<"final:"<<endl<<endl;
	cout<<"whitespaceswap count "<<whitespaceswap<<endl;
    cout<<"decrease count "<<placer.decrease_count<<endl;
    cout<<"increase count "<<placer.increase_count<<endl<<endl;

    time_t tt=time(0);
    now=localtime(&tt);
    cout<<"start time: "<<old_hour<<":"<<old_min<<":"<<old_sec<<endl;
    cout<<"end   time: "<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<endl<<endl;


    cout<<"before:\t 1 is legal, 0 is illegal: "<<legal_before<<endl;
    cout<<"before:\t total hpwl: "<<hpwl_before<<endl;

    cout<<"after:\t 1 is legal, 0 is illegal: "<<check_legal()<<endl;
    cout<<"after:\t total hpwl: "<<total_hpwl()<<endl;
    cout<<"***********************************"<<endl;
    
    fclose(fout);
    return res;
}
