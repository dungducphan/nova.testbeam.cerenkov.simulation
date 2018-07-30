#include "Style.C"

void plotUVcutoff() {
  GeneralStyle();

  TFile* file = new TFile("uvcutoffhist.root","READ");

  TH1D* h_70 = (TH1D*) file->Get("h_UVcutoff");

  h_70->SetTitle("");

  h_70->GetXaxis()->SetTitle("NPE");
  h_70->GetXaxis()->CenterTitle();

  double pos_xi = 0.57;
  double pos_xf = 0.87;
  double pos_yi = 0.55;
  double pos_yf = 0.85;

  TLegend* leg_70 = new TLegend(pos_xi, pos_yi, pos_xf, pos_yf);

  TCanvas* c = new TCanvas();

  // UV cutoff
  h_70->Draw("hist");
  leg_70->AddEntry((TObject*)0, "UV Cutoff", "");
  leg_70->AddEntry((TObject*)0, Form("NPE = %i #pm %i", (int)h_70->GetMean(), (int)h_70->GetStdDev()), "");
  leg_70->Draw();


  c->SaveAs("/Users/dphan/Desktop/UVCutoff.pdf");
}
