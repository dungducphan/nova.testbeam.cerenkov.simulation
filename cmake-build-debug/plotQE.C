#include <iostream>
#include <fstream>
#include <vector>

#include "Style.C"

using namespace std;

void plotQE() {
    GeneralStyle();

    vector<double> wl_fake, qe_fake;
    vector<double> wl_real, qe_real;

    double temp_wl, temp_qe;
    
    ifstream fakePMT("FakePMT.csv");
    while (fakePMT >> temp_wl >> temp_qe) {
        wl_fake.push_back(temp_wl);
        qe_fake.push_back(temp_qe);
    }
    fakePMT.close();

    ifstream realPMT("realPMT.csv");
    while (realPMT >> temp_wl >> temp_qe) {
        wl_real.push_back(temp_wl);
        qe_real.push_back(temp_qe);
    }
    realPMT.close();

    double* wla_fake = (double*)malloc(wl_fake.size()*sizeof(double));
    double* qea_fake = (double*)malloc(wl_fake.size()*sizeof(double));
    double* wla_real = (double*)malloc(wl_real.size()*sizeof(double));
    double* qea_real = (double*)malloc(wl_real.size()*sizeof(double));

    for (unsigned int i = 0; i < wl_fake.size(); i++) {
        wla_fake[i] = wl_fake.at(i);
        qea_fake[i] = qe_fake.at(i);
    }

    for (unsigned int i = 0; i < wl_real.size(); i++) {
        wla_real[i] = wl_real.at(i);
        qea_real[i] = qe_real.at(i);
    }


    TGraph* gr_fake = new TGraph(wl_fake.size(), wla_fake, qea_fake);
    TGraph* gr_real = new TGraph(wl_real.size(), wla_real, qea_real);
    gr_fake->GetXaxis()->SetLimits(200, 700);
    gr_real->GetXaxis()->SetLimits(200, 700);
    gr_fake->GetYaxis()->SetRangeUser(0, 30); 
    gr_real->GetYaxis()->SetRangeUser(0, 30);
    gr_fake->SetLineWidth(2);
    gr_real->SetLineWidth(2);
    gr_fake->SetLineColor(kRed);
    gr_real->SetLineColor(kBlue);
    gr_fake->SetTitle("");
    gr_fake->GetXaxis()->SetTitle("Wavelength (nm)");
    gr_fake->GetXaxis()->CenterTitle();
    gr_fake->GetYaxis()->SetTitle("QE (%)");
    gr_fake->GetYaxis()->CenterTitle();

    TLegend* leg = new TLegend(0.60, 0.65, 0.83, 0.80);
    leg->AddEntry(gr_fake, "Simplified QE", "l");
    leg->AddEntry(gr_real, "8-inch PMT QE", "l");

    TCanvas* c = new TCanvas();
    StyleLinearLinear(c);
    gr_fake->Draw("AL");
    gr_real->Draw("same");
    leg->Draw();

}
