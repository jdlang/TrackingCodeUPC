#include "RiceStyle.h"
#include <vector>

void double_plot_pt_eta(
    vector<float> ptMinVec,     // Vector of min pT cuts
    vector<float> etaMaxVec,    // Vector of max eta cuts
    string plotTitle,
    string fileLabel,
    string dirOut,
    string plotRootName,        // Base name of plots for ROOT file
    TFile* rootIn,              // ROOT input file
    TFile* rootOut,             // ROOT output file
    TH2F* hVar,                 // y-axis variable
    TH2F* hTrk,                 // x-axis variable (usually hSim or hRec)
    float plotMaxY = 1.0,       // maximum of plot y-axis (between 0. and 1.)
    bool exclusiveBins = true,  // use exclusive binning (no overlap)
    float ptPlotMin  = 0.1,     // min pT for x-axis
    float ptPlotMax  = 10.0,    // max pT for x-axis
    float etaPlotMax = 3.0,     // max +/- eta for x-axis
    float legX = 0.15,          // left edge of plot legend
    float legY = 0.40           // vertical midpoint of plot legend
) {
    // Function variables
    vector<int> markColors = {845,865,855,895,805,795};
    vector<int> markStyles = { 24, 25, 27, 28, 26, 32};
    vector<float> markSize = {1.0,0.9,1.5,0.9,1.1,1.1};
    vector<int> lineStyles = {  1,  1,  1,  1,  1,  1};
    
    // BUILD CANVAS AND MAIN ELEMENTS ----------
    
    // Build Canvas
    TCanvas* canvas = makeMultiCanvas( fileLabel.c_str(), plotTitle.c_str(), 3, 1 );
    
    // Build Legends
    TLegend* legPt1 = new TLegend(
            legX,
            legY + 0.5 * (0.05 * etaMaxVec.size()),
            legX + 0.35,
            legY - + 0.5 * (0.05 * etaMaxVec.size())
        );
        legPt1->SetFillColor(0);
        legPt1->SetFillStyle(0);
        legPt1->SetBorderSize(0);
        legPt1->SetTextSize(0.035);
    TLegend* legPt2 = (TLegend*) legPt1->Clone();
    TLegend* legEta = new TLegend(
            legX,
            legY + 0.5 * (0.05 * ptMinVec.size()),
            legX + 0.35,
            legY - + 0.5 * (0.05 * ptMinVec.size())
        );
        legEta->SetFillColor(0);
        legEta->SetFillStyle(0);
        legEta->SetBorderSize(0);
        legEta->SetTextSize(0.035);
    // Build Dummy Plots
    TH1D* hPtDummy1 = new TH1D("hPtDummy", ";p_{T} [GeV/c]",80,ptPlotMin,ptPlotMax);
        hPtDummy1->GetYaxis()->CenterTitle();
        hPtDummy1->GetYaxis()->SetTitleOffset(1.8);
        hPtDummy1->GetYaxis()->SetRangeUser(0.0, plotMaxY);
        hPtDummy1->GetYaxis()->SetTitle(plotTitle.c_str());
        hPtDummy1->SetFillStyle(0);
    TH1D* hPtDummy2 = (TH1D*) hPtDummy1->Clone();
        hPtDummy2->GetYaxis()->CenterTitle();
        hPtDummy2->GetYaxis()->SetTitleOffset(1.8);
        hPtDummy2->GetYaxis()->SetRangeUser(0.0, plotMaxY);
        hPtDummy2->GetYaxis()->SetTitle(plotTitle.c_str());
        hPtDummy2->SetFillStyle(0);
    TH1D* hEtaDummy = new TH1D("hEtaDummy",";#eta",60,-etaPlotMax,etaPlotMax);
        hEtaDummy->GetYaxis()->CenterTitle();
        hEtaDummy->GetYaxis()->SetTitleOffset(1.8);
        hEtaDummy->GetYaxis()->SetRangeUser(0.0, plotMaxY);
        hEtaDummy->GetYaxis()->SetTitle(plotTitle.c_str());
        hEtaDummy->SetFillStyle(0);

    
    // PAD 1: PT PLOTS (iterates over eta cuts) ----------
    
    canvas->cd(1);
    hPtDummy1->Draw();
    for( int i = 0 ; i < etaMaxVec.size() ; i++ ) {
        float etaCutLo = 0.0;
        if( exclusiveBins && (i > 0) ) etaCutLo = etaMaxVec[i-1];
        float etaCutHi = etaMaxVec[i];
        
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << etaCutLo;
        string etaLoStr = stream.str(); stream.str("");
        stream << std::fixed << std::setprecision(1) << etaCutHi;
        string etaHiStr = stream.str(); stream.str("");
        string plotName = plotRootName + "_vs_pT_etaCutM_" + etaLoStr + "-" + etaHiStr;
        
        float etaBinMLo = hTrk->GetXaxis()->FindBin(-etaCutHi + 0.001);
        float etaBinMHi = hTrk->GetXaxis()->FindBin(-etaCutLo - 0.001);
        
        // pT for negative and positive eta bins
        TH1D* hVarPt1 = (TH1D*) hVar->ProjectionY("hVarPt1",etaBinMLo,etaBinMHi,"e");
        TH1D* hTrkPt1 = (TH1D*) hTrk->ProjectionY("hTrkPt1",etaBinMLo,etaBinMHi,"e");
        
        int j = i % ptMinVec.size(); // protects from # bins > # styles
        TGraphAsymmErrors* gPt1 = new TGraphAsymmErrors();
            gPt1->SetName((plotName + "_1").c_str());
            gPt1->Divide(hVarPt1, hTrkPt1);
            gPt1->SetMarkerColor(markColors[j]);
            gPt1->SetMarkerStyle(markStyles[j]);
            gPt1->SetMarkerSize(markSize[j]);
            gPt1->SetLineColor(markColors[j]);
            gPt1->SetLineStyle(lineStyles[j]);
        
        if( exclusiveBins ) legPt1->AddEntry(gPt1, ("-" + etaHiStr + " < #eta < -" + etaLoStr).c_str(), "lp");
        else legPt1->AddEntry(gPt1, ("#eta < " + etaHiStr).c_str(), "lp");
        
        gPt1->Draw("pc");
        
        rootOut->cd();
        gPt1->Write();
        rootIn->cd();
    }
    legPt1->Draw();
    gPad->SetTicks();
//    canvas->GetPad(1)->SetLeftMargin(0.12);
//    canvas->GetPad(1)->SetBottomMargin(0.13);
//    canvas->GetPad(1)->SetLogx(1);
    
    canvas->cd(2);
    hPtDummy2->Draw();
    for( int i = 0 ; i < etaMaxVec.size() ; i++ ) {
        float etaCutLo = 0.0;
        if( exclusiveBins && (i > 0) ) etaCutLo = etaMaxVec[i-1];
        float etaCutHi = etaMaxVec[i];
        
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << etaCutLo;
        string etaLoStr = stream.str(); stream.str("");
        stream << std::fixed << std::setprecision(1) << etaCutHi;
        string etaHiStr = stream.str(); stream.str("");
        string plotName = plotRootName + "_vs_pT_etaCutP_" + etaLoStr + "-" + etaHiStr;
        
        int etaBinPLo =  hTrk->GetXaxis()->FindBin(etaCutLo + 0.001);
        int etaBinPHi =  hTrk->GetXaxis()->FindBin(etaCutHi - 0.001);
        
        // pT for negative and positive eta bins
        TH1D* hVarPt2 = (TH1D*) hVar->ProjectionY("hVarPt2",etaBinPLo,etaBinPHi,"e");
        TH1D* hTrkPt2 = (TH1D*) hTrk->ProjectionY("hTrkPt2",etaBinPLo,etaBinPHi,"e");
        
        int j = i % ptMinVec.size(); // protects from # bins > # styles
        TGraphAsymmErrors* gPt2 = new TGraphAsymmErrors();
            gPt2->SetName((plotName + "_2").c_str());
            gPt2->Divide(hVarPt2, hTrkPt2);
            gPt2->SetMarkerColor(markColors[j]);
            gPt2->SetMarkerStyle(markStyles[j]);
            gPt2->SetMarkerSize(markSize[j]);
            gPt2->SetLineColor(markColors[j]);
            gPt2->SetLineStyle(lineStyles[j]);
        
        if( exclusiveBins ) legPt2->AddEntry(gPt2, (etaLoStr + " < #eta < " + etaHiStr).c_str(), "lp");
        else legPt2->AddEntry(gPt2, ("#eta > -" + etaHiStr).c_str(), "lp");
        
        gPt2->Draw("pc");
        
        rootOut->cd();
        gPt2->Write();
        rootIn->cd();
    }
    legPt2->Draw();
    gPad->SetTicks();
    
    // PAD 2: ETA PLOTS (iterates over pT cuts) ----------
    
    canvas->cd(3);
    hEtaDummy->Draw();
    for( int i = 0 ; i < ptMinVec.size() ; i++ ) {
        float ptCutLo = ptMinVec[i];
        float ptCutHi = ptPlotMax;
        if( exclusiveBins && (i+1 < ptMinVec.size()) ) ptCutHi = ptMinVec[i+1];
        
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << ptCutLo;
        string ptLoStr = stream.str(); stream.str("");
        stream << std::fixed << std::setprecision(1) << ptCutHi;
        string ptHiStr = stream.str(); stream.str("");
        string plotName = plotRootName + "_vs_eta_ptCut_" + ptLoStr + "-" + ptHiStr;

        int ptBinLo = hTrk->GetYaxis()->FindBin(ptCutLo + 0.01);
        int ptBinHi = hTrk->GetYaxis()->FindBin(ptCutHi - 0.01);
        
        TH1D* hVarEta = (TH1D*) hVar->ProjectionX("hVarEta",ptBinLo,ptBinHi,"e");
        TH1D* hTrkEta = (TH1D*) hTrk->ProjectionX("hTrkEta",ptBinLo,ptBinHi,"e");
        
        int j = i % ptMinVec.size(); // protects from # bins > # styles
        TGraphAsymmErrors* gEta = new TGraphAsymmErrors();
            gEta->SetName(plotName.c_str());
            gEta->Divide(hVarEta, hTrkEta);
            gEta->SetMarkerColor(markColors[j]);
            gEta->SetMarkerStyle(markStyles[j]);
            gEta->SetMarkerSize(markSize[j]);
            gEta->SetLineColor(markColors[j]);
            gEta->SetLineStyle(lineStyles[j]);
        
        if( exclusiveBins ) legEta->AddEntry(gEta, (ptLoStr + " < p_{T} < " + ptHiStr + " GeV/c").c_str(), "lp");
        else legEta->AddEntry(gEta, ("p_{T} > " + ptLoStr + " GeV/c").c_str(), "lp");
        
        gEta->Draw("pc");
        
        rootOut->cd();
        gEta->Write();
        rootIn->cd();
    }
    legEta->Draw();
    gPad->SetTicks();
    
    rootOut->cd();
    canvas->Write();
    rootIn->cd();
    
    saveCanvas(canvas, dirOut, fileLabel.c_str());
}



