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

  if(argc < 3){
    cout << "[ERROR] Wrong usage you have to provide a minimum of 2 arguiments, only " << argc-1 << " provided." <<endl;
    cout << "CreateCards_topEFT year WCname [statOnly/noRates/noScales]" <<endl;
    return 0;
  }

  string year;
  if(strcmp(argv[1], "UL2016preVFP") == 0)  year = "UL2016preVFP";
  else if(strcmp(argv[1], "UL2016") == 0)   year = "UL2016";
  else if(strcmp(argv[1], "UL2017") == 0)   year = "UL2017";
  else if(strcmp(argv[1], "UL2018") == 0)   year = "UL2018";
  else{
    cout << "[ERROR] year " << argv[1] << " not known. Select a valid year [UL2016preVFP, UL2016, UL2017, UL2018]" <<endl;
    return 0;
  }

  string WCname;
  if(strcmp(argv[2], "cHq1Re11") == 0)        WCname = "cHq1Re11";
  else if(strcmp(argv[2], "cHq1Re22") == 0)   WCname = "cHq1Re22";
  else if(strcmp(argv[2], "cHq1Re33") == 0)   WCname = "cHq1Re33";
  else if(strcmp(argv[2], "cHq3Re11") == 0)   WCname = "cHq3Re11";
  else if(strcmp(argv[2], "cHq3Re22") == 0)   WCname = "cHq3Re22";
  else if(strcmp(argv[2], "cHq3Re33") == 0)   WCname = "cHq3Re33";
  else{
    cout << "[ERROR] Wilson coefficient " << argv[2] << " not known. Select a valid WC [cHq1Re11, cHq1Re22, cHq1Re33, cHq3Re11, cHq3Re22, cHq3Re33]" <<endl;
    return 0;
  }

  bool statOnly = false;
  bool noRates = false;
  bool noScales = false;
  bool noBtag = false;
  bool noJEC = false;
  bool noLepton = false;
  bool noLumi = false;
  bool noPS = false;
  bool noFakerate = false;

  if(argc > 3){
    if(strcmp(argv[3], "statOnly") == 0)        statOnly = true;
    else if(strcmp(argv[3], "noRates") == 0)    noRates = true;
    else if(strcmp(argv[3], "noScales") == 0)   noScales = true;
    else if(strcmp(argv[3], "noBtag") == 0)     noBtag = true;
    else if(strcmp(argv[3], "noJEC") == 0)      noJEC = true;
    else if(strcmp(argv[3], "noLepton") == 0)   noLepton = true;
    else if(strcmp(argv[3], "noLumi") == 0)     noLumi = true;
    else if(strcmp(argv[3], "noPS") == 0)       noPS = true;
    else if(strcmp(argv[3], "noFakerate") == 0) noFakerate = true;
  }

  vector<string> WCvals;
  int nEFTs = 21;
  for(int ieft=0; ieft<nEFTs; ieft++){
    std::string sx = std::to_string(ieft);
    WCvals.push_back(sx);
  }

  for(auto WCpoint: WCvals ){
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
    vector<string> bkg_procs = {"tWZ", "ttZ", "ttX", "tZq", "ZZ", "triBoson", "nonprompt"};
    //signal
    vector<string>  sig_procs = {"WZ"}; // arbitrary define WZ as signal

    cb.AddObservations({"*"}, {"topEFT"}, {"13TeV"}, {WCname}, cats);
    cb.AddProcesses({"*"},  {"topEFT"}, {"13TeV"}, {WCname}, bkg_procs, cats, false);
    cb.AddProcesses({"*"},  {"topEFT"}, {"13TeV"}, {WCname}, sig_procs, cats, true);

    //Some of the code for this is in a nested namespace, so
    // we'll make some using declarations first to simplify things a bit.
    using ch::syst::SystMap;
    using ch::syst::era;
    using ch::syst::bin_id;
    using ch::syst::process;

    // Shape uncertainties
    if(!statOnly){
      if(!noBtag){
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
      }
      cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Trigger", "shape", SystMap<>::init(1.00));
      cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "PU", "shape", SystMap<>::init(1.00));

      if(!noJEC){
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "JES", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "JER", "shape", SystMap<>::init(1.00));
      }

      if(!noFakerate){
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Fakerate", "shape", SystMap<>::init(1.00));
        cb.cp().process({"nonprompt"}).AddSyst(cb, "nonprompt_rate", "lnN", SystMap<>::init(1.3)); //Flat 30%
      }

      cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Prefire", "shape", SystMap<>::init(1.00));

      if(!noLepton){
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepReco", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDsys", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2016preVFP", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2016", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2017", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "LepIDstat_2018", "shape", SystMap<>::init(1.00));
      }

      if(!noLumi){
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_uncorrelated_2016", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_uncorrelated_2017", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_uncorrelated_2018", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_correlated_161718", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "Lumi_correlated_1718", "shape", SystMap<>::init(1.00));
      }

      cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "PDF", "shape", SystMap<>::init(1.00));

      if(!noScales){
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muR", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "muF", "shape", SystMap<>::init(1.00));
      }

      if(!noPS){
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "ISR", "shape", SystMap<>::init(1.00));
        cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "FSR", "shape", SystMap<>::init(1.00));
      }

      if(!noRates){
        // Rate uncertainties backgrounds
        cb.cp().process({"ttX"}).AddSyst(cb, "ttX_rate", "lnN", SystMap<>::init(1.2));

        cb.cp().process({"triBoson"}).AddSyst(cb, "triBoson_rate", "lnN", SystMap<>::init(1.2));

        // Rate uncertainties signal
        // ttZ, tZq and tWZ numbers from https://link.springer.com/content/pdf/10.1007/JHEP12(2021)083.pdf
        // ZZ and WZ from AN
        cb.cp().process({"tWZ"}).AddSyst(cb, "tWZ_rate", "lnN", SystMap<>::init(1.2));

        cb.cp().process({"tZq"}).AddSyst(cb, "tZq_rate", "lnN", SystMap<>::init(1.033));

        cb.cp().process({"ttZ"}).AddSyst(cb, "ttZ_rate", "lnN", SystMap<>::init(1.11));

        cb.cp().process({"ZZ"}).AddSyst(cb, "ZZ_rate", "lnN", SystMap<>::init(1.025));

        cb.cp().process({"WZ"}).AddSyst(cb, "WZ_rate", "lnN", SystMap<>::init(1.025));

      }
    }
    else{
      // use a dummy uncertainty if running only on stat
      cb.cp().process( ch::JoinStr({sig_procs, bkg_procs}) ).AddSyst(cb, "dummy", "lnN", SystMap<>::init(1.001));
    }


    string dir = "../../../../../../../groups/hephy/cms/dennis.schwarz/www/tWZ/CombineInput_UL_noData/"+year+"/"; // relative link from this dir
    string input_filename = dir+"CombineInput_"+WCname+"_"+WCpoint+".root";


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
    TString filename = "topEFT_"+WCname+"_"+WCpoint+".input.root";
    TFile output(filename, "RECREATE");

    // Finally we iterate through each bin,mass combination and write a
    // datacard.
    for (auto b : bins) {
      cout << ">> Writing datacard for bin: " << b << " and mass: " << WCpoint
      << "\n";
      // We need to filter on both the mass and the mass hypothesis,
      // where we must remember to include the "*" mass entry to get
      // all the data and backgrounds.
      cb.cp().bin({b}).mass({WCpoint, "*"}).WriteDatacard(
        b + "_" + WCpoint + ".txt", output);
    }
  }
}
