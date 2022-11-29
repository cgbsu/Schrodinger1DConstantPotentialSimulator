#include <ComputeRegion.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

template<auto ProfileTagParamterConstant = defaultProfile>
struct ChartBuffers
{
	using ConstantsType = Constants<ProfileTagParamterConstant>;
	using ScalarType = typename ConstantsType::ScalarType;
	std::vector<ScalarType> xBuffer, yBuffer;
};

template<auto ProfileTagParamterConstant = defaultProfile>
static auto consolidateData(std::vector<Data<ProfileTagParamterConstant>>& regions)
		-> ChartBuffers<ProfileTagParamterConstant> 
{
	using ConstantsType = Constants<ProfileTagParamterConstant>;
	using ScalarType = typename ConstantsType::ScalarType;
	std::vector<ScalarType> xBuffer, yBuffer;
	for(const auto& region : regions) {
		xBuffer.insert(xBuffer.end(), region.x.begin(), region.x.end());
		yBuffer.insert(yBuffer.end(), region.y.begin(), region.y.end());
	}
	return ChartBuffers{xBuffer, yBuffer};
}

template<auto ProfileTagParamterConstant = defaultProfile>
static void plot(const std::vector<Data<ProfileTagParamterConstant>>& data)//const ChartBuffers<ProfileTagParamterConstant>& buffers)
{
	if(ImGui::Begin("1D Schrodinger Constant Potential Simulation"))
	{
		if(ImPlot::BeginPlot("Wave Function"))
		{
			size_t region = 0;
			for(const auto& buffer : data)
			{
				ImPlot::PlotScatter(
						allToString("Region ", region).c_str(), 
						buffer.x.data(), 
						buffer.y.data(), 
						buffer.x.size()
					);
				++region;
			}
			ImPlot::EndPlot();
		}
		else
			std::cerr << "Something went wrong while plotting.\n";
	}
	else
		std::cerr << "ImGui failed to begin!\n";
	ImGui::End();
}

template<auto ProfileTagParamterConstant = defaultProfile>
static auto simulationParametersGui(
		float& reasonableMaximum, 
		const std::unique_ptr<SimulationParameters<ProfileTagParamterConstant>>& simulationParameters, 
		const auto sliderDimensions
	) -> std::unique_ptr<SimulationParameters<ProfileTagParamterConstant>>
{
	constexpr const auto profile = ProfileTagParamterConstant;
	float totalEnergy = simulationParameters->totalEnergy;
	float mass = simulationParameters->mass;
	ImGui::Begin("Global Simulation Parameters");
	ImGui::VSliderFloat(
			"Reasonible Maximum", 
			sliderDimensions, 
			&reasonableMaximum, 
			0.f, 
			100.f, 
			"%.3f", 
			ImGuiSliderFlags_None
		);
	ImGui::SameLine();
	ImGui::VSliderFloat(
			"Total Energy", 
			sliderDimensions, 
			&totalEnergy, 
			0.f, 
			reasonableMaximum, 
			"%.3f", 
			ImGuiSliderFlags_None
		);
	ImGui::SameLine();
	ImGui::VSliderFloat(
			"Mass", 
			sliderDimensions, 
			&mass, 
			0.f, 
			reasonableMaximum, 
			"%.3f", 
			ImGuiSliderFlags_None
		);
	ImGui::End();
	return std::make_unique<SimulationParameters<profile>>(totalEnergy, mass);
}

template<auto ProfileTagParamterConstant = defaultProfile>
static auto virtualRegionParametersGui(
		const float reasonableMaximum, 
		const std::unique_ptr<SimulationParameters<ProfileTagParamterConstant>>& simulationParameters, 
		const VirtualRegionCoefficients<ProfileTagParamterConstant> current, 
		const auto sliderDimensions
	)
		-> const VirtualRegionCoefficients<ProfileTagParamterConstant>
{
	constexpr const auto profile = ProfileTagParamterConstant;
	float startBondryTransmissivePart = current.boundry.transmission;
	float startBondryReflectivePart = current.boundry.reflection;
	float startBondryPotential = current.regionParameters.potential;
	float startBondryLength = current.regionParameters.length;
	ImGui::Begin("Virtual Starter Region Parameters");
	ImGui::VSliderFloat(
			"Start Boundry (Transmission Part)", 
			sliderDimensions, 
			&startBondryTransmissivePart, 
			0.f, 
			1.0f, 
			"%.3f", 
			ImGuiSliderFlags_None
		);
	ImGui::SameLine();
	ImGui::VSliderFloat(
			"Start Boundry (Reflection Part)", 
			sliderDimensions, 
			&startBondryReflectivePart, 
			0.f, 
			1.0f, 
			"%.3f", 
			ImGuiSliderFlags_None
		);
	ImGui::SameLine();
	ImGui::VSliderFloat(
			"Start Boundry \"Potential\"", 
			sliderDimensions, 
			&startBondryPotential, 
			0.f, 
			simulationParameters->totalEnergy, 
			"%.3f", 
			ImGuiSliderFlags_None
		);
	ImGui::SameLine();
	ImGui::VSliderFloat(
			"Start Boundry \"Length\"", 
			sliderDimensions, 
			&startBondryLength, 
			0.f, 
			reasonableMaximum, 
			"%.3f", 
			ImGuiSliderFlags_None
		);
	const auto starterRegion = VirtualRegionCoefficients<profile>(
			simulationParameters, 
			RegionParameters<profile>{startBondryPotential, startBondryLength}, 
			BoundryCoefficientsType<profile>{
					startBondryTransmissivePart, 
					startBondryReflectivePart
				}
		);
	ImGui::Text(
		"%s", 
		allToString(
			"Start Region Harmonic Constant: ", 
			starterRegion.harmonicConstant
		).c_str());
	ImGui::End();
	return starterRegion;
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static auto* initializeGUI()
{
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) {
		std::cerr << "GLFW failed to initialize!\n";
	}
	auto* window = glfwCreateWindow(1800, 800, "Graph", NULL, NULL);
	if(!window) {
		glfwTerminate();
		std::cout << "GLFW failed to create a window!\n";
		return window;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImPlot::CreateContext();
	//ImGui::GetIO().DisplaySize = ImVec2(400.f, 400.f);
    ImGui::GetIO().Fonts->Build();
	return window;
}

static void cleanUpGUI(auto* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void renderLoop(auto* window, auto job)
{
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();	
		job();
        ImGui::Render();
		int display_w, display_h;
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