void make_plots_2D(
    string fileIn,
    string dirOut,
    string fileOut,
    vector<float> ptMinVec,
    vector<float> etaMaxVec,
    bool  exclusiveBins = true,
    float ptPlotMin = 0.1,  // min domain for pt plots
    float ptPlotMax = 10.0, // max pT value for plots
    float etaPlotMax = 3.0  // max eta for plots
) {
    
    // Convert string arguments
    std::filesystem::create_directories(dirOut.c_str());
    
    // Internal parameters
    char  ndir[256] = "HITrackCorrections";
    float linMin = 0.0; // minimum value for linear plots
    float legX   = 0.15; // left edge of plot legend
    float legY   = 0.50; // vertical midpoint of plot legend
    float effMaxY = 1.00; // absolute efficiency y-max
    float effLegY = 0.30; // vertical midpoint of plot legend
    float secMaxY = 0.20; // secondary y-max
    float secLegY = 0.75; // vertical midpoint of plot legend
    float mulMaxY = 0.05; // multiplicity y-max
    float mulLegY = 0.75; // vertical midpoint of plot legend
    float fakMaxY = 0.20; // fake track y-max
    float fakLegY = 0.75; // vertical midpoint of plot legend

    RiceStyle();
    gStyle->SetOptStat(0);
    
    // Read in file and histograms
    TFile* fin = new TFile(fileIn.c_str(), "READ");
    TFile* fout = new TFile((dirOut + fileOut).c_str(),"RECREATE");
    
    // Sim-to-Reco: Simulated tracks, Efficiency fraction,Multiple reconstruction fraction
    TH2F* hSim = (TH2F*) fin->Get(Form("%s/hsim",ndir)); hSim->GetYaxis()->SetRangeUser(ptPlotMin,ptPlotMax);
    TH2F* hEff = (TH2F*) fin->Get(Form("%s/heff",ndir)); hEff->GetYaxis()->SetRangeUser(ptPlotMin,ptPlotMax);
    TH2F* hMul = (TH2F*) fin->Get(Form("%s/hmul",ndir)); hMul->GetYaxis()->SetRangeUser(ptPlotMin,ptPlotMax);
    // Reco-to-Sim: Reco tracks, Fake fraction, Secondary fraction
    TH2F* hRec = (TH2F*) fin->Get(Form("%s/hrec",ndir)); hRec->GetYaxis()->SetRangeUser(ptPlotMin,ptPlotMax);
    TH2F* hFak = (TH2F*) fin->Get(Form("%s/hfak",ndir)); hFak->GetYaxis()->SetRangeUser(ptPlotMin,ptPlotMax);
    TH2F* hSec = (TH2F*) fin->Get(Form("%s/hsec",ndir)); hSec->GetYaxis()->SetRangeUser(ptPlotMin,ptPlotMax);
    
    // Ratio histograms
    TH2F* rEff = (TH2F*) hEff->Clone("rEff");
    TH2F* rMul = (TH2F*) hMul->Clone("rMul");
    TH2F* rFak = (TH2F*) hFak->Clone("rFak");
    TH2F* rSec = (TH2F*) hSec->Clone("rSec");

    // RECONSTRUCTION FRACTION PLOTS --------------------
    
    // Reco efficiency fraction
    TCanvas *c2 = new TCanvas("c2","Reco Efficiency Fraction",500,500);
    c2->SetLogy(1);
    gPad->SetRightMargin(0.15);
    rEff->Divide(hEff,hSim,1,1,"B");
    rEff->SetStats(0);
    rEff->SetMaximum(effMaxY); rEff->SetMinimum(linMin);
    rEff->SetTitle("Absolute Efficiency");
    rEff->Draw("colz");
    rEff->Write();
    saveCanvas(c2, dirOut, "Ratio_Efficiency");

    // Multiple reco fraction
    TCanvas *c3 = new TCanvas("c3","Multiple Reco Fraction",500,500);
    c3->SetLogy(1);
    gPad->SetRightMargin(0.15);
    rMul->Divide(hMul,hSim,1,1,"B");
    rMul->SetStats(0);
    rMul->SetMaximum(mulMaxY); rMul->SetMinimum(linMin);
    rMul->SetTitle("Multiple Reconstruction Fraction");
    rMul->Draw("colz");
    rMul->Write();
    saveCanvas(c3, dirOut, "Ratio_MultiReco");

    // Fake reco fraction
    TCanvas *c4 = new TCanvas("c4","Fake Reco Fraction",500,500);
    c4->SetLogy(1);
    gPad->SetRightMargin(0.15);
    rFak->Divide(hFak,hRec,1,1,"B");
    rFak->SetStats(0);
    rFak->SetMaximum(fakMaxY); rFak->SetMinimum(linMin);
    rFak->SetTitle("Fake Reconstruction Fraction");
    rFak->Draw("colz");
    rFak->Write();
    saveCanvas(c4, dirOut, "Ratio_FakeReco");

    // Secondary reco fraction
    TCanvas *c5 = new TCanvas("c5","Secondary Fraction",500,500);
    c5->SetLogy(1);
    gPad->SetRightMargin(0.15);
    rSec->Divide(hSec,hRec,1,1,"B");
    rSec->SetStats(0);
    rSec->SetMaximum(secMaxY); rSec->SetMinimum(linMin);
    rSec->SetTitle("Non-Primary Reconstruction Fraction");
    rSec->Draw("colz");
    rSec->Write();
    saveCanvas(c5, dirOut, "Ratio_SecondReco");
    
    // Plots vs. pT with multiple eta cuts
    
    // Efficiency
    double_plot_pt_eta(
        ptMinVec, // vector<float> ptMinVec
        etaMaxVec, // vector<float> etaMaxVec
        "Absolute Efficiency", // string plotTitle
        "Double_AbsoluteEfficiency", // string fileLable
        dirOut, // string dirOut
        "gEfficiency", // string plotRootName
        fin,  // TFile* rootIn
        fout, // TFile* rootOut
        hEff, // TH2F* hVar
        hSim, // TH2F* hTrk
        effMaxY, // float plotMaxY
        exclusiveBins, // bool exclusiveBins
        ptPlotMin, // float ptPlotMin
        ptPlotMax, // float ptPlotMax
        etaPlotMax, // float etaPlotMax
        legX, // float legX
        effLegY // float legY
    );
    
    // Multiple Tracks
    double_plot_pt_eta(
        ptMinVec, // vector<float> ptMinVec
        etaMaxVec, // vector<float> etaMaxVec
        "Multiple Reconstruction Fraction", // string plotTitle
        "Double_MultipleRecoFraction", // string fileLable
        dirOut, // string dirOut
        "gMultiReco", // string plotRootName
        fin,  // TFile* rootIn
        fout, // TFile* rootFile
        hMul, // TH2F* hVar
        hSim, // TH2F* hTrk
        mulMaxY, // float plotMaxY
        exclusiveBins, // bool exclusiveBins
        ptPlotMin, // float ptPlotMin
        ptPlotMax, // float ptPlotMax
        etaPlotMax, // float etaPlotMax
        legX, // float legX
        mulLegY // float legY
    );
    
    // Fake Tracks
    double_plot_pt_eta(
        ptMinVec, // vector<float> ptMinVec
        etaMaxVec, // vector<float> etaMaxVec
        "Fake Reconstruction Fraction", // string plotTitle
        "Double_FakeRecoFraction", // string fileLable
        dirOut, // string dirOut
        "gFakeReco", // string plotRootName
        fin,  // TFile* rootIn
        fout, // TFile* rootFile
        hFak, // TH2F* hVar
        hRec, // TH2F* hTrk
        fakMaxY, // float plotMaxY
        exclusiveBins, // bool exclusiveBins
        ptPlotMin, // float ptPlotMin
        ptPlotMax, // float ptPlotMax
        etaPlotMax, // float etaPlotMax
        legX, // float legX
        fakLegY // float legY
    );
    
    // Secondary Tracks
    double_plot_pt_eta(
        ptMinVec, // vector<float> ptMinVec
        etaMaxVec, // vector<float> etaMaxVec
        "Non-Primary Reconstruction Fraction", // string plotTitle
        "Double_SecondaryRecoFraction", // string fileLable
        dirOut, // string dirOut
        "gSecondReco", // string plotRootName
        fin,  // TFile* rootIn
        fout, // TFile* rootFile
        hSec, // TH2F* hVar
        hRec, // TH2F* hTrk
        secMaxY, // float plotMaxY
        exclusiveBins, // bool exclusiveBins
        ptPlotMin, // float ptPlotMin
        ptPlotMax, // float ptPlotMax
        etaPlotMax, // float etaPlotMax
        legX, // float legX
        secLegY // float legY
    );
        
    // SIM & RECO HISTOGRAMS --------------------
    
    fout->cd();
    
    // Simulated Tracks
    TCanvas *c0 = new TCanvas("c0","Simulated Tracks",500,500);
    c0->SetLogy(1);
    gPad->SetRightMargin(0.15);
    hSim->SetStats(0);
    hSim->SetMinimum(linMin);
    hSim->SetTitle("Simulated Tracks");
    hSim->Draw("colz");
    hSim->Write();
    saveCanvas(c0, dirOut, "2DHist_SimulatedTracks");
    
    // Reco Tracks
    TCanvas *c1 = new TCanvas("c0","Reconstructed Tracks",500,500);
    c1->SetLogy(1);
    gPad->SetRightMargin(0.15);
    hRec->SetStats(0);
    hRec->SetMinimum(linMin);
    hRec->SetTitle("Reconstructed Tracks");
    hRec->Draw("colz");
    hRec->Write();
    saveCanvas(c1, dirOut, "2DHist_RecoTracks");
    
    fout->Close();
}



// This seems to be unused
void set_plot_style() {
    // nicer colz plots

    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}



void plotHists() {
    // Vector of min pT cuts
    vector<float> ptMinVec  = {0.1, 0.3, 0.5, 1.0, 3.0};
    // Vector of max |eta| cuts
    vector<float> etaMaxVec = {1.0, 2.4, 3.0};
    
    vector<vector<string>> fileNames = {
        {
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/TrackAnalyzer_single_diff_STARLIGHT_UPC_2024_01_19.root",
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/AndreRootOutput/",
            "_plots.root"
        },
        {
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/trk_GeneralTrack_6k.root",
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/RootOutput/",
            "_plots.root"
        }
    };
    for( int i=0 ; i < fileNames.size() ; i++ ) {
        cout << "\nMAKING 2D PLOTS...\n\n" << "fileIn:  " << fileNames[i][0].c_str() << "\ndirOut:  " << fileNames[i][1].c_str() << "\nfileOut: " << fileNames[i][2].c_str() << endl;
        make_plots_2D(
            fileNames[i][0],
            fileNames[i][1],
            fileNames[i][2],
            ptMinVec,
            etaMaxVec
        );
    }
}
