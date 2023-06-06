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

int main() {
  int nPT = 4;
  int nETA = 3; 
  vector<string> binsPT, binsETA;
  for(int i=1; i<=nPT; i++)  binsPT.push_back( std::to_string(i) );
  for(int i=1; i<=nETA; i++) binsETA.push_back( std::to_string(i) );
  vector<string> WPs = {"LOOSE", "TIGHT"};
  vector<string> years = {"UL2016preVFP", "UL2016", "UL2017", "UL2018"};
  vector<string> channels = {"muon", "elec"};
  
  
  
  vector<string> ListOfFitHists;
  for(auto year: years ){
    for(auto channel: channels ){
      for(auto ptbin: binsPT ){
        for(auto etabin: binsETA ){
          for(auto WP: WPs ){
            string histname = year+"_"+channel+"_PT"+ptbin+"_ETA"+etabin+"_"+WP;
            ListOfFitHists.push_back(histname);
          }
        }
      }
    }
  }

  for(auto histname: ListOfFitHists){
    string mass = "0";
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
      {1, "singlelep"}
    };
    // ch::Categories is just a typedef of vector<pair<int, string>>
    
    //bkg_procs
    vector<string> bkg_procs = {"prompt"};
    //signal
    vector<string>  sig_procs = {"nonprompt"}; 
    
    cb.AddObservations({"*"}, {"Fakerate"}, {"13TeV"}, {histname}, cats);
    cb.AddProcesses({"*"},  {"Fakerate"}, {"13TeV"}, {histname}, bkg_procs, cats, false);
    cb.AddProcesses({"*"},  {"Fakerate"}, {"13TeV"}, {histname}, sig_procs, cats, true);
    
    //Some of the code for this is in a nested namespace, so
    // we'll make some using declarations first to simplify things a bit.
    using ch::syst::SystMap;
    using ch::syst::era;
    using ch::syst::bin_id;
    using ch::syst::process;
    
        
    // Rate uncertainties backgrounds
    cb.cp().process({"prompt"})
    .AddSyst(cb, "prompt_rate", "lnN", SystMap<>::init(1.2));
    
    // cb.cp().process({"nonprompt"})
    // .AddSyst(cb, "nonprompt_rate", "lnN", SystMap<>::init(2.0));
    
    
    string dir = "../../../../../../../groups/hephy/cms/dennis.schwarz/www/tWZ/Fakerate/CombineInput_tunePtCone/"; // relative link from this dir
    string input_filename = dir+"FakeRate_"+histname+".root";
    
    
    cb.cp().backgrounds().ExtractShapes(
      aux_shapes + input_filename,
      "$BIN/$PROCESS",
      "$BIN/$PROCESS__$SYSTEMATIC");
    cb.cp().signals().ExtractShapes(
      aux_shapes + input_filename,
      "$BIN/$PROCESS",
      "$BIN/$PROCESS__$SYSTEMATIC");
        
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
    TString filename = "Fakerate_"+histname+"_"+mass+".input.root";
    TFile output(filename, "RECREATE");
    
    // Finally we iterate through each bin,mass combination and write a
    // datacard.
    for (auto b : bins) {
      cout << ">> Writing datacard for bin: " << b << " and mass: " << mass
      << "\n";
      // We need to filter on both the mass and the mass hypothesis,
      // where we must remember to include the "*" mass entry to get
      // all the data and backgrounds.
      cb.cp().bin({b}).mass({mass, "*"}).WriteDatacard(
        b + "_" + mass + ".txt", output);
    }
  }
}
