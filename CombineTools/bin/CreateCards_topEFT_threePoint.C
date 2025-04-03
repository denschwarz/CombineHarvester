#include <string>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <cstdlib>
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"

using namespace std;

int main(int argc, char *argv[]) {

  if(argc < 2){
    cout << "[ERROR] Wrong usage you have to provide a minimum of 1 arguiment, only " << argc-1 << " provided." <<endl;
    cout << "CreateCards_topEFT_threePoint [year] [light] [minus] [njetSplit] [scaleCorrelation] [signalInjection] [fluctuatePseudoData] [blind] [binning] [noQuad] [noSigSys] [SMZero] [half]" <<endl;
    return 0;
  }

  string year;
  if(strcmp(argv[1], "UL2016preVFP") == 0)  year = "UL2016preVFP";
  else if(strcmp(argv[1], "UL2016") == 0)   year = "UL2016";
  else if(strcmp(argv[1], "UL2017") == 0)   year = "UL2017";
  else if(strcmp(argv[1], "UL2018") == 0)   year = "UL2018";
  else if(strcmp(argv[1], "ULRunII") == 0)  year = "ULRunII";
  else{
    cout << "[ERROR] year " << argv[1] << " not known. Select a valid year [UL2016preVFP, UL2016, UL2017, UL2018, ULRunII]" <<endl;
    return 0;
  }

  bool light = false;
  if(argc > 2){
    if(strcmp(argv[2], "light") == 0)  light = true;
    else if(strcmp(argv[2], "notlight") == 0)  light = false;
    else{
      cout << "[ERROR] option " << argv[2] << " not known. Select a valid option [light/notlight]" <<endl;
      return 0;
    }
  }

  bool minus = false;
  bool minus_excludeTriplet = false;
  if(argc > 3){
    if(strcmp(argv[3], "minus") == 0)  minus = true;
    else if(strcmp(argv[3], "excludeTriplet") == 0){
      minus = true;
      minus_excludeTriplet = true;
    }
    else if(strcmp(argv[3], "notminus") == 0)  minus = false;
    else{
      cout << "[ERROR] option " << argv[3] << " not known. Select a valid option [minus/notminus/excludeTriplet]" <<endl;
      return 0;
    }
  }

  bool njetSplit = false;
  if(argc > 4){
    if(strcmp(argv[4], "njetSplit") == 0)  njetSplit = true;
    else if(strcmp(argv[4], "notnjetSplit") == 0)  njetSplit = false;
    else{
      cout << "[ERROR] option " << argv[4] << " not known. Select a valid option [njetSplit]" <<endl;
      return 0;
    }
  }

  bool scaleCorrelation = false;
  if(argc > 5){
    if(strcmp(argv[5], "scaleCorrelation") == 0)  scaleCorrelation = true;
    else if(strcmp(argv[5], "notscaleCorrelation") == 0)  scaleCorrelation = false;
    else{
      cout << "[ERROR] option " << argv[5] << " not known. Select a valid option [scaleCorrelation]" <<endl;
      return 0;
    }
  }

  bool signalInjection = false;
  string signalInjectionString;
  if(argc > 6){
    if(strcmp(argv[6], "notsignalInjection") == 0) {
      signalInjection = false;
    }
    else if(strcmp(argv[6], "signalInjectionLight") == 0){
      signalInjection = true;
      signalInjectionString = "signalInjectionLight";
    }
    else if(strcmp(argv[6], "signalInjectionHeavy") == 0){
      signalInjection = true;
      signalInjectionString = "signalInjectionHeavy";
    }
    else if(strcmp(argv[6], "signalInjectionMixed") == 0){
      signalInjection = true;
      signalInjectionString = "signalInjectionMixed";
    }
    else if(strcmp(argv[6], "signalInjectionWZjets") == 0){
      signalInjection = true;
      signalInjectionString = "signalInjectionWZjets";
    }
    else{
      cout << "[ERROR] option " << argv[6] << " not known. Select a valid option [signalInjection]" <<endl;
      return 0;
    }
  }

  bool fluctuatePseudoData = false;
  if(argc > 7){
    if(strcmp(argv[7], "fluctuate") == 0)  fluctuatePseudoData = true;
    else if(strcmp(argv[7], "notfluctuate") == 0)  fluctuatePseudoData = false;
    else{
      cout << "[ERROR] option " << argv[7] << " not known. Select a valid option [fluctuate/notfluctuate]" <<endl;
      return 0;
    }
  }

  bool unblind = false;
  if(argc > 8){
    if(strcmp(argv[8], "unblind") == 0)  unblind = true;
    else if(strcmp(argv[8], "blind") == 0)  unblind = false;
    else{
      cout << "[ERROR] option " << argv[8] << " not known. Select a valid option [blind/unblind]" <<endl;
      return 0;
    }
  }

  bool newbinning = false;
  string binningString;
  if(argc > 9){
    if(strcmp(argv[9], "defaultBinning") == 0)  newbinning = false;
    else if(strcmp(argv[9], "A") == 0){
      newbinning = true;
      binningString = "binning-A";
    }
    else if(strcmp(argv[9], "B") == 0){
      newbinning = true;
      binningString = "binning-B";
    }
    else{
      cout << "[ERROR] option " << argv[9] << " not known. Select a valid option [defaultBinning/binning-A/binning-B]" <<endl;
      return 0;
    }
  }

  bool noQuad = false;
  if(argc > 10){
    if(strcmp(argv[10], "noQuad") == 0)  noQuad = true;
    else if(strcmp(argv[10], "useQuad") == 0)  noQuad = false;
    else{
      cout << "[ERROR] option " << argv[10] << " not known. Select a valid option [noQuad/useQuad]" <<endl;
      return 0;
    }
  }

  int sigSysMode = 0;
  if(argc > 11){
    if(strcmp(argv[11], "defaultSys") == 0)  sigSysMode = 1;
    else if(strcmp(argv[11], "noSigSys") == 0)  sigSysMode = 2;
    else if(strcmp(argv[11], "noEFTSys") == 0)  sigSysMode = 3;
    else{
      cout << "[ERROR] option " << argv[11] << " not known. Select a valid option [defaultSys/noSigSys/noEFTSys]" <<endl;
      return 0;
    }
  }

  string zeroString;
  bool zeroMode = false;
  if(argc > 12){
    if(strcmp(argv[12], "SMZero") == 0){
      zeroString = "SMZero";
      zeroMode = true;
    }
    else if(strcmp(argv[12], "noZero") == 0){
      zeroString = "noZero";
      zeroMode = true;
    }
    else if(strcmp(argv[12], "defaultZero") == 0){
      zeroString = "";
    }
    else{
      cout << "[ERROR] option " << argv[12] << " not known. Select a valid option [defaultZero/SMZero/noZero]" <<endl;
      return 0;
    }
  }

  bool useHalf = false;
  if(argc > 13){
    if(strcmp(argv[13], "half") == 0)  useHalf = true;
    else if(strcmp(argv[13], "nohalf") == 0)  useHalf = false;
    else{
      cout << "[ERROR] option " << argv[13] << " not known. Select a valid option [half/nohalf]" <<endl;
      return 0;
    }
  }

  // First define the location of the "auxiliaries" directory where we can
  // source the input files containing the datacard shapes
  //  string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/auxiliaries/shapes/";
  string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/CombineTools/bin/";

  // Create an empty CombineHarvester instance that will hold all of the
  // datacard configuration and histograms etc.
  ch::CombineHarvester cb;
  // Uncomment this next line to see a *lot* of debug information
  // cb.SetVerbosity(3);

  // Here we will just define two categories for an 8TeV analysis. Each entry in
  // the vector below specifies a bin name and corresponding bin_id.
  ch::Categories cats = {
    {1, "ZZ__Z1_pt"},
    {2, "WZ__Z1_pt"},
    {3, "ttZ__Z1_pt"},
  };
  if(njetSplit){
    cats = {
      {1, "ZZ__Z1_pt"},
      {2, "WZ__Z1_pt"},
      {3, "ttZ_3jets__Z1_pt"},
      {4, "ttZ_4jets__Z1_pt"},
    };
  }

  // ch::Categories is just a typedef of vector<pair<int, string>>

  //bkg_procs
  vector<string> bkg_procs = {"tWZ", "ttX", "tZq", "ggToZZ", "triBoson", "nonprompt"};
  vector<string> bkg_procs_no_nonprompt = {"tWZ", "ttX", "tZq", "ggToZZ", "triBoson"};

  //signal
  vector<string> WCnames = {"cHq1Re11", "cHq1Re22", "cHq1Re33", "cHq3Re11", "cHq3Re22", "cHq3Re33"};
  vector<string> WCnames_mix = {};
  if(light){
    WCnames = {"cHq1Re1122", "cHq1Re33", "cHq3Re1122", "cHq3Re33"};
    WCnames_mix = {"cHq1Re1122_cHq1Re33","cHq1Re1122_cHq3Re1122","cHq1Re1122_cHq3Re33","cHq1Re33_cHq3Re1122","cHq1Re33_cHq3Re33","cHq3Re1122_cHq3Re33"};
    if(minus){
      // WCnames = {"cHqMRe1122", "cHqMRe33", "cHq3MRe1122", "cHq3MRe33"};
      // WCnames_mix = {"cHqMRe1122_cHqMRe33","cHqMRe1122_cHq3MRe1122","cHqMRe1122_cHq3MRe33","cHqMRe33_cHq3MRe1122","cHqMRe33_cHq3MRe33","cHq3MRe1122_cHq3MRe33"};
      WCnames = {"cHqMRe1122", "cHqMRe33", "cHq3MRe1122", "cHq3MRe33", "cHuRe1122", "cHuRe33", "cHdRe1122", "cHdRe33", "cW", "cWtil"};
      WCnames_mix = {
          "cHqMRe1122_cHqMRe33",
          "cHqMRe1122_cHq3MRe1122",
          "cHqMRe1122_cHq3MRe33",
          "cHqMRe1122_cHuRe1122",
          "cHqMRe1122_cHuRe33",
          "cHqMRe1122_cHdRe1122",
          "cHqMRe1122_cHdRe33",
          "cHqMRe1122_cW",
          "cHqMRe1122_cWtil",
          "cHqMRe33_cHq3MRe1122",
          "cHqMRe33_cHq3MRe33",
          "cHqMRe33_cHuRe1122",
          "cHqMRe33_cHuRe33",
          "cHqMRe33_cHdRe1122",
          "cHqMRe33_cHdRe33",
          "cHqMRe33_cW",
          "cHqMRe33_cWtil",
          "cHq3MRe1122_cHq3MRe33",
          "cHq3MRe1122_cHuRe1122",
          "cHq3MRe1122_cHuRe33",
          "cHq3MRe1122_cHdRe1122",
          "cHq3MRe1122_cHdRe33",
          "cHq3MRe1122_cW",
          "cHq3MRe1122_cWtil",
          "cHq3MRe33_cHuRe1122",
          "cHq3MRe33_cHuRe33",
          "cHq3MRe33_cHdRe1122",
          "cHq3MRe33_cHdRe33",
          "cHq3MRe33_cW",
          "cHq3MRe33_cWtil",
          "cHuRe1122_cHuRe33",
          "cHuRe1122_cHdRe1122",
          "cHuRe1122_cHdRe33",
          "cHuRe1122_cW",
          "cHuRe1122_cWtil",
          "cHuRe33_cHdRe1122",
          "cHuRe33_cHdRe33",
          "cHuRe33_cW",
          "cHuRe33_cWtil",
          "cHdRe1122_cHdRe33",
          "cHdRe1122_cW",
          "cHdRe1122_cWtil",
          "cHdRe33_cW",
          "cHdRe33_cWtil",
          "cW_cWtil"
      };
      if(minus_excludeTriplet){
        WCnames = {"cHqMRe1122", "cHqMRe33", "cHq3MRe1122"};
        WCnames_mix = {"cHqMRe1122_cHqMRe33","cHqMRe1122_cHq3MRe1122","cHqMRe33_cHq3MRe1122"};
      }
    }
  }
  vector<string> sig_procs = {"sm"};
  for(auto WCname: WCnames ){
    sig_procs.push_back("sm_lin_quad_"+WCname);
    sig_procs.push_back("quad_"+WCname);
  }
  for(auto WCname_mix: WCnames_mix ){
    sig_procs.push_back("sm_lin_quad_mixed_"+WCname_mix);
  }

  vector<string> sig_procs_for_sys = sig_procs;
  if(sigSysMode == 1)       sig_procs_for_sys = sig_procs;
  else if(sigSysMode == 2)  sig_procs_for_sys = vector<string>();
  else if(sigSysMode == 3)  sig_procs_for_sys = {"sm"};
  else{
    cout << "[ERROR] sigSysMode " << sigSysMode << " not known." <<endl;
    return 0;
  }


  cb.AddObservations({"*"}, {"topEFT"}, {"13TeV"}, {year}, cats);
  cb.AddProcesses({"*"},  {"topEFT"}, {"13TeV"}, {year}, bkg_procs, cats, false);
  cb.AddProcesses({"*"},  {"topEFT"}, {"13TeV"}, {year}, sig_procs, cats, true);

  //Some of the code for this is in a nested namespace, so
  // we'll make some using declarations first to simplify things a bit.
  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::bin_id;
  using ch::syst::process;

  // Shape uncertainties

  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_b_correlated", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_l_correlated", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_b_uncorrelated_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_l_uncorrelated_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_b_uncorrelated_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_l_uncorrelated_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_b_uncorrelated_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_l_uncorrelated_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_b_uncorrelated_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "BTag_l_uncorrelated_2018", "shape", SystMap<>::init(1.00));


  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Trigger_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Trigger_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Trigger_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Trigger_2018", "shape", SystMap<>::init(1.00));


  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "PU", "shape", SystMap<>::init(1.00));

  // cb.cp().process( ch::JoinStr({sig_procs, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_AbsoluteMPFBias", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_AbsoluteScale", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_AbsoluteStat_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_AbsoluteStat_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_AbsoluteStat_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_AbsoluteStat_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeBal", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeFSR", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC1_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC1_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC1_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC1_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC2_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC2_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC2_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJEREC2_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeJERHF", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtBB", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC1_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC1_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC1_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC1_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC2_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC2_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC2_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtEC2_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativePtHF", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatEC_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatEC_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatEC_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatEC_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatFSR_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatFSR_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatFSR_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatFSR_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatHF_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatHF_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatHF_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeStatHF_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeSample_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeSample_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeSample_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_RelativeSample_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_PileUpDataMC", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_PileUpPtBB", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_PileUpPtEC1", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_PileUpPtEC2", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_PileUpPtHF", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_PileUpPtRef", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_FlavorQCD", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_Fragmentation", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_SinglePionECAL", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_SinglePionHCAL", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_TimePtEta_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_TimePtEta_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_TimePtEta_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JES_TimePtEta_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JER_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JER_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JER_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "JER_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Unclustered_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Unclustered_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Unclustered_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Unclustered_2018", "shape", SystMap<>::init(1.00));

  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_elec_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_elec_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_elec_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_elec_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_muon_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_muon_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_muon_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "Fakerate_muon_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_correlated_elec", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_elec_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_elec_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_elec_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_elec_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_correlated_muon", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_muon_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_muon_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_muon_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_muon_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_correlated_both", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_both_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_both_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_both_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "FakerateClosure_uncorrelated_both_2018", "shape", SystMap<>::init(1.00));
  // cb.cp().process({"nonprompt"}).AddSyst(cb, "rate_nonprompt", "lnN", SystMap<>::init(1.3)); //Flat 30%

  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Prefire", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepReco", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDsys_muon", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_muon_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_muon_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_muon_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_muon_2018", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDsys_elec", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_elec_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_elec_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_elec_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "LepIDstat_elec_2018", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Lumi_uncorrelated_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Lumi_uncorrelated_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Lumi_uncorrelated_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Lumi_correlated_161718", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "Lumi_correlated_1718", "shape", SystMap<>::init(1.00));

  vector<string> pdfstrings;
  for(int i=0; i<100; i++){
    string pdfstring = "PDF_"+to_string(i+1);
    pdfstrings.push_back(pdfstring);
    cb.cp().process( ch::JoinStr({ sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, pdfstring, "shape", SystMap<>::init(1.00));
  }

  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "WZ_Njet_reweight", "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "WZ_heavyFlavour", "shape", SystMap<>::init(1.00));

  if(scaleCorrelation){
    cb.cp().process(sig_procs_for_sys).AddSyst(cb, "muR_diboson",       "shape", SystMap<>::init(1.00));
  }
  else{
    cb.cp().process(sig_procs_for_sys).AddSyst(cb, "muR_WZ",       "shape", SystMap<>::init(1.00));
    cb.cp().process(sig_procs_for_sys).AddSyst(cb, "muR_ZZ",       "shape", SystMap<>::init(1.00));
  }
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "muR_ttZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"tZq"}).AddSyst(cb, "muR_tZq",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"tWZ"}).AddSyst(cb, "muR_tWZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"ttX"}).AddSyst(cb, "muR_ttX",      "shape", SystMap<>::init(1.00));
  // cb.cp().process({"ggToZZ"}).AddSyst(cb, "muR_ggToZZ",   "shape", SystMap<>::init(1.00));
  cb.cp().process({"triBoson"}).AddSyst(cb, "muR_triBoson", "shape", SystMap<>::init(1.00));

  if(scaleCorrelation){
    cb.cp().process(sig_procs_for_sys).AddSyst(cb, "muF_diboson",       "shape", SystMap<>::init(1.00));
  }
  else{
    cb.cp().process( sig_procs_for_sys ).AddSyst(cb, "muF_WZ",       "shape", SystMap<>::init(1.00));
    cb.cp().process( sig_procs_for_sys ).AddSyst(cb, "muF_ZZ",       "shape", SystMap<>::init(1.00));
  }
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "muF_ttZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"tZq"}).AddSyst(cb, "muF_tZq",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"tWZ"}).AddSyst(cb, "muF_tWZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"ttX"}).AddSyst(cb, "muF_ttX",      "shape", SystMap<>::init(1.00));
  // cb.cp().process({"ggToZZ"}).AddSyst(cb, "muF_ggToZZ",   "shape", SystMap<>::init(1.00));
  cb.cp().process({"triBoson"}).AddSyst(cb, "muF_triBoson", "shape", SystMap<>::init(1.00));

  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "EWK_mul_WZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "EWK_mul_ZZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "EWK_add_WZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "EWK_add_ZZ",      "shape", SystMap<>::init(1.00));

  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "ISR_ttZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "ISR_WZ",       "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "ISR_ZZ",       "shape", SystMap<>::init(1.00));
  cb.cp().process({"tZq"}).AddSyst(cb, "ISR_tZq",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"tWZ"}).AddSyst(cb, "ISR_tWZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"ttX"}).AddSyst(cb, "ISR_ttX",      "shape", SystMap<>::init(1.00));
  cb.cp().process({"ggToZZ"}).AddSyst(cb, "ISR_ggToZZ",   "shape", SystMap<>::init(1.00));
  cb.cp().process({"triBoson"}).AddSyst(cb, "ISR_triBoson", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs_for_sys, bkg_procs_no_nonprompt}) ).AddSyst(cb, "FSR", "shape", SystMap<>::init(1.00));
  // cb.cp().process( ch::JoinStr({sig_procs_for_sys}) ).AddSyst(cb, "FSR_ttZ",      "shape", SystMap<>::init(1.00));
  // cb.cp().process( ch::JoinStr({sig_procs_for_sys}) ).AddSyst(cb, "FSR_WZ",       "shape", SystMap<>::init(1.00));
  // cb.cp().process( ch::JoinStr({sig_procs_for_sys}) ).AddSyst(cb, "FSR_ZZ",       "shape", SystMap<>::init(1.00));
  // cb.cp().process({"tZq"}).AddSyst(cb, "FSR_tZq",      "shape", SystMap<>::init(1.00));
  // cb.cp().process({"tWZ"}).AddSyst(cb, "FSR_tWZ",      "shape", SystMap<>::init(1.00));
  // cb.cp().process({"ttX"}).AddSyst(cb, "FSR_ttX",      "shape", SystMap<>::init(1.00));
  // cb.cp().process({"ggToZZ"}).AddSyst(cb, "FSR_ggToZZ",   "shape", SystMap<>::init(1.00));
  // cb.cp().process({"triBoson"}).AddSyst(cb, "FSR_triBoson", "shape", SystMap<>::init(1.00));

  // Rate uncertainties signals
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "rate_ttZ", "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "rate_WZ", "shape", SystMap<>::init(1.00));
  cb.cp().process(sig_procs_for_sys).AddSyst(cb, "rate_ZZ", "shape", SystMap<>::init(1.00));

  // Rate uncertainties backgrounds
  cb.cp().process({"ttX"}).AddSyst(cb, "rate_ttX", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"triBoson"}).AddSyst(cb, "rate_triBoson", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"tWZ"}).AddSyst(cb, "rate_tWZ", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"tZq"}).AddSyst(cb, "rate_tZq", "lnN", SystMap<>::init(1.1));
  cb.cp().process({"ggToZZ"}).AddSyst(cb, "rate_ggToZZ", "lnN", SystMap<>::init(1.2));
  // ttZ, tZq and tWZ numbers from https://link.springer.com/content/pdf/10.1007/JHEP12(2021)083.pdf
  // ZZ and WZ from AN

  // EFT uncertainties
  cb.cp().process({"tWZ"}).AddSyst(cb, "eft_tWZ", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"tZq"}).AddSyst(cb, "eft_tZq", "lnN", SystMap<>::init(1.2));

  vector<string> PSUncerts = {
    "ISR_ttZ","ISR_WZ","ISR_ZZ","ISR_tZq","ISR_tWZ","ISR_ttX","ISR_triBoson","ISR_ggToZZ",
    "FSR"
  };
  vector<string> scaleUncerts_sig = {
    "muR_ttZ","muR_WZ","muR_ZZ",
    "muF_ttZ","muF_WZ","muF_ZZ"
  };
  vector<string> scaleUncerts_bkg = {
    "muR_tZq","muR_tWZ","muR_ttX","muR_triBoson",//"muR_ggToZZ",
    "muF_tZq","muF_tWZ","muF_ttX","muF_triBoson"//,"muF_ggToZZ",
  };
  vector<string> WZUncerts = {"WZ_Njet_reweight", "WZ_heavyFlavour"};
  vector<string> EWKUncerts = {
    "EWK_mul_WZ", "EWK_mul_ZZ",
    "EWK_add_WZ", "EWK_add_ZZ"
  };
  if(scaleCorrelation){
    scaleUncerts_sig = {"muR_ttZ","muR_diboson","muF_ttZ","muF_diboson"};
  }

  vector<string> pdfUncerts;
  for(auto pdfstring: pdfstrings) pdfUncerts.push_back(pdfstring);
  vector<string> rateSigUncerts = {"rate_ttZ", "rate_WZ", "rate_ZZ"};
  vector<string> rateBkgUncerts = {"rate_ttX", "rate_triBoson", "rate_tWZ", "rate_tZq", "rate_ggToZZ"};
  vector<string> eftBkgUncerts = {"eft_tWZ", "eft_tZq"};

  vector<string> nonpromptUncerts = {"Fakerate_muon_2016preVFP", "Fakerate_muon_2016","Fakerate_muon_2017","Fakerate_muon_2018", "Fakerate_elec_2016preVFP", "Fakerate_elec_2016","Fakerate_elec_2017","Fakerate_elec_2018", "FakerateClosure_correlated_elec", "FakerateClosure_uncorrelated_elec_2016preVFP", "FakerateClosure_uncorrelated_elec_2016", "FakerateClosure_uncorrelated_elec_2017", "FakerateClosure_uncorrelated_elec_2018", "FakerateClosure_correlated_muon", "FakerateClosure_uncorrelated_muon_2016preVFP", "FakerateClosure_uncorrelated_muon_2016", "FakerateClosure_uncorrelated_muon_2017", "FakerateClosure_uncorrelated_muon_2018", "FakerateClosure_correlated_both", "FakerateClosure_uncorrelated_both_2016preVFP", "FakerateClosure_uncorrelated_both_2016", "FakerateClosure_uncorrelated_both_2017", "FakerateClosure_uncorrelated_both_2018"};
  vector<string> lumiUncerts = {"Lumi_uncorrelated_2016","Lumi_uncorrelated_2017","Lumi_uncorrelated_2018","Lumi_correlated_161718","Lumi_correlated_1718"};
  vector<string> btagUncerts = {"BTag_b_correlated","BTag_l_correlated","BTag_b_uncorrelated_2016preVFP","BTag_l_uncorrelated_2016preVFP","BTag_b_uncorrelated_2016","BTag_l_uncorrelated_2016","BTag_b_uncorrelated_2017","BTag_l_uncorrelated_2017","BTag_b_uncorrelated_2018","BTag_l_uncorrelated_2018"};
  vector<string> leptonUncerts = {"LepReco", "LepIDsys_muon","LepIDstat_muon_2016preVFP","LepIDstat_muon_2016","LepIDstat_muon_2017","LepIDstat_muon_2018", "LepIDsys_elec","LepIDstat_elec_2016preVFP","LepIDstat_elec_2016","LepIDstat_elec_2017","LepIDstat_elec_2018"};
  vector<string> otherExpUncerts = {"Trigger_2016preVFP", "Trigger_2016", "Trigger_2017", "Trigger_2018", "PU", "Prefire"};
  vector<string> jetUncerts = {"JER_2016preVFP", "JER_2016", "JER_2017", "JER_2018", "Unclustered_2016preVFP", "Unclustered_2016", "Unclustered_2017", "Unclustered_2018", "JES_AbsoluteMPFBias",    "JES_AbsoluteScale",  "JES_AbsoluteStat_2016preVFP",  "JES_AbsoluteStat_2016",  "JES_AbsoluteStat_2017",  "JES_AbsoluteStat_2018",  "JES_RelativeBal",  "JES_RelativeFSR",  "JES_RelativeJEREC1_2016preVFP",  "JES_RelativeJEREC1_2016",  "JES_RelativeJEREC1_2017",  "JES_RelativeJEREC1_2018",  "JES_RelativeJEREC2_2016preVFP",  "JES_RelativeJEREC2_2016",  "JES_RelativeJEREC2_2017",  "JES_RelativeJEREC2_2018",  "JES_RelativeJERHF",  "JES_RelativePtBB",  "JES_RelativePtEC1_2016preVFP",  "JES_RelativePtEC1_2016",  "JES_RelativePtEC1_2017",  "JES_RelativePtEC1_2018",  "JES_RelativePtEC2_2016preVFP",  "JES_RelativePtEC2_2016",  "JES_RelativePtEC2_2017",  "JES_RelativePtEC2_2018",  "JES_RelativePtHF",  "JES_RelativeStatEC_2016preVFP",  "JES_RelativeStatEC_2016",  "JES_RelativeStatEC_2017",  "JES_RelativeStatEC_2018",  "JES_RelativeStatFSR_2016preVFP",  "JES_RelativeStatFSR_2016",  "JES_RelativeStatFSR_2017",  "JES_RelativeStatFSR_2018",  "JES_RelativeStatHF_2016preVFP",  "JES_RelativeStatHF_2016",  "JES_RelativeStatHF_2017",  "JES_RelativeStatHF_2018",  "JES_RelativeSample_2016preVFP",  "JES_RelativeSample_2016",  "JES_RelativeSample_2017",  "JES_RelativeSample_2018", "JES_PileUpDataMC",  "JES_PileUpPtBB",  "JES_PileUpPtEC1",  "JES_PileUpPtEC2",  "JES_PileUpPtHF",  "JES_PileUpPtRef",  "JES_FlavorQCD",  "JES_Fragmentation",  "JES_SinglePionECAL","JES_SinglePionHCAL","JES_TimePtEta_2016preVFP","JES_TimePtEta_2016","JES_TimePtEta_2017","JES_TimePtEta_2018"};

  cb.cp().SetGroup("ps", PSUncerts);
  cb.cp().SetGroup("scale_sig", scaleUncerts_sig);
  cb.cp().SetGroup("scale_bkg", scaleUncerts_bkg);
  cb.cp().SetGroup("wz", WZUncerts);
  cb.cp().SetGroup("ewk", EWKUncerts);
  cb.cp().SetGroup("pdf", pdfUncerts);
  cb.cp().SetGroup("rate_sig", rateSigUncerts);
  cb.cp().SetGroup("rate_bkg", rateBkgUncerts);
  cb.cp().SetGroup("eft_bkg", eftBkgUncerts);
  cb.cp().SetGroup("nonprompt", nonpromptUncerts);
  cb.cp().SetGroup("lumi", lumiUncerts);
  cb.cp().SetGroup("btag", btagUncerts);
  cb.cp().SetGroup("lepton", leptonUncerts);
  cb.cp().SetGroup("jet", jetUncerts);
  cb.cp().SetGroup("other_exp", otherExpUncerts);

  string dirname_suffix = "";
  if(light)            dirname_suffix+="_light";
  if(minus)            dirname_suffix+="_minus";
  if(njetSplit)        dirname_suffix+="_NjetSplit";
  if(scaleCorrelation) dirname_suffix+="_scaleCorrelation";
  if(signalInjection)  dirname_suffix+="_"+signalInjectionString;
  if(fluctuatePseudoData) dirname_suffix+="_fluctuatePseudoData";
  if(newbinning)       dirname_suffix+="_"+binningString;
  if(noQuad)           dirname_suffix+="_noQuad";
  if(zeroMode)         dirname_suffix+="_"+zeroString;
  if(useHalf)          dirname_suffix+="_HALF";

  // relative link from this dir
  string dir = "../../../../../../../groups/hephy/cms/dennis.schwarz/www/tWZ/CombineInput_UL_threePoint_noData"+dirname_suffix+"/"+year+"/";
  if(unblind) dir = "../../../../../../../groups/hephy/cms/dennis.schwarz/www/tWZ/CombineInput_UL_threePoint"+dirname_suffix+"/"+year+"/";
  string input_filename = dir+"CombineInput.root";

  cb.cp().backgrounds().ExtractShapes(
    aux_shapes + input_filename,
    "$BIN/$PROCESS",
    "$BIN/$PROCESS__$SYSTEMATIC"
  );

  cb.cp().signals().ExtractShapes(
    aux_shapes + input_filename,
    "$BIN/$PROCESS",
    "$BIN/$PROCESS__$SYSTEMATIC"
  );

  // This function modifies every entry to have a standardised bin name of
  // the form: {analysis}_{channel}_{bin_id}_{era}
  // hich is commonly used in the htt analyses
  ch::SetStandardBinNames(cb);

  // First we generate a set of bin names:
  set<string> bins = cb.bin_set();
  // This method will produce a set of unique bin names by considering all
  // Observation, Process and Systematic entries in the CombineHarvester
  // instance.

  // We create the output root file that will contain all the shapes.
  TString filename = "topEFT_"+year+".input.root";
  TFile output(filename, "RECREATE");

  // Finally we iterate through each bin,mass combination and write a
  // datacard.
  for (auto b : bins) {
    cout << ">> Writing datacard for bin: " << b << " and mass: " << 1
    << "\n";
    // We need to filter on both the mass and the mass hypothesis,
    // where we must remember to include the "*" mass entry to get
    // all the data and backgrounds.
    cb.cp().bin({b}).mass({1, "*"}).WriteDatacard(
      b + "_" + year + ".txt", output);
  }
}
