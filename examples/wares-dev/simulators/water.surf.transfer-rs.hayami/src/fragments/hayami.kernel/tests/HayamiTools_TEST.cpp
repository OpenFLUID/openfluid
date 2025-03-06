/**
  @file HayamiTools_TEST.cpp
*/

#include <fstream>
#include <iostream>
#include <filesystem>

#include "../HayamiTools.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

// =====================================================================
// =====================================================================

using namespace fragments::math::numerical;

const std::string TestOutputDir = "./";

void computePropagatedOutput(openfluid::core::SerieOfDoubleValue* QInput, unsigned int MaxSteps, std::vector<double>& QOutput, double& QOutCumul)
{
  double Celerity = 0.045;
  double Sigma    = 500.0;
  double Length   = 50.0;
  int TimeStep    = 60;

  auto n = QInput->size();

  t_HayamiKernel Kernel;
  ComputeHayamiKernel(Celerity, Sigma, Length, MaxSteps, TimeStep, Kernel);
  
  openfluid::core::SerieOfDoubleValue* QInputCurr = new openfluid::core::SerieOfDoubleValue(MaxSteps,openfluid::core::DoubleValue(0.0));
  openfluid::core::DoubleValue QInputSum = 0.0;

  QOutput    = std::vector<double>();
  QOutCumul  = 0.0;
  std::cout << std::scientific << "\nQOut = [";
  for (unsigned int CurrentStep=0;CurrentStep!=n;CurrentStep++)
  {
    QInputCurr->push_back(QInput->at(CurrentStep));
    QInputCurr->erase(QInputCurr->begin());
    QInputSum = QInputSum + QInput->at(CurrentStep);
    QOutput.push_back((QInputSum > 0 ? DoHayamiPropagation(Kernel, CurrentStep, QInputCurr, MaxSteps, TimeStep) : 0.0));
    QOutCumul += QOutput.back();
    std::cout << " " << QOutput.back();
  }
  std::cout << " ]";
  delete(QInputCurr);

}

// =====================================================================
// =====================================================================

void writeVectorInFile(const std::string& filename, openfluid::core::SerieOfDoubleValue* vect) //TOIMPL replace by openfluid util?
{
  std::ofstream out(filename);

  if (out.is_open())
  {
    for (unsigned int k=0;k!=vect->size();k++)
    {
      out << std::endl << k << " " << vect->at(k);
    }
    out.close();
  }
  else
  {
    std::cout << "\nCan not open " << filename << " from current path " << std::filesystem::current_path().string() << std::endl;
  }
}

// =====================================================================
// =====================================================================

void writeVectorInFile(const std::string& filename, const std::vector<double>& vect) //TOIMPL replace by openfluid util?
{
  std::ofstream out(filename);

  if (out.is_open())
  {
    for (unsigned int k=0;k!=vect.size();k++)
    {
      out << std::endl << k << " " << vect[k];
    }
    out.close();
  }
  else
  {
    std::cout << "\nCan not open " << filename << " from current path " << std::filesystem::current_path().string() << std::endl;
  }
}

// =====================================================================
// =====================================================================


TEST_CASE( "Do Hayami propagation", "[DoHayamiPropagation]" )
{

  // create output directory
  if (!std::filesystem::exists(TestOutputDir+"unitTestOUT"))
  {
    std::filesystem::create_directory(TestOutputDir+"unitTestOUT");
  }

  // define water input
  int n = 100;
  openfluid::core::SerieOfDoubleValue* QInput = new openfluid::core::SerieOfDoubleValue(n,openfluid::core::DoubleValue(0.0));
  double QInCumul = 0.0;
  for (unsigned int k=20; k!=40; k++)
  {
    (*QInput)[k] = openfluid::core::DoubleValue(1.0);
    QInCumul += QInput->at(k);
  }
  std::cout << std::scientific << "\nQIn = [";
  for (unsigned int CurrentStep=0;CurrentStep!=QInput->size();CurrentStep++)
  {
    std::cout << " " << QInput->at(CurrentStep);
  }
  std::cout << " ]\n";

  std::vector<double> QOutput;
  double QOutCumul;
  unsigned int MaxSteps;
  
  writeVectorInFile(TestOutputDir+"unitTestOUT/unittest_QInput.csv",QInput);
  
  std::cout << "\n* Test Hayami propagation with QInput.size() < MaxStep";
  MaxSteps = 200;
  computePropagatedOutput(QInput, MaxSteps, QOutput, QOutCumul);
  writeVectorInFile(TestOutputDir+"unitTestOUT/unittest_lessThanMaxStep.csv",QOutput);
  std::cout << std::scientific << "\nQIn cumul = " << QInCumul << " QOut cumul = " << QOutCumul << " diff = " << std::abs(QOutCumul - QInCumul) / QInCumul;
  REQUIRE(( QOutCumul == Catch::Approx(1.950168e+01).margin(1e-9) ));
  REQUIRE(( QOutput[20] == Catch::Approx(1.181085e-07).margin(1e-9) ));
  REQUIRE(( QOutput[39] == Catch::Approx(6.984109e-01).margin(1e-9) ));
  REQUIRE(( QOutput[99] == Catch::Approx(2.196153e-02).margin(1e-9) ));
  std::cout << std::endl;

  std::cout << "\n* Test Hayami propagation with QInput.size() == MaxStep";
  MaxSteps = 100;
  computePropagatedOutput(QInput, MaxSteps, QOutput, QOutCumul);
  writeVectorInFile(TestOutputDir+"unitTestOUT/unittest_equalMaxStep.csv",QOutput);

  std::cout << std::scientific << "\nQIn cumul = " << QInCumul << " QOut cumul = " << QOutCumul << " diff = " << std::abs(QOutCumul - QInCumul) / QInCumul;
  REQUIRE(( QOutCumul == Catch::Approx(1.964300e+01).margin(1e-9) ));
  REQUIRE(( QOutput[20] == Catch::Approx(1.189643e-07).margin(1e-9) ));
  REQUIRE(( QOutput[39] == Catch::Approx(7.034719e-01).margin(1e-9) ));
  REQUIRE(( QOutput[99] == Catch::Approx(2.212067e-02).margin(1e-9) ));
  std::cout << std::endl;

  std::cout << "\n* Test Hayami propagation with QInput.size() > MaxStep";
  MaxSteps = 50;
  computePropagatedOutput(QInput, MaxSteps, QOutput, QOutCumul);
  writeVectorInFile(TestOutputDir+"unitTestOUT/unittest_greaterThanMaxStep.csv",QOutput);

  std::cout << std::scientific << "\nQIn cumul = " << QInCumul << " QOut cumul = " << QOutCumul << " diff = " << std::abs(QOutCumul - QInCumul) / QInCumul;
  REQUIRE(( QOutCumul == Catch::Approx(20.0).margin(1e-9) ));
  REQUIRE(( QOutput[20] == Catch::Approx(1.256881e-07).margin(1e-9) ));
  REQUIRE(( QOutput[39] == Catch::Approx(7.432317e-01).margin(1e-9) ));
  REQUIRE(( QOutput[99] == Catch::Approx(0.0).margin(1e-9) ));
  std::cout << std::endl;

  delete(QInput);

  // TOIMPL compare with REF

}

// =====================================================================
// =====================================================================


