//#include <ComputeRegion.hpp>
//
//int main(int argc, char** args)
//{
//	constexpr const auto profile = Profiles::PhysicallyInaccurateSimple;
//	using ScalarType = Constants<profile>::ScalarType;
//	constexpr const ScalarType totalEnergy = .1f;
//	constexpr const ScalarType mass = 1.f;
//	constexpr const auto startRegionParameters = RegionParameters<profile>{1.f, 4.f};
//	constexpr const auto startBoundry = BoundryCoefficientsType<profile> {.4f, .4f};
//	const auto regionParameters = std::array{
//			RegionParameters<profile>{.8f, 3.f}, 
//			RegionParameters<profile>{.5f, 2.f}, 
//			RegionParameters<profile>{.2f, 1.f}
//		};
//	const auto simulationParameters 
//			= std::make_unique<SimulationParameters<profile>>(
//					totalEnergy, 
//					mass
//				);
//	const auto virtualStarterRegion = VirtualRegionCoefficients<profile>(
//			simulationParameters, 
//			startRegionParameters, 
//			startBoundry
//		);
//	std::cout << "Virtual Starter Region: " << virtualStarterRegion << "\n";
//	std::vector<RegionCoefficients<profile>> regions;
//	regions.push_back(RegionCoefficients<profile>(
//			simulationParameters, 
//			regionParameters[0], 
//			virtualStarterRegion
//		));
//	std::cout << regions.back() << "\n";
//	for(size_t ii = 1; ii < regionParameters.size(); ++ii) {
//		regions.push_back(regions.back().makeNext(regionParameters[ii]));
//		std::cout << regions.back() << "\n";
//	}
//	std::vector<Data<profile>> waveValues;
//	waveValues.push_back(
//			computeWaveFunction<profile>(
//					regions[regions.size() - 1], 
//					virtualStarterRegion, 
//					.01f
//				)
//		);
//	for(size_t ii = regions.size() - 1; ii >= 1; --ii)
//	{
//		waveValues.push_back(
//				computeWaveFunction<profile>(
//						regions[ii - 1], 
//						regions[ii], 
//						.01f
//					)
//			);
//	}
//	std::cout << waveValues.size() << "\n";
//	const auto buffers = consolidateData<profile>(waveValues);
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImPlot::CreateContext();
//	ImGui::GetIO().DisplaySize = ImVec2(400.f, 400.f);
//    ImGui::GetIO().Fonts->Build();
//	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
//	for(size_t ii = 0; ii < 1000000; ++ii)
//		plot(buffers);
//	ImPlot::DestroyContext();
//	ImGui::DestroyContext();
//}
//
