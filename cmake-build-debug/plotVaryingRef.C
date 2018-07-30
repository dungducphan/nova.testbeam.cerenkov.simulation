#include "Style.C"

void plotVaryingRef() {
    GeneralStyle();

    TFile* file = new TFile("varyingReflectivity.root","READ");
    
    TH1D* h_70 = (TH1D*) file->Get("h_70");
    TH1D* h_80 = (TH1D*) file->Get("h_80");
    TH1D* h_90 = (TH1D*) file->Get("h_90");
    TH1D* h_100 = (TH1D*) file->Get("h_100");

    h_70->SetTitle("");
    h_80->SetTitle("");
    h_90->SetTitle("");
    h_100->SetTitle("");

    h_70->GetXaxis()->SetTitle("NPE");
    h_70->GetXaxis()->CenterTitle();
    h_80->GetXaxis()->SetTitle("NPE");
    h_80->GetXaxis()->CenterTitle();
    h_90->GetXaxis()->SetTitle("NPE");
    h_90->GetXaxis()->CenterTitle();
    h_100->GetXaxis()->SetTitle("NPE");
    h_100->GetXaxis()->CenterTitle();

    double pos_xi = 0.57;
    double pos_xf = 0.87;
    double pos_yi = 0.55;
    double pos_yf = 0.85;

    TLegend* leg_70 = new TLegend(pos_xi, pos_yi, pos_xf, pos_yf);
    TLegend* leg_80 = new TLegend(pos_xi, pos_yi, pos_xf, pos_yf);
    TLegend* leg_90 = new TLegend(pos_xi, pos_yi, pos_xf, pos_yf);
    TLegend* leg_100 = new TLegend(pos_xi, pos_yi, pos_xf, pos_yf);

    TCanvas* c = new TCanvas("c","c", 600, 4800);
    c->Divide(2,2);
    
    // 70%
    c->cd(1);
    h_70->Draw("hist");
    leg_70->AddEntry((TObject*)0, "70% Reflectivity", "");
    leg_70->AddEntry((TObject*)0, Form("NPE = %i #pm %i", (int)h_70->GetMean(), (int)h_70->GetStdDev()), "");
    leg_70->Draw();

    // 80%
    c->cd(2);
    h_80->Draw("hist");
    leg_80->AddEntry((TObject*)0, "80% Reflectivity", "");
    leg_80->AddEntry((TObject*)0, Form("NPE = %i #pm %i", (int)h_80->GetMean(), (int)h_80->GetStdDev()), "");
    leg_80->Draw();

    // 90%
    c->cd(3);
    h_90->Draw("hist");
    leg_90->AddEntry((TObject*)0, "90% Reflectivity", "");
    leg_90->AddEntry((TObject*)0, Form("NPE = %i #pm %i", (int)h_90->GetMean(), (int)h_90->GetStdDev()), "");
    leg_90->Draw();

    // 100%
    c->cd(4);
    h_100->Draw("hist");
    leg_100->AddEntry((TObject*)0, "100% Reflectivity", "");
    leg_100->AddEntry((TObject*)0, Form("NPE = %i #pm %i", (int)h_100->GetMean(), (int)h_100->GetStdDev()), "");
    leg_100->Draw();


    c->SaveAs("/Users/dphan/Desktop/CompareReflectivity.pdf");



}
