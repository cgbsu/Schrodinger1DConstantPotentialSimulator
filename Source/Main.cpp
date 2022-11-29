#include <ComputeRegion.hpp>
#include <ChartView.hpp>

int main(int argc, char** args)
{
	constexpr const auto profile = Profiles::PhysicallyInaccurateSimple;
	using ScalarType = Constants<profile>::ScalarType;
	ScalarType totalEnergy = .1f;
	ScalarType mass = 1.f;
	const auto startRegionParameters = RegionParameters<profile>{1.f, 0.f};
	const auto startBoundry = BoundryCoefficientsType<profile> {.4f, .4f};
	const auto regionParameters = std::array{
			RegionParameters<profile>{.8f, 3.f}, 
			RegionParameters<profile>{.5f, 2.f}, 
			RegionParameters<profile>{.2f, 1.f}
		};
	const auto simulationParameters 
			= std::make_unique<SimulationParameters<profile>>(
					totalEnergy, 
					mass
				);
	const auto virtualStarterRegion = VirtualRegionCoefficients<profile>(
			simulationParameters, 
			startRegionParameters, 
			startBoundry
		);
	std::cout << "Virtual Starter Region: " << virtualStarterRegion << "\n";
	std::vector<RegionCoefficients<profile>> regions;
	std::vector<VirtualRegionCoefficients<profile>> virtualRegions{virtualStarterRegion};
	regions.push_back(RegionCoefficients<profile>(
			simulationParameters, 
			regionParameters[0], 
			virtualStarterRegion
		));
	virtualRegions.push_back(regions.back());
	std::cout << regions.back() << "\n";
	for(size_t ii = 1; ii < regionParameters.size(); ++ii)
	{
		regions.push_back(regions.back().makeNext(regionParameters[ii]));
		virtualRegions.push_back(regions.back());
		std::cout << regions.back() << "\n";
	}
	std::vector<Data<profile>> waveValues;
	waveValues.push_back(
			computeWaveFunction<profile>(
					regions[regions.size() - 1], 
					virtualStarterRegion, 
					.01f
				)
		);
	for(size_t ii = 1; ii < virtualRegions.size(); --ii)
	{
		waveValues.push_back(
				computeWaveFunction<profile>(
						regions[ii], 
						virtualRegions[ii - 1], 
						.01f
					)
			);
	}
	std::cout << waveValues.size() << "\n";
	float startBondryReal = 0.f;
	float startBondryImaginary = 0.f;
	float startBondryPotential = 0.f;
	float startBondryLength = 0.f;
	float reasonableMaximum = 1.f;

	auto* window = initializeGUI();
	renderLoop(window, [&]()
		{
			ImGui::VSliderFloat(
					"Start Boundry (Real Part)", 
					ImVec2(18, 160), 
					&startBondryReal, 
					0.f, 
					1.0f, 
					"%.3f", 
					ImGuiSliderFlags_None
				);
			ImGui::SameLine();
			ImGui::VSliderFloat(
					"Start Boundry (Imaginary Part)", 
					ImVec2(18, 160), 
					&startBondryImaginary, 
					0.f, 
					1.0f, 
					"%.3f", 
					ImGuiSliderFlags_None
				);
			ImGui::VSliderFloat(
					"Reasonible Maximum", 
					ImVec2(18, 160), 
					&reasonableMaximum, 
					0.f, 
					100.f, 
					"%.3f", 
					ImGuiSliderFlags_None
				);
			ImGui::SameLine();
			ImGui::VSliderFloat(
					"Total Energy", 
					ImVec2(18, 160), 
					&totalEnergy, 
					0.f, 
					reasonableMaximum, 
					"%.3f", 
					ImGuiSliderFlags_None
				);
			ImGui::SameLine();
			ImGui::VSliderFloat(
					"Mass", 
					ImVec2(18, 160), 
					&mass, 
					0.f, 
					reasonableMaximum, 
					"%.3f", 
					ImGuiSliderFlags_None
				);
			ImGui::SameLine();
			ImGui::VSliderFloat(
					"Start Boundry Potential", 
					ImVec2(18, 160), 
					&startBondryPotential, 
					0.f, 
					totalEnergy, 
					"%.3f", 
					ImGuiSliderFlags_None
				);
			ImGui::SameLine();
			ImGui::VSliderFloat(
					"Start Boundry Length", 
					ImVec2(18, 160), 
					&startBondryLength, 
					0.f, 
					reasonableMaximum, 
					"%.3f", 
					ImGuiSliderFlags_None
				);
			plot(waveValues);
		});
	cleanUpGUI(window);
}

