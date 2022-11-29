#include <ComputeRegion.hpp>
#include <ChartView.hpp>

int main(int argc, char** args)
{
	constexpr const auto profile = Profiles::PhysicallyInaccurateSimple;
	using ScalarType = Constants<profile>::ScalarType;
	ScalarType totalEnergy = .1f;
	ScalarType mass = 1.f;
	auto simulationParameters 
			= std::make_unique<SimulationParameters<profile>>(
					totalEnergy, 
					mass
				);
	auto virtualStarterRegion 
			= std::make_unique<VirtualRegionCoefficients<profile>>(
					simulationParameters, 
					RegionParameters<profile>{1.f, 0.f}, 
					BoundryCoefficientsType<profile> {.4f, .4f}
				);
	std::vector<RegionParameters<profile>> regionParameters;
	auto* window = initializeGUI();
	const auto sliderDimensions = ImVec2(40, 160);
	float reasonableMaximum = 1.f;
	renderLoop(window, [&]()
		{
			simulationParameters = std::move(simulationParametersGui(
					reasonableMaximum, 
					simulationParameters, 
					sliderDimensions
				));
			virtualStarterRegion = std::move(virtualRegionParametersGui(
					reasonableMaximum, 
					simulationParameters, 
					virtualStarterRegion, 
					sliderDimensions
				));
			regionParameters = simulationControlWindow(
					reasonableMaximum, 
					simulationParameters, 
					virtualStarterRegion, 
					regionParameters, 
					sliderDimensions
				);
			if(regionParameters.size() > 0)
			{
				const auto regionCoefficients = regionCoefficientsFromParameters<profile>(
						simulationParameters, 
						virtualStarterRegion, 
						regionParameters
					);
				const auto waveFunctionData = computeWaveFunctionValues<profile>(
						regionCoefficients, 
						virtualStarterRegion, 
						.001f
					);
				plot(waveFunctionData);
			}
		});
	cleanUpGUI(window);
}

