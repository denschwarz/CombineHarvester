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
    cout << "CreateCards_topEFT_threePoint year [light]" <<endl;
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
    else{
      cout << "[ERROR] option " << argv[1] << " not known. Select a valid option [light]" <<endl;
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
  // ch::Categories is just a typedef of vector<pair<int, string>>

  //bkg_procs
  vector<string> bkg_procs = {"tWZ", "ttX", "tZq", "triBoson", "nonprompt"};

  //signal
  vector<string> WCnames = {"cHq1Re11", "cHq1Re22", "cHq1Re33", "cHq3Re11", "cHq3Re22", "cHq3Re33"};
  vector<string> WCnames_mix = {};
  if(light){
    WCnames = {"cHq1Re1122", "cHq1Re33", "cHq3Re1122", "cHq3Re33"};
    WCnames_mix = {"cHq1Re1122_cHq1Re33", "cHq3Re1122_cHq3Re33"};
  }
  vector<string> sig_procs = {"sm"};
  for(auto WCname: WCnames ){
    sig_procs.push_back("sm_lin_quad_"+WCname);
    sig_procs.push_back("sm_quad_"+WCname);
  }
  for(auto WCname_mix: WCnames_mix ){
    sig_procs.push_back("sm_lin_quad_mixed_"+WCname_mix);
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

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_b_correlated", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_l_correlated", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_b_uncorrelated_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_l_uncorrelated_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_b_uncorrelated_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_l_uncorrelated_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_b_uncorrelated_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_l_uncorrelated_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_b_uncorrelated_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "BTag_l_uncorrelated_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Trigger", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "PU", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "JES", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "JER", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Fakerate", "shape", SystMap<>::init(1.00));
  cb.cp().process({"nonprompt"}).AddSyst(cb, "rate_nonprompt", "lnN", SystMap<>::init(1.3)); //Flat 30%

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Prefire", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepReco", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDsys", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2016preVFP", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2018", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_uncorrelated_2016", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_uncorrelated_2017", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_uncorrelated_2018", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_correlated_161718", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_correlated_1718", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "PDF", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR_ttZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR_WZ",       "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR_ZZ",       "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR_tZq",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR_tWZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR_ttX",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR_triBoson", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF_ttZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF_WZ",       "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF_ZZ",       "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF_tZq",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF_tWZ",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF_ttX",      "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF_triBoson", "shape", SystMap<>::init(1.00));

  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "ISR", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "FSR", "shape", SystMap<>::init(1.00));

  // Rate uncertainties signals
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "rate_ttZ", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "rate_WZ", "shape", SystMap<>::init(1.00));
  cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "rate_ZZ", "shape", SystMap<>::init(1.00));

  // Rate uncertainties backgrounds
  cb.cp().process({"ttX"}).AddSyst(cb, "rate_ttX", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"triBoson"}).AddSyst(cb, "rate_triBoson", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"tWZ"}).AddSyst(cb, "rate_tWZ", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"tZq"}).AddSyst(cb, "rate_tZq", "lnN", SystMap<>::init(1.033));
  // ttZ, tZq and tWZ numbers from https://link.springer.com/content/pdf/10.1007/JHEP12(2021)083.pdf
  // ZZ and WZ from AN

  vector<string> theoryUncerts = {"PDF", "ISR", "FSR", "muR_ttZ","muR_WZ","muR_ZZ","muR_tZq","muR_tWZ","muR_ttX","muR_triBoson","muF_ttZ","muF_WZ","muF_ZZ","muF_tZq","muF_tWZ","muF_ttX","muF_triBoson"};
  vector<string> rateSigUncerts = {"rate_ttZ", "rate_WZ", "rate_ZZ"};
  vector<string> rateBkgUncerts = {"rate_ttX", "rate_triBoson", "rate_tWZ", "rate_tZq"};
  vector<string> nonpromptUncerts = {"Fakerate", "rate_nonprompt"};
  vector<string> lumiUncerts = {"Lumi_uncorrelated_2016","Lumi_uncorrelated_2017","Lumi_uncorrelated_2018","Lumi_correlated_161718","Lumi_correlated_1718"};
  vector<string> btagUncerts = {"BTag_b_correlated","BTag_l_correlated","BTag_b_uncorrelated_2016preVFP","BTag_l_uncorrelated_2016preVFP","BTag_b_uncorrelated_2016","BTag_l_uncorrelated_2016","BTag_b_uncorrelated_2017","BTag_l_uncorrelated_2017","BTag_b_uncorrelated_2018","BTag_l_uncorrelated_2018"};
  vector<string> leptonUncerts = {"LepReco", "LepIDsys","LepIDstat_2016preVFP","LepIDstat_2016","LepIDstat_2017","LepIDstat_2018"};
  vector<string> jetUncerts = {"JES", "JER"};
  vector<string> otherExpUncerts = {"Trigger", "PU", "Prefire"};

  cb.cp().SetGroup("theory", theoryUncerts);
  cb.cp().SetGroup("rate_sig", rateSigUncerts);
  cb.cp().SetGroup("rate_bkg", rateBkgUncerts);
  cb.cp().SetGroup("nonprompt", nonpromptUncerts);
  cb.cp().SetGroup("lumi", lumiUncerts);
  cb.cp().SetGroup("btag", btagUncerts);
  cb.cp().SetGroup("lepton", leptonUncerts);
  cb.cp().SetGroup("jet", jetUncerts);
  cb.cp().SetGroup("other_exp", otherExpUncerts);


  string dir = "../../../../../../../groups/hephy/cms/dennis.schwarz/www/tWZ/CombineInput_UL_threePoint_noData/"+year+"/"; // relative link from this dir
  if(light) dir = "../../../../../../../groups/hephy/cms/dennis.schwarz/www/tWZ/CombineInput_UL_threePoint_light_noData/"+year+"/"; // relative link from this dir
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
