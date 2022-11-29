#include <ComputeRegion.hpp>

int main(int argc, char** args)
{
	constexpr const auto profile = Profiles::PhysicallyInaccurateSimple;
	using ScalarType = Constants<profile>::ScalarType;
	constexpr const ScalarType totalEnergy = .1f;
	constexpr const ScalarType mass = 1.f;
	constexpr const BoundryCoefficientsType<profile> startBoundry{.4f, .4f};
	const auto regionParameters = std::array{
			RegionParameters<profile>{.8f, 1.f}, 
			RegionParameters<profile>{.5f, 2.f}, 
			RegionParameters<profile>{.2f, 3.f}, 
			RegionParameters<profile>{.9f, 4.f}, 
		};
	const auto simulationParameters 
			= std::make_unique<SimulationParameters<profile>>(
					totalEnergy, 
					mass
				);
	const auto virtualStarterRegion = VirtualRegionCoefficients<profile>(
			simulationParameters, 
			regionParameters[regionParameters.size() - 1], 
			startBoundry
		);
	std::cout << "Virtual Starter Region: " << virtualStarterRegion << "\n";
	std::vector<VirtualRegionCoefficients<profile>> virtualRegions;
	virtualRegions.push_back(virtualStarterRegion);
	std::vector<RegionCoefficients<profile>> regions;
	for(size_t ii = 0; ii < regionParameters.size() - 1; ++ii)
	{
		regions.push_back(RegionCoefficients<profile>(
				simulationParameters, 
				regionParameters[ii], 
				virtualRegions.back()
			));
		virtualRegions.push_back(regions.back().toVirtualRegionCoefficients());
		std::cout << regions.back() << "\n";
	}
	//QApplication application(argc, args);
	//QMainWindow window;
	//window.setCentralWidget(new ChartView());
	//window.resize(400, 300);
	//window.show();
	//return application.exec();
	return 0;
}

