#include "RiceStyle.h"
#include <vector>

void make_plots_2D(
    string fileIn,
    string dirOut,
    string fileOut,
    string ptMax1Str,
    string ptMax2Str,
    string etaMax1Str,
    string etaMax2Str
) {
    
    // Convert string arguments
    std::filesystem::create_directories(dirOut.c_str());
    float ptMax1  = stof(ptMax1Str);
    float ptMax2  = stof(ptMax2Str);
    float etaMax1 = stof(etaMax1Str);
    float etaMax2 = stof(etaMax2Str);
    
    char ndir[256] = "HITrackCorrections";
    float ptMin   = 0.1;
    float ptLimit = 100.;

    RiceStyle();
    gStyle->SetOptStat(0);

    TFile* f = new TFile(fileIn.c_str(), "READ");

    // sim-to-reco hists
    TH2F *hSim = (TH2F*) f->Get(Form("%s/hsim",ndir)); hSim->GetYaxis()->SetRangeUser(ptMin,ptLimit);
    TH2F *hEff = (TH2F*) f->Get(Form("%s/heff",ndir)); hEff->GetYaxis()->SetRangeUser(ptMin,ptLimit);
    TH2F *hMul = (TH2F*) f->Get(Form("%s/hmul",ndir)); hMul->GetYaxis()->SetRangeUser(ptMin,ptLimit);

    // reco-to-sim hists
    TH2F *hRec = (TH2F*) f->Get(Form("%s/hrec",ndir)); hRec->GetYaxis()->SetRangeUser(ptMin,ptLimit);
    TH2F *hFak = (TH2F*) f->Get(Form("%s/hfak",ndir)); hFak->GetYaxis()->SetRangeUser(ptMin,ptLimit);
    TH2F *hSec = (TH2F*) f->Get(Form("%s/hsec",ndir)); hSec->GetYaxis()->SetRangeUser(ptMin,ptLimit);

    // ratio histograms
    TH2F *rEff = (TH2F*) hEff->Clone("rEff");
    TH2F *rMul = (TH2F*) hMul->Clone("rMul");
    TH2F *rFak = (TH2F*) hFak->Clone("rFak");
    TH2F *rSec = (TH2F*) hSec->Clone("rSec");

    //---------------------------------------------

    // reco efficiency fraction
    TCanvas *c2 = new TCanvas("c2","Reco Efficiency Fraction",1000,500);
    //c2->SetLogy();
    gPad->SetRightMargin(0.15);
    rEff->Divide(hEff,hSim,1,1,"B");
    rEff->SetStats(0);
    rEff->SetMaximum(1.0); rEff->SetMinimum(0.0);
    rEff->SetTitle("Absolute Efficiency");
    rEff->Draw("colz");

    // multiple reco fraction
    TCanvas *c3 = new TCanvas("c3","Multiple Reco Fraction",1000,500);
    //c3->SetLogy();
    gPad->SetRightMargin(0.15);
    rMul->Divide(hMul,hSim,1,1,"B");
    rMul->SetStats(0);
    rMul->SetMaximum(1.0); rMul->SetMinimum(0.0);
    rMul->SetTitle("Multiple Reconstruction Fraction");
    rMul->Draw("colz");

    // fake reco fraction
    TCanvas *c4 = new TCanvas("c4","Fake Reco Fraction",1000,500);
    //c4->SetLogy();
    gPad->SetRightMargin(0.15);
    rFak->Divide(hFak,hRec,1,1,"B");
    rFak->SetStats(0);
    rFak->SetMaximum(1.0); rFak->SetMinimum(0.0);
    rFak->SetTitle("Fake Reconstruction Fraction");
    rFak->Draw("colz");

    // secondary reco fraction
    TCanvas *c5 = new TCanvas("c5","Secondary Fraction",1000,500);
    //c5->SetLogy();
    gPad->SetRightMargin(0.15);
    rSec->Divide(hSec,hRec,1,1,"B");
    rSec->SetStats(0);
    rSec->SetMaximum(1.0); rSec->SetMinimum(0.0);
    rSec->SetTitle("Non-Primary Reconstruction Fraction");
    rSec->Draw("colz");

    //---------------------------------------------

    // find bins corresponding to projections for below
    Int_t ptBinMax1 = hSim->GetYaxis()->FindBin(ptMax1 + 0.01);
    Int_t ptBinMax2 = hSim->GetYaxis()->FindBin(ptMax2 + 0.01);
    Int_t ptBins   = hSim->GetYaxis()->GetNbins();
    Int_t etaBin1M = hSim->GetXaxis()->FindBin(-etaMax1 + 0.01);
    Int_t etaBin1P = hSim->GetXaxis()->FindBin(etaMax1 - 0.01);
    Int_t etaBin2M = hSim->GetXaxis()->FindBin(-etaMax2 + 0.01);
    Int_t etaBin2P = hSim->GetXaxis()->FindBin(etaMax2 - 0.01);
    
    // projected hists: pt < ptMax1
    TH1D* hSimEta1 = (TH1D*) hSim->ProjectionX("hSimEta1",ptBinMax1,ptBins,"e");
    TH1D* hEffEta1 = (TH1D*) hEff->ProjectionX("hEffEta1",ptBinMax1,ptBins,"e");
    TH1D* hMulEta1 = (TH1D*) hMul->ProjectionX("hMulEta1",ptBinMax1,ptBins,"e");
    TH1D* hRecEta1 = (TH1D*) hRec->ProjectionX("hRecEta1",ptBinMax1,ptBins,"e");
    TH1D* hFakEta1 = (TH1D*) hFak->ProjectionX("hFakEta1",ptBinMax1,ptBins,"e");
    TH1D* hSecEta1 = (TH1D*) hSec->ProjectionX("hSecEta1",ptBinMax1,ptBins,"e");
    // projected hists: pt < ptMax2
    TH1D* hSimEta2 = (TH1D*) hSim->ProjectionX("hSimEta2",ptBinMax2,ptBins,"e");
    TH1D* hEffEta2 = (TH1D*) hEff->ProjectionX("hEffEta2",ptBinMax2,ptBins,"e");
    TH1D* hMulEta2 = (TH1D*) hMul->ProjectionX("hMulEta2",ptBinMax2,ptBins,"e");
    TH1D* hRecEta2 = (TH1D*) hRec->ProjectionX("hRecEta2",ptBinMax2,ptBins,"e");
    TH1D* hFakEta2 = (TH1D*) hFak->ProjectionX("hFakEta2",ptBinMax2,ptBins,"e");
    TH1D* hSecEta2 = (TH1D*) hSec->ProjectionX("hSecEta2",ptBinMax2,ptBins,"e");

    TH1D* hDumEta = new TH1D("hDumEta",";#eta",60,-2.4,2.4);
        hDumEta->SetMaximum(1.0);
        hDumEta->GetXaxis()->CenterTitle();
        hDumEta->GetYaxis()->SetTitleOffset(1.8);
    TH1D* hDumEta2 = (TH1D*) hDumEta->Clone("hDumEta2");
        hDumEta2->SetMaximum(0.1);
    TH1D* hDumEta3 = (TH1D*) hDumEta->Clone("hDumEta3");
        hDumEta3->SetMaximum(0.00049);
    
    // projected hists: abs(eta) < etaMax1
    TH1D* hSimPt1 = (TH1D*) hSim->ProjectionY("hSimPt1",etaBin1M,etaBin1P,"e");
    TH1D* hEffPt1 = (TH1D*) hEff->ProjectionY("hEffPt1",etaBin1M,etaBin1P,"e");
    TH1D* hMulPt1 = (TH1D*) hMul->ProjectionY("hMulPt1",etaBin1M,etaBin1P,"e");
    TH1D* hRecPt1 = (TH1D*) hRec->ProjectionY("hRecPt1",etaBin1M,etaBin1P,"e");
    TH1D* hFakPt1 = (TH1D*) hFak->ProjectionY("hFakPt1",etaBin1M,etaBin1P,"e");
    TH1D* hSecPt1 = (TH1D*) hSec->ProjectionY("hSecPt1",etaBin1M,etaBin1P,"e");
    // projected hists: abs(eta) < etaMax2
    TH1D* hSimPt2 = (TH1D*) hSim->ProjectionY("hSimPt2",etaBin2M,etaBin2P,"e");
    TH1D* hEffPt2 = (TH1D*) hEff->ProjectionY("hEffPt2",etaBin2M,etaBin2P,"e");
    TH1D* hMulPt2 = (TH1D*) hMul->ProjectionY("hMulPt2",etaBin2M,etaBin2P,"e");
    TH1D* hRecPt2 = (TH1D*) hRec->ProjectionY("hRecPt2",etaBin2M,etaBin2P,"e");
    TH1D* hFakPt2 = (TH1D*) hFak->ProjectionY("hFakPt2",etaBin2M,etaBin2P,"e");
    TH1D* hSecPt2 = (TH1D*) hSec->ProjectionY("hSecPt2",etaBin2M,etaBin2P,"e");

    TH1D* hDumPt = new TH1D("hDumPt",";p_{T} [GeV/c]",80,0.1,10.0);
        hDumPt->SetMaximum(1.0);
        hDumPt->GetXaxis()->CenterTitle();
        hDumPt->GetYaxis()->SetTitleOffset(1.8);
    TH1D* hDumPt2 = (TH1D*) hDumPt->Clone("hDumPt2");
        hDumPt2->SetMaximum(0.1);
    TH1D* hDumPt3 = (TH1D*) hDumPt->Clone("hDumPt3");
        hDumPt3->SetMaximum(0.00049);

    // Efficiency for etaMax1
    TGraphAsymmErrors* gEffEta1 = new TGraphAsymmErrors();
        gEffEta1->SetName("gEffEta1");
        gEffEta1->BayesDivide(hEffEta1,hSimEta1); // WHAT IS THIS DOING?
        gEffEta1->SetMarkerColor(2);
        gEffEta1->SetMarkerStyle(25);
        gEffEta1->SetLineColor(2);
        gEffEta1->SetLineStyle(2);
    // Efficiency for etaMax2
    TGraphAsymmErrors* gEffEta2 = new TGraphAsymmErrors();
        gEffEta2->SetName("gEffEta2");
        gEffEta2->BayesDivide(hEffEta2,hSimEta2); // WHAT IS THIS DOING?
        gEffEta2->SetMarkerColor(4);
        gEffEta2->SetMarkerStyle(24);
        gEffEta2->SetLineColor(4);
    // Eta Legend
    TLegend *legEta = new TLegend(0.15,0.80,0.50,0.60);
        legEta->SetFillColor(0);
        legEta->SetFillStyle(0);
        legEta->SetBorderSize(0);
        legEta->SetTextSize(0.04);
        legEta->AddEntry(gEffEta1,("p_{T} > " + ptMax1Str + " GeV/c").c_str(),"lp"); // IS THIS BACKWARDS???
        legEta->AddEntry(gEffEta2,("p_{T} > " + ptMax2Str + " GeV/c").c_str(),"lp");

    // Efficiency for ptMax1
    TGraphAsymmErrors* gEffPt1 = new TGraphAsymmErrors();
        gEffPt1->SetName("gEffPt1");
        gEffPt1->BayesDivide(hEffPt1,hSimPt1); // WHAT IS THIS DOING?
        gEffPt1->SetMarkerColor(4);
        gEffPt1->SetMarkerStyle(24);
        gEffPt1->SetLineColor(4);
    // Efficiency for ptMax2
    TGraphAsymmErrors* gEffPt2 = new TGraphAsymmErrors();
        gEffPt2->SetName("gEffPt2");
        gEffPt2->BayesDivide(hEffPt2,hSimPt2); // WHAT IS THIS DOING?
        gEffPt2->SetLineColor(2);
        gEffPt2->SetLineStyle(2);
        gEffPt2->SetMarkerColor(2);
        gEffPt2->SetMarkerStyle(25);
    // pT Legend
    TLegend *legPt = new TLegend(0.15,0.80,0.50,0.60);
        legPt->SetFillColor(0);
        legPt->SetFillStyle(0);
        legPt->SetBorderSize(0);
        legPt->SetTextSize(0.04);
        legPt->AddEntry(gEffPt1,("|#eta| < " + etaMax1Str).c_str(),"lp"); // IS THIS BACKWARDS???
        legPt->AddEntry(gEffPt2,("|#eta| < " + etaMax2Str).c_str(),"lp");

    //Dener--------------------------------
    TH1F* hDumEtaEff;
    TH1F* hDumPtEff;
    //-------------------------------------
    TCanvas* c7 = makeMultiCanvas("c7", "Efficiency Fraction", 2,1 );
    
    hDumEtaEff = (TH1F*) hDumEta->Clone("hDumEtaEff");
    fixedFontHist1D(hDumEtaEff,1.05,1.2);
    hDumEtaEff->GetYaxis()->SetTitle("Absolute efficiency");
    
    hDumPtEff = (TH1F*) hDumPt->Clone("hDumPtEff");
    fixedFontHist1D(hDumPtEff,1.05,1.2);
    hDumPtEff->GetYaxis()->SetTitle("Absolute efficiency");
    
    c7->cd(1);
        hDumEtaEff->Draw();
        gEffEta1->Draw("pc");
        gEffEta2->Draw("pc");
        legEta->Draw();
        gPad->SetTicks();
        c7->GetPad(1)->SetLeftMargin(0.12);
        c7->GetPad(1)->SetBottomMargin(0.13);
        c7->GetPad(1)->SetLogx(0);
    c7->cd(2);
        hDumPtEff->Draw();
        gEffPt1->Draw("pc");
        gEffPt2->Draw("pc");
        legPt->Draw();
        gPad->SetTicks();
        c7->GetPad(2)->SetLeftMargin(0.12);
        c7->GetPad(2)->SetBottomMargin(0.13);
        c7->GetPad(2)->SetLogx();
    saveCanvas(c7, dirOut, "AbsoluteEfficiency");

    // Multiple Reco
    TGraphAsymmErrors* gMulEta1 = new TGraphAsymmErrors();
        gMulEta1->SetName("gMulEta1");
        gMulEta1->BayesDivide(hMulEta1,hSimEta1);
        gMulEta1->SetMarkerStyle(25);
        gMulEta1->SetLineStyle(2);
        gMulEta1->SetLineColor(2);
        gMulEta1->SetMarkerColor(2);

    TGraphAsymmErrors* gMulPt1 = new TGraphAsymmErrors();
        gMulPt1->SetName("gMulPt1");
        gMulPt1->BayesDivide(hMulPt1,hSimPt1);
        gMulPt1->SetMarkerStyle(24);
        gMulPt1->SetLineColor(4);
        gMulPt1->SetMarkerColor(4);

    TGraphAsymmErrors* gMulEta2 = new TGraphAsymmErrors();
        gMulEta2->SetName("gMulEta2");
        gMulEta2->BayesDivide(hMulEta2,hSimEta2);
        gMulEta2->SetMarkerStyle(24);
        gMulEta2->SetLineColor(4);
        gMulEta2->SetMarkerColor(4);

    TGraphAsymmErrors* gMulPt2 = new TGraphAsymmErrors();
        gMulPt2->SetName("gMulPt2");
        gMulPt2->BayesDivide(hMulPt2,hSimPt2);
        gMulPt2->SetMarkerStyle(25);
        gMulPt2->SetLineStyle(2);
        gMulPt2->SetLineColor(2);
        gMulPt2->SetMarkerColor(2);

    //Dener--------------------------------
    TH1F* hDumEtaMul;
    TH1F* hDumPtMul;
    TLegend* legEta2;
    TLegend* legPt2;
    //      //-------------------------------------
    TCanvas *c8 = makeMultiCanvas("c8","Multiple Fraction", 2, 1);
    
    hDumEtaMul = (TH1F*) hDumEta3->Clone("hDumEtaMul");
    fixedFontHist1D(hDumEtaMul, 1.05, 1.2);
    hDumEtaMul->GetYaxis()->SetRangeUser(0, 1.0);
    hDumEtaMul->GetYaxis()->SetTitle("Multiple Reconstruction Fraction");
    
    hDumPtMul = (TH1F*) hDumPt3->Clone("hDumPtMul");
    fixedFontHist1D(hDumPtMul, 1.05, 1.2);
    hDumPtMul->GetYaxis()->SetRangeUser(0, 1.0);
    hDumPtMul->GetYaxis()->SetTitle("Multiple Reconstruction Fraction");
    
    legEta2 = (TLegend*) legEta->Clone(); // legEta2->SetY1(0.65); legEta2->SetY2(0.85);
    legPt2 = (TLegend*) legPt->Clone(); // legPt2->SetY1(0.65); legPt2->SetY2(0.85);
    
    c8->cd(1);
        hDumEtaMul->Draw();
        gMulEta1->Draw("pc");
        gMulEta2->Draw("pc");
        legEta2->Draw();
        gPad->SetTicks();
        c8->GetPad(1)->SetLeftMargin(0.12);
        c8->GetPad(1)->SetBottomMargin(0.13);
        gPad->SetLogx(0);
    c8->cd(2);
        hDumPtMul->Draw();
        gMulPt1->Draw("pc");
        gMulPt2->Draw("pc");
        legPt2->Draw();
        gPad->SetTicks();
        c8->GetPad(2)->SetLeftMargin(0.12);
        c8->GetPad(2)->SetBottomMargin(0.13);
        gPad->SetLogx(1);
    saveCanvas(c8, dirOut, "MultipleReconstruction");

    // Fakes
    TGraphAsymmErrors* gFakEta1 = new TGraphAsymmErrors();
        gFakEta1->SetName("gFakEta1");
        gFakEta1->BayesDivide(hFakEta1,hRecEta1);
        gFakEta1->SetMarkerStyle(25);
        gFakEta1->SetLineStyle(2);
        gFakEta1->SetLineColor(2);
        gFakEta1->SetMarkerColor(2);

    TGraphAsymmErrors* gFakPt1 = new TGraphAsymmErrors();
        gFakPt1->SetName("gFakPt1");
        gFakPt1->BayesDivide(hFakPt1,hRecPt1);
        gFakPt1->SetMarkerStyle(24);
        gFakPt1->SetLineColor(4);
        gFakPt1->SetMarkerColor(4);

    TGraphAsymmErrors* gFakEta2 = new TGraphAsymmErrors();
        gFakEta2->SetName("gFakEta2");
        gFakEta2->BayesDivide(hFakEta2,hRecEta2);
        gFakEta2->SetMarkerStyle(24);
        gFakEta2->SetLineColor(4);
        gFakEta2->SetMarkerColor(4);

    TGraphAsymmErrors* gFakPt2 = new TGraphAsymmErrors();
        gFakPt2->SetName("gFakPt2");
        gFakPt2->BayesDivide(hFakPt2,hRecPt2);
        gFakPt2->SetMarkerStyle(25);
        gFakPt2->SetLineStyle(2);
        gFakPt2->SetLineColor(2);
        gFakPt2->SetMarkerColor(2);

    //TCanvas *c9 = new TCanvas("c9","Fake Fraction",900,500);

    //Dener--------------------------------
    TH1F* hDumEtaFak;
    TH1F* hDumPtFak;
    //-------------------------------------
    TCanvas* c9 = makeMultiCanvas("c9", "Fake Fraction", 2, 1);
    
    hDumEtaFak = (TH1F*) hDumEta2->Clone("hDumEtaMul");
    fixedFontHist1D(hDumEtaFak, 1.05, 1.2);
    hDumEtaFak->GetYaxis()->SetRangeUser(0.0, 1.0);
    hDumEtaFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
    
    hDumPtFak = (TH1F*) hDumPt2->Clone("hDumPtMul");
    fixedFontHist1D(hDumPtFak, 1.05, 1.2);
    hDumPtFak->GetYaxis()->SetRangeUser(0.0, 1.0);
    hDumPtFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
    
    c9->cd(1); hDumEtaFak->Draw();
        gFakEta1->Draw("pc");
        gFakEta2->Draw("pc");
        legEta2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.13);
    c9->cd(2); hDumPtFak->Draw();
        gFakPt1->Draw("pc");
        gFakPt2->Draw("pc");
        legPt2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.13);
        gPad->SetLogx(1);
    saveCanvas(c9, dirOut, "FakeRate");
    //  c9->GetPad(2)->SetLogx();

    // Secondaries
    TGraphAsymmErrors* gSecEta1 = new TGraphAsymmErrors();
        gSecEta1->SetName("gSecEta1");
        gSecEta1->BayesDivide(hSecEta1,hRecEta1);
        gSecEta1->SetMarkerStyle(25);
        gSecEta1->SetLineStyle(2);
        gSecEta1->SetLineColor(2);
        gSecEta1->SetMarkerColor(2);

    TGraphAsymmErrors* gSecPt1 = new TGraphAsymmErrors();
        gSecPt1->SetName("gSecPt1");
        gSecPt1->BayesDivide(hSecPt1,hRecPt1);
        gSecPt1->SetMarkerStyle(24);
        gSecPt1->SetLineColor(4);
        gSecPt1->SetMarkerColor(4);

    TGraphAsymmErrors* gSecEta2 = new TGraphAsymmErrors();
        gSecEta2->SetName("gSecEta2");
        gSecEta2->BayesDivide(hSecEta2,hRecEta2);
        gSecEta2->SetMarkerStyle(24);
        gSecEta2->SetLineColor(4);
        gSecEta2->SetMarkerColor(4);

    TGraphAsymmErrors* gSecPt2 = new TGraphAsymmErrors();
        gSecPt2->SetName("gSecPt2");
        gSecPt2->BayesDivide(hSecPt2,hRecPt2);
        gSecPt2->SetMarkerStyle(25);
        gSecPt2->SetLineStyle(2);
        gSecPt2->SetLineColor(2);
        gSecPt2->SetMarkerColor(2);

    //Dener--------------------------------
      TH1F* hDumEtaSec;
      TH1F* hDumPtSec;
    //-------------------------------------
    
    TCanvas* c10 = makeMultiCanvas("c10", "Secondary Fraction", 2, 1);
    
    hDumEtaSec = (TH1F*) hDumEta2->Clone("hDumEtaMul");
    fixedFontHist1D(hDumEtaSec, 1.05, 1.3);
    hDumEtaSec->GetYaxis()->SetRangeUser(0.0, 1.0);
    hDumEtaSec->GetYaxis()->SetTitle("Non-Primary Reconstruction Fraction");
    
    hDumPtSec = (TH1F*) hDumPt2->Clone("hDumPtMul");
    fixedFontHist1D(hDumPtSec, 1.05, 1.3);
    hDumPtSec->GetYaxis()->SetRangeUser(0.0 ,1.0);
    hDumPtSec->GetYaxis()->SetTitle("Non-Primary Reconstruction Fraction");
    
    c10->cd(1);
        hDumEtaSec->Draw();
        gSecEta1->Draw("pc");
        gSecEta2->Draw("pc");
        legEta2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.13);
    c10->cd(2);
        hDumPtSec->Draw();
        gSecPt1->Draw("pc");
        gSecPt2->Draw("pc");
        legPt2->Draw();
        gPad->SetTicks();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.13);
        gPad->SetLogx(1);
    saveCanvas(c10, dirOut, "SecondaryReconstruction");

    TFile *fout = new TFile((dirOut + fileOut).c_str(),"RECREATE");
    gEffPt1->Write();  gEffPt2->Write();
    gEffEta1->Write(); gEffEta2->Write();
    gMulPt1->Write();  gMulPt2->Write();
    gMulEta1->Write(); gMulEta2->Write();
    gFakPt1->Write();  gFakPt2->Write();
    gFakEta1->Write(); gFakEta2->Write();
    gSecPt1->Write();  gSecPt2->Write();
    gSecEta1->Write(); gSecEta2->Write();
    fout->Close();
}



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



void plotHist2D_v2() {
    vector<vector<string>> fileNames = {
        {
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/TrackAnalyzer_single_diff_STARLIGHT_UPC_2024_01_19.root",
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/AndreRootOutput/",
            "plots.root",
            "0.4", "2.0",  // ptMax1, ptMax2
            "1.0", "2.4"    // etaMax1, etaMax2
        },
        {
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/trk_GeneralTrack_6k.root",
            "/Users/jordanlang/Documents/Research/MIT Research/UPC Analysis/Tracking/Data/RootOutput/",
            "plots.root",
            "0.4", "2.0",  // ptMax1, ptMax2
            "1.0", "2.4"    // etaMax1, etaMax2
        }
    };
    for( int i=0 ; i < fileNames.size() ; i++ ) {
        cout << "fileIn:  " << fileNames[i][0].c_str() << "\ndirOut:  " << fileNames[i][1].c_str() << "\nfileOut: " << fileNames[i][2].c_str() << endl;
        make_plots_2D(
            fileNames[i][0],
            fileNames[i][1],
            fileNames[i][2],
            fileNames[i][3],
            fileNames[i][4],
            fileNames[i][5],
            fileNames[i][6]
        );
    };
}
