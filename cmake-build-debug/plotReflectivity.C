#include <fstream>
#include <vector>

#include "Style.C"

void plotReflectivity() {
  GeneralStyle();

  std::ifstream uvcutoff("/Users/dphan/nova.testbeam.cerenkov/optical.sim/nova.testbeam.cerenkov.simulation/cmake-build-debug/uvcutoff.csv");
  std::vector<double> wavelength_reflec, reflec;
  double x, y;
  while (uvcutoff >> x >> y) {
    wavelength_reflec.push_back(x);
    reflec.push_back(y);
    x = 0;
    y = 0;
  }
  uvcutoff.close();

  double *ephoton_mirror      = (double*) malloc(wavelength_reflec.size()* sizeof(double));
  double *reflectivity_mirror = (double*) malloc(wavelength_reflec.size()* sizeof(double));
  double *efficiency_mirror   = (double*) malloc(wavelength_reflec.size()* sizeof(double));

  for (unsigned short int idx = 0; idx < wavelength_reflec.size(); idx++) {
    ephoton_mirror[idx] = wavelength_reflec.at(idx);
    reflectivity_mirror[idx] = 0.;
    efficiency_mirror[idx] = (reflec.at(idx))/100.;
  }

  TGraph* gr = new TGraph(wavelength_reflec.size(), ephoton_mirror, efficiency_mirror);
  TCanvas* c = new TCanvas();
  StyleLinearLinear(c);
  gr->GetXaxis()->SetLimits(180, 700);
  gr->GetYaxis()->SetRangeUser(0,1);
  gr->GetXaxis()->SetTitle("Wavelength (nm)");
  gr->GetXaxis()->CenterTitle();
  gr->GetYaxis()->SetTitle("Reflectivity");
  gr->GetYaxis()->CenterTitle();
  gr->SetTitle("");
  gr->SetLineWidth(2);
  gr->SetLineColor(kRed);
  gr->Draw("AL");

}