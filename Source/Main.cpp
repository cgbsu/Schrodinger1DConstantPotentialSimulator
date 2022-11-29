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
				std::vector<RegionCoefficients<profile>> regionCoefficients{
						virtualStarterRegion->makeNext(regionParameters[0])
					};
				for(size_t ii = 1; ii < regionParameters.size(); ++ii)
				{
					regionCoefficients.push_back(
							regionCoefficients[ii - 1].makeNext(regionParameters[ii])
						);
				}
				std::vector<VirtualRegionCoefficients<profile>> virtualRegions{*virtualStarterRegion};
				std::vector<Data<profile>> waveFunctionData;
				for(size_t ii = 0; ii < regionCoefficients.size(); ++ii)
				{
					waveFunctionData.push_back(computeWaveFunction<profile>(
							regionCoefficients[ii], 
							virtualRegions[ii], 
							.01f
						));
					virtualRegions.push_back(regionCoefficients[ii]);
				}
				plot(waveFunctionData);
			}
		});
	cleanUpGUI(window);
}

