#include "Style.C"

void plotCrossCheck() {
    GeneralStyle();

    TFile* file = new TFile("crosscheckhist.root","READ");
    TH1D* hist = (TH1D*)file->Get("h");

    hist->SetTitle("");
    hist->GetXaxis()->SetTitle("NPE");
    hist->GetXaxis()->CenterTitle();

    TCanvas* c = new TCanvas();
    hist->Draw("hist");
    c->SaveAs("/Users/dphan/Desktop/hist_crosscheck.pdf");
}
