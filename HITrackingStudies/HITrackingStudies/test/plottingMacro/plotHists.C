#include "RiceStyle.h"
#include <vector>

void make_plots_2D(
    string fileIn,
    string dirOut,
    string fileOut,
    string ptMin1Str,
    string etaMax1Str,
    string ptMin2Str = "0",
    string etaMax2Str = "0"
) {
    
    // Convert string arguments
    std::filesystem::create_directories(dirOut.c_str());
    float ptMin1  = stof(ptMin1Str);
    float etaMax1 = stof(etaMax1Str);
    float ptMin2  = stof(ptMin2Str);
    float etaMax2 = stof(etaMax2Str);
    // Add second plot iff second pt and eta are provided
    bool  usePlot2 = false;
    if( (ptMin2 != 0) && (etaMax2 != 0) ) usePlot2 = true;
    
    // Internal parameters
    char ndir[256] = "HITrackCorrections";
    float logMin = 0.1; // minimum value for log plots
    float linMin = 0.0; // minimum value for linear plots
    float ptMax  = 50.; // max pT value for plots
    float legX   = 0.15; // left edge of plot legend
    float legY   = 0.50; // vertical midpoint of plot legend
    float effMax = 1.0; // absolute efficiency y-max
    float secMax = 0.5; // secondary y-max
    float mulMax = 0.1; // multiplicity y-max
    float fakMax = 0.5; // fake track y-max
    float etaMax  = etaMax1; // set max eta for plots
    if( etaMax2 > etaMax1 ) etaMax = etaMax2;

    RiceStyle();
    gStyle->SetOptStat(0);
    
    // Read in file and histograms
    TFile* f = new TFile(fileIn.c_str(), "READ");

    // sim-to-reco hists
    TH2F *hSim = (TH2F*) f->Get(Form("%s/hsim",ndir)); hSim->GetYaxis()->SetRangeUser(logMin,ptMax);
    TH2F *hEff = (TH2F*) f->Get(Form("%s/heff",ndir)); hEff->GetYaxis()->SetRangeUser(logMin,ptMax);
    TH2F *hMul = (TH2F*) f->Get(Form("%s/hmul",ndir)); hMul->GetYaxis()->SetRangeUser(logMin,ptMax);

    // reco-to-sim hists
    TH2F *hRec = (TH2F*) f->Get(Form("%s/hrec",ndir)); hRec->GetYaxis()->SetRangeUser(logMin,ptMax);
    TH2F *hFak = (TH2F*) f->Get(Form("%s/hfak",ndir)); hFak->GetYaxis()->SetRangeUser(logMin,ptMax);
    TH2F *hSec = (TH2F*) f->Get(Form("%s/hsec",ndir)); hSec->GetYaxis()->SetRangeUser(logMin,ptMax);

    // ratio histograms
    TH2F *rEff = (TH2F*) hEff->Clone("rEff");
    TH2F *rMul = (TH2F*) hMul->Clone("rMul");
    TH2F *rFak = (TH2F*) hFak->Clone("rFak");
    TH2F *rSec = (TH2F*) hSec->Clone("rSec");

    // RECONSTRUCTION FRACTION PLOTS --------------------

    // reco efficiency fraction
    TCanvas *c2 = new TCanvas("c2","Reco Efficiency Fraction",1000,500);
    //c2->SetLogy();
    gPad->SetRightMargin(0.15);
    rEff->Divide(hEff,hSim,1,1,"B");
    rEff->SetStats(0);
    rEff->SetMaximum(effMax); rEff->SetMinimum(linMin);
    rEff->SetTitle("Absolute Efficiency");
    rEff->Draw("colz");

    // multiple reco fraction
    TCanvas *c3 = new TCanvas("c3","Multiple Reco Fraction",1000,500);
    //c3->SetLogy();
    gPad->SetRightMargin(0.15);
    rMul->Divide(hMul,hSim,1,1,"B");
    rMul->SetStats(0);
    rMul->SetMaximum(mulMax); rMul->SetMinimum(linMin);
    rMul->SetTitle("Multiple Reconstruction Fraction");
    rMul->Draw("colz");

    // fake reco fraction
    TCanvas *c4 = new TCanvas("c4","Fake Reco Fraction",1000,500);
    //c4->SetLogy();
    gPad->SetRightMargin(0.15);
    rFak->Divide(hFak,hRec,1,1,"B");
    rFak->SetStats(0);
    rFak->SetMaximum(fakMax); rFak->SetMinimum(linMin);
    rFak->SetTitle("Fake Reconstruction Fraction");
    rFak->Draw("colz");

    // secondary reco fraction
    TCanvas *c5 = new TCanvas("c5","Secondary Fraction",1000,500);
    //c5->SetLogy();
    gPad->SetRightMargin(0.15);
    rSec->Divide(hSec,hRec,1,1,"B");
    rSec->SetStats(0);
    rSec->SetMaximum(secMax); rSec->SetMinimum(linMin);
    rSec->SetTitle("Non-Primary Reconstruction Fraction");
    rSec->Draw("colz");

    // find bins corresponding to projections for below
    Int_t ptBinMax1 = hSim->GetYaxis()->FindBin(ptMin1 + 0.01);
    Int_t ptBinMax2 = hSim->GetYaxis()->FindBin(ptMin2 + 0.01);
    Int_t ptBins    = hSim->GetYaxis()->GetNbins();
    Int_t etaBin1M  = hSim->GetXaxis()->FindBin(-etaMax1 + 0.01);
    Int_t etaBin1P  = hSim->GetXaxis()->FindBin(etaMax1 - 0.01);
    Int_t etaBin2M  = hSim->GetXaxis()->FindBin(-etaMax2 + 0.01);
    Int_t etaBin2P  = hSim->GetXaxis()->FindBin(etaMax2 - 0.01);
    
    // Plots vs. pT with multiple eta cuts
    
    // projected hists: pt < ptMin1
    TH1D* hSimEta1 = (TH1D*) hSim->ProjectionX("hSimEta1",ptBinMax1,ptBins,"e");
    TH1D* hEffEta1 = (TH1D*) hEff->ProjectionX("hEffEta1",ptBinMax1,ptBins,"e");
    TH1D* hMulEta1 = (TH1D*) hMul->ProjectionX("hMulEta1",ptBinMax1,ptBins,"e");
    TH1D* hRecEta1 = (TH1D*) hRec->ProjectionX("hRecEta1",ptBinMax1,ptBins,"e");
    TH1D* hFakEta1 = (TH1D*) hFak->ProjectionX("hFakEta1",ptBinMax1,ptBins,"e");
    TH1D* hSecEta1 = (TH1D*) hSec->ProjectionX("hSecEta1",ptBinMax1,ptBins,"e");
    // projected hists: pt < ptMin2
    TH1D* hSimEta2; TH1D* hEffEta2; TH1D* hMulEta2;
    TH1D* hRecEta2; TH1D* hFakEta2; TH1D* hSecEta2;
    if( usePlot2 ){
        hSimEta2 = (TH1D*) hSim->ProjectionX("hSimEta2",ptBinMax2,ptBins,"e");
        hEffEta2 = (TH1D*) hEff->ProjectionX("hEffEta2",ptBinMax2,ptBins,"e");
        hMulEta2 = (TH1D*) hMul->ProjectionX("hMulEta2",ptBinMax2,ptBins,"e");
        hRecEta2 = (TH1D*) hRec->ProjectionX("hRecEta2",ptBinMax2,ptBins,"e");
        hFakEta2 = (TH1D*) hFak->ProjectionX("hFakEta2",ptBinMax2,ptBins,"e");
        hSecEta2 = (TH1D*) hSec->ProjectionX("hSecEta2",ptBinMax2,ptBins,"e");
    }
    
    TH1D* hDumEta = new TH1D("hDumEta",";#eta",60,-etaMax,etaMax);
        hDumEta->SetMaximum(1.0);
        hDumEta->GetXaxis()->CenterTitle();
        hDumEta->GetYaxis()->SetTitleOffset(1.8);
    TH1D* hDumEta2 = (TH1D*) hDumEta->Clone("hDumEta2");
        hDumEta2->SetMaximum(1.0);
    TH1D* hDumEta3 = (TH1D*) hDumEta->Clone("hDumEta3");
        hDumEta3->SetMaximum(1.0);
    
    // Plots vs. eta with multiple pT cuts
    
    // projected hists: abs(eta) < etaMax1
    TH1D* hSimPt1 = (TH1D*) hSim->ProjectionY("hSimPt1",etaBin1M,etaBin1P,"e");
    TH1D* hEffPt1 = (TH1D*) hEff->ProjectionY("hEffPt1",etaBin1M,etaBin1P,"e");
    TH1D* hMulPt1 = (TH1D*) hMul->ProjectionY("hMulPt1",etaBin1M,etaBin1P,"e");
    TH1D* hRecPt1 = (TH1D*) hRec->ProjectionY("hRecPt1",etaBin1M,etaBin1P,"e");
    TH1D* hFakPt1 = (TH1D*) hFak->ProjectionY("hFakPt1",etaBin1M,etaBin1P,"e");
    TH1D* hSecPt1 = (TH1D*) hSec->ProjectionY("hSecPt1",etaBin1M,etaBin1P,"e");
    // projected hists: abs(eta) < etaMax2
    TH1D* hSimPt2; TH1D* hEffPt2; TH1D* hMulPt2;
    TH1D* hRecPt2; TH1D* hFakPt2; TH1D* hSecPt2;
    if( usePlot2 ){
        hSimPt2 = (TH1D*) hSim->ProjectionY("hSimPt2",etaBin2M,etaBin2P,"e");
        hEffPt2 = (TH1D*) hEff->ProjectionY("hEffPt2",etaBin2M,etaBin2P,"e");
        hMulPt2 = (TH1D*) hMul->ProjectionY("hMulPt2",etaBin2M,etaBin2P,"e");
        hRecPt2 = (TH1D*) hRec->ProjectionY("hRecPt2",etaBin2M,etaBin2P,"e");
        hFakPt2 = (TH1D*) hFak->ProjectionY("hFakPt2",etaBin2M,etaBin2P,"e");
        hSecPt2 = (TH1D*) hSec->ProjectionY("hSecPt2",etaBin2M,etaBin2P,"e");
    }

    TH1D* hDumPt = new TH1D("hDumPt",";p_{T} [GeV/c]",80,logMin,ptMax);
        hDumPt->SetMaximum(1.0);
        hDumPt->GetXaxis()->CenterTitle();
        hDumPt->GetYaxis()->SetTitleOffset(1.8);
    TH1D* hDumPt2 = (TH1D*) hDumPt->Clone("hDumPt2");
        hDumPt2->SetMaximum(1.0);
    TH1D* hDumPt3 = (TH1D*) hDumPt->Clone("hDumPt3");
        hDumPt3->SetMaximum(1.0);
    
    // ABSOLUTE EFFICIENCY FRACTION --------------------
    
    // Build TGraphs
    TGraphAsymmErrors* gEffEta1 = new TGraphAsymmErrors();
        gEffEta1->SetName("gEffEta1");
        gEffEta1->Divide(hEffEta1,hSimEta1); // WHAT IS THIS DOING?
        gEffEta1->SetMarkerColor(4);
        gEffEta1->SetMarkerStyle(24);
        gEffEta1->SetLineColor(4);
    TGraphAsymmErrors* gEffPt1 = new TGraphAsymmErrors();
        gEffPt1->SetName("gEffPt1");
        gEffPt1->Divide(hEffPt1,hSimPt1); // WHAT IS THIS DOING?
        gEffPt1->SetMarkerColor(4);
        gEffPt1->SetMarkerStyle(24);
        gEffPt1->SetLineColor(4);
    TGraphAsymmErrors* gEffEta2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gEffEta2->SetName("gEffEta2");
        gEffEta2->Divide(hEffEta2,hSimEta2); // WHAT IS THIS DOING?
        gEffEta2->SetMarkerColor(2);
        gEffEta2->SetMarkerStyle(25);
        gEffEta2->SetLineColor(2);
        gEffEta2->SetLineStyle(2);
    }
    TGraphAsymmErrors* gEffPt2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gEffPt2->SetName("gEffPt2");
        gEffPt2->Divide(hEffPt2,hSimPt2); // WHAT IS THIS DOING?
        gEffPt2->SetLineColor(2);
        gEffPt2->SetLineStyle(2);
        gEffPt2->SetMarkerColor(2);
        gEffPt2->SetMarkerStyle(25);
    }
    
    // Eta Legend
    TLegend* legEta = new TLegend(legX, legY+0.10, legX+0.35, legY-0.10);
        legEta->SetFillColor(0);
        legEta->SetFillStyle(0);
        legEta->SetBorderSize(0);
        legEta->SetTextSize(0.04);
        legEta->AddEntry(gEffEta1,("p_{T} > " + ptMin1Str + " GeV/c").c_str(),"lp"); // IS THIS BACKWARDS???
        if( usePlot2 ) legEta->AddEntry(gEffEta2,("p_{T} > " + ptMin2Str + " GeV/c").c_str(),"lp");
    // pT Legend
    TLegend* legPt = new TLegend(legX, legY+0.10, legX+0.35, legY-0.10);
        legPt->SetFillColor(0);
        legPt->SetFillStyle(0);
        legPt->SetBorderSize(0);
        legPt->SetTextSize(0.04);
        legPt->AddEntry(gEffPt1,("|#eta| < " + etaMax1Str).c_str(),"lp"); // IS THIS BACKWARDS???
        if( usePlot2 ) legPt->AddEntry(gEffPt2,("|#eta| < " + etaMax2Str).c_str(),"lp");
    TCanvas* c7 = makeMultiCanvas("c7", "Efficiency Fraction", 2,1 );
    // graph vs eta
    TH1F* hDumEtaEff = (TH1F*) hDumEta->Clone("hDumEtaEff");
    fixedFontHist1D(hDumEtaEff,1.05,1.2);
    hDumEtaEff->GetYaxis()->SetTitle("Absolute efficiency");
    // graph vs pt
    TH1F* hDumPtEff = (TH1F*) hDumPt->Clone("hDumPtEff");
    fixedFontHist1D(hDumPtEff,1.05,1.2);
    hDumPtEff->GetYaxis()->SetTitle("Absolute efficiency");
    // save canvas
    c7->cd(1);
        hDumEtaEff->Draw();
        gEffEta1->Draw("pc");
        if( usePlot2 ) gEffEta2->Draw("pc");
        legEta->Draw();
        gPad->SetTicks();
        c7->GetPad(1)->SetLeftMargin(0.12);
        c7->GetPad(1)->SetBottomMargin(0.13);
        c7->GetPad(1)->SetLogx(0);
    c7->cd(2);
        hDumPtEff->Draw();
        gEffPt1->Draw("pc");
        if( usePlot2 ) gEffPt2->Draw("pc");
        legPt->Draw();
        gPad->SetTicks();
        c7->GetPad(2)->SetLeftMargin(0.12);
        c7->GetPad(2)->SetBottomMargin(0.13);
        c7->GetPad(2)->SetLogx();
    saveCanvas(c7, dirOut, "AbsoluteEfficiency");
    
    // MULTIPLE RECONSTRUCTION FRACTION --------------------
    
    // Build TGraphs
    TGraphAsymmErrors* gMulEta1 = new TGraphAsymmErrors();
        gMulEta1->SetName("gMulEta1");
        gMulEta1->Divide(hMulEta1,hSimEta1);
        gMulEta1->SetMarkerStyle(25);
        gMulEta1->SetLineStyle(2);
        gMulEta1->SetLineColor(2);
        gMulEta1->SetMarkerColor(2);
    TGraphAsymmErrors* gMulPt1 = new TGraphAsymmErrors();
        gMulPt1->SetName("gMulPt1");
        gMulPt1->Divide(hMulPt1,hSimPt1);
        gMulPt1->SetMarkerStyle(24);
        gMulPt1->SetLineColor(4);
        gMulPt1->SetMarkerColor(4);
    TGraphAsymmErrors* gMulEta2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gMulEta2->SetName("gMulEta2");
        gMulEta2->Divide(hMulEta2,hSimEta2);
        gMulEta2->SetMarkerStyle(24);
        gMulEta2->SetLineColor(4);
        gMulEta2->SetMarkerColor(4);
    }
    TGraphAsymmErrors* gMulPt2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gMulPt2->SetName("gMulPt2");
        gMulPt2->Divide(hMulPt2,hSimPt2);
        gMulPt2->SetMarkerStyle(25);
        gMulPt2->SetLineStyle(2);
        gMulPt2->SetLineColor(2);
        gMulPt2->SetMarkerColor(2);
    }
    TCanvas *c8 = makeMultiCanvas("c8","Multiple Fraction", 2, 1);
    // graph vs eta
    TH1F* hDumEtaMul = (TH1F*) hDumEta3->Clone("hDumEtaMul");
    fixedFontHist1D(hDumEtaMul, 1.05, 1.2);
    hDumEtaMul->GetYaxis()->SetRangeUser(linMin, mulMax);
    hDumEtaMul->GetYaxis()->SetTitle("Multiple Reconstruction Fraction");
    // graph vs pt
    TH1F* hDumPtMul = (TH1F*) hDumPt3->Clone("hDumPtMul");
    fixedFontHist1D(hDumPtMul, 1.05, 1.2);
    hDumPtMul->GetYaxis()->SetRangeUser(linMin, mulMax);
    hDumPtMul->GetYaxis()->SetTitle("Multiple Reconstruction Fraction");
    // new legends
    TLegend* legEta2 = (TLegend*) legEta->Clone();
    TLegend* legPt2 = (TLegend*) legPt->Clone();
    // save canvas
    c8->cd(1);
        hDumEtaMul->Draw();
        gMulEta1->Draw("pc");
        if( usePlot2 ) gMulEta2->Draw("pc");
        legEta2->Draw();
        gPad->SetTicks();
        c8->GetPad(1)->SetLeftMargin(0.12);
        c8->GetPad(1)->SetBottomMargin(0.13);
        gPad->SetLogx(0);
    c8->cd(2);
        hDumPtMul->Draw();
        gMulPt1->Draw("pc");
        if( usePlot2 ) gMulPt2->Draw("pc");
        legPt2->Draw();
        gPad->SetTicks();
        c8->GetPad(2)->SetLeftMargin(0.12);
        c8->GetPad(2)->SetBottomMargin(0.13);
        gPad->SetLogx(1);
    saveCanvas(c8, dirOut, "MultipleReconstruction");
    
    // FAKE RECONSTRUCTION FRACTION --------------------
    
    // Build TGraphs
    TGraphAsymmErrors* gFakEta1 = new TGraphAsymmErrors();
        gFakEta1->SetName("gFakEta1");
        gFakEta1->Divide(hFakEta1,hRecEta1);
        gFakEta1->SetMarkerStyle(24);
        gFakEta1->SetLineColor(4);
        gFakEta1->SetMarkerColor(4);
    TGraphAsymmErrors* gFakPt1 = new TGraphAsymmErrors();
        gFakPt1->SetName("gFakPt1");
        gFakPt1->Divide(hFakPt1,hRecPt1);
        gFakPt1->SetMarkerStyle(24);
        gFakPt1->SetLineColor(4);
        gFakPt1->SetMarkerColor(4);
    TGraphAsymmErrors* gFakEta2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gFakEta2->SetName("gFakEta2");
        gFakEta2->Divide(hFakEta2,hRecEta2);
        gFakEta2->SetMarkerStyle(25);
        gFakEta2->SetLineStyle(2);
        gFakEta2->SetLineColor(2);
        gFakEta2->SetMarkerColor(2);
    }
    TGraphAsymmErrors* gFakPt2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gFakPt2->SetName("gFakPt2");
        gFakPt2->Divide(hFakPt2,hRecPt2);
        gFakPt2->SetMarkerStyle(25);
        gFakPt2->SetLineStyle(2);
        gFakPt2->SetLineColor(2);
        gFakPt2->SetMarkerColor(2);
    }
    TCanvas* c9 = makeMultiCanvas("c9", "Fake Fraction", 2, 1);
    // graph vs eta
    TH1F* hDumEtaFak = (TH1F*) hDumEta2->Clone("hDumEtaMul");
    fixedFontHist1D(hDumEtaFak, 1.05, 1.2);
    hDumEtaFak->GetYaxis()->SetRangeUser(linMin, fakMax);
    hDumEtaFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
    // graph vs pt
    TH1F* hDumPtFak = (TH1F*) hDumPt2->Clone("hDumPtMul");
    fixedFontHist1D(hDumPtFak, 1.05, 1.2);
    hDumPtFak->GetYaxis()->SetRangeUser(linMin, fakMax);
    hDumPtFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
    // save canvas
    c9->cd(1); hDumEtaFak->Draw();
        gFakEta1->Draw("pc");
        if( usePlot2 ) gFakEta2->Draw("pc");
        legEta2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.13);
    c9->cd(2); hDumPtFak->Draw();
        gFakPt1->Draw("pc");
        if( usePlot2 ) gFakPt2->Draw("pc");
        legPt2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.13);
        gPad->SetLogx(1);
    saveCanvas(c9, dirOut, "FakeRate");
    
    // NON-PRIMARY (SECONDARY)  RECONSTRUCTION FRACTION --------------------
    
    // Build TGraphs
    TGraphAsymmErrors* gSecEta1 = new TGraphAsymmErrors();
        gSecEta1->SetName("gSecEta1");
        gSecEta1->Divide(hSecEta1,hRecEta1);
        gSecEta1->SetMarkerStyle(24);
        gSecEta1->SetLineColor(4);
        gSecEta1->SetMarkerColor(4);
    TGraphAsymmErrors* gSecPt1 = new TGraphAsymmErrors();
        gSecPt1->SetName("gSecPt1");
        gSecPt1->Divide(hSecPt1,hRecPt1);
        gSecPt1->SetMarkerStyle(24);
        gSecPt1->SetLineColor(4);
        gSecPt1->SetMarkerColor(4);
    TGraphAsymmErrors* gSecEta2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gSecEta2->SetName("gSecEta2");
        gSecEta2->Divide(hSecEta2,hRecEta2);
        gSecEta2->SetMarkerStyle(25);
        gSecEta2->SetLineStyle(2);
        gSecEta2->SetLineColor(2);
        gSecEta2->SetMarkerColor(2);
    }
    TGraphAsymmErrors* gSecPt2 = new TGraphAsymmErrors();
    if( usePlot2 ){
        gSecPt2->SetName("gSecPt2");
        gSecPt2->Divide(hSecPt2,hRecPt2);
        gSecPt2->SetMarkerStyle(25);
        gSecPt2->SetLineStyle(2);
        gSecPt2->SetLineColor(2);
        gSecPt2->SetMarkerColor(2);
    }
    TCanvas* c10 = makeMultiCanvas("c10", "Secondary Fraction", 2, 1);
    // graph vs eta
    TH1F* hDumEtaSec = (TH1F*) hDumEta2->Clone("hDumEtaMul");
    fixedFontHist1D(hDumEtaSec, 1.05, 1.3);
    hDumEtaSec->GetYaxis()->SetRangeUser(linMin, secMax);
    hDumEtaSec->GetYaxis()->SetTitle("Non-Primary Reconstruction Fraction");
    // graph vs pt
    TH1F* hDumPtSec = (TH1F*) hDumPt2->Clone("hDumPtMul");
    fixedFontHist1D(hDumPtSec, 1.05, 1.3);
    hDumPtSec->GetYaxis()->SetRangeUser(linMin ,secMax);
    hDumPtSec->GetYaxis()->SetTitle("Non-Primary Reconstruction Fraction");
    // save canvas
    c10->cd(1);
        hDumEtaSec->Draw();
        gSecEta1->Draw("pc");
        if( usePlot2 ) gSecEta2->Draw("pc");
        legEta2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.13);
    c10->cd(2);
        hDumPtSec->Draw();
        gSecPt1->Draw("pc");
        if( usePlot2 ) gSecPt2->Draw("pc");
        legPt2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.13);
        gPad->SetLogx(1);
    saveCanvas(c10, dirOut, "SecondaryReconstruction");

    // EXPORT ROOT FILE --------------------
    
    TFile *fout = new TFile((dirOut + fileOut).c_str(),"RECREATE");
        gEffPt1->Write();
        gEffEta1->Write();
        gMulPt1->Write();
        gMulEta1->Write();
        gFakPt1->Write();
        gFakEta1->Write();
        gSecPt1->Write();
        gSecEta1->Write();
    if( usePlot2 ){
        gEffPt2->Write();
        gEffEta2->Write();
        gMulPt2->Write();
        gMulEta2->Write();
        gFakPt2->Write();
        gFakEta2->Write();
        gSecPt2->Write();
        gSecEta2->Write();
    }
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
            "0.5",  // ptMin1
            "2.4",  // etaMax1
            "2.0",  // ptMin2
            "3.0"   // etaMax2
        );
    };
}
