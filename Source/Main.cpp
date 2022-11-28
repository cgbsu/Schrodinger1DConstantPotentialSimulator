#include <ComputeRegion.hpp>

int main(int argc, char** args)
{
	const auto fromHarmonicConstant = computeHarmonicConstant(1.f, .5f, 1.f);
	const auto toHarmonicConstant = computeHarmonicConstant(.6f, .5f, 1.f);
	const auto scatteringCoefficients = computeTransmissionReflectionRatios(
			fromHarmonicConstant, 
			toHarmonicConstant
		);
	const auto boundryCoefficients = computeBoundry(
			BoundryCoefficientsType{.4f, .4f}, 
			scatteringCoefficients
		);
	std::cout 
			<< "From Harmonic Constant: " << fromHarmonicConstant << "\n"
			<< "ToHarmonic Constant: " << toHarmonicConstant << "\n"
			<< "Transmission Ratio: " << scatteringCoefficients.transmission << "\n"
			<< "Reflection Ratio: " << scatteringCoefficients.reflection << "\n"
			<< "Tranmssion Part of Boundry: " << boundryCoefficients.transmission << "\n"
			<< "Reflection Part of Boundry: " << boundryCoefficients.reflection << "\n";
	//QApplication application(argc, args);
	//QMainWindow window;
	//window.setCentralWidget(new ChartView());
	//window.resize(400, 300);
	//window.show();
	//return application.exec();
	return 0;
}

