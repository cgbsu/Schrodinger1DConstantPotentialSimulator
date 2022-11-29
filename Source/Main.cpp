#include <ComputeRegion.hpp>
#include "imgui.h"
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"
#include "implot.h"
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char** args)
{
	constexpr const auto profile = Profiles::PhysicallyInaccurateSimple;
	using ScalarType = Constants<profile>::ScalarType;
	constexpr const ScalarType totalEnergy = .1f;
	constexpr const ScalarType mass = 1.f;
	constexpr const auto startRegionParameters = RegionParameters<profile>{1.f, 0.f};
	constexpr const auto startBoundry = BoundryCoefficientsType<profile> {.4f, .4f};
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
	const auto buffers = consolidateData<profile>(waveValues);
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) {
		std::cerr << "GLFW failed to initialize!\n";
		return 1;
	}
	auto* window = glfwCreateWindow(400, 400, "Graph", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		std::cout << "GLFW failed to create a window!\n";
		return 2;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImPlot::CreateContext();
	ImGui::GetIO().DisplaySize = ImVec2(400.f, 400.f);
    ImGui::GetIO().Fonts->Build();
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();	
		plot(buffers);
        ImGui::Render();
		int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

