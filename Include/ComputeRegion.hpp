#include <Data.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__COMPUTE__REGION__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__COMPUTE__REGION__HPP

template<auto ProfileTagParamterConstant = defaultProfile>
struct ScatteringCoefficients
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ThisType = ScatteringCoefficients<profileTag>;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	const ScalarType transmission;
	const ScalarType reflection;
	const std::string toString() const noexcept
	{
		return allToString(
				"{.transmission=", transmission, 
				", .reflection=", reflection, 
				"}"
			);
	}
	friend std::ostream& operator<<(
				std::ostream& outputMediaStream, 
				const ThisType& coeffcients
			) {
		outputMediaStream << coeffcients.toString();	
		return outputMediaStream;
	}
};

template<auto ProfileTagParamterConstant = defaultProfile>
using BoundryCoefficientsType = ScatteringCoefficients<ProfileTagParamterConstant>;

template<auto ProfileTagParamterConstant = defaultProfile>
using AmplitudeCoefficientsType = ScatteringCoefficients<ProfileTagParamterConstant>;

template<auto ProfileTagParamterConstant = defaultProfile>
struct SimulationParameters
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ThisType = SimulationParameters<profileTag>;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	const ScalarType totalEnergy;
	const ScalarType mass;
	const std::string toString() const noexcept
	{
		return allToString(
				"{.totalEnergy=", totalEnergy, 
				", .mass=", mass, 
				"}"
			);
	}
	friend std::ostream& operator<<(
				std::ostream& outputMediaStream, 
				const ThisType& simulationParameters
			) {
		outputMediaStream << simulationParameters.toString();
		return outputMediaStream;
	}
};

template<auto ProfileTagParamterConstant = defaultProfile>
struct RegionParameters
{
	constexpr const static auto profileTag = ProfileTagParamterConstant;
	using ThisType = RegionParameters<profileTag>;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	ScalarType potential;
	ScalarType length;
	const std::string toString() const noexcept
	{
		return allToString(
				"{.potential=", potential, 
				", .length=", length, 
				"}"
			);
	}
	friend std::ostream& operator<<(
				std::ostream& outputMediaStream, 
				const ThisType& regionParameters
			) {
		outputMediaStream << regionParameters.toString();
		return outputMediaStream;
	}
};

template<auto ProfileTagParamterConstant = defaultProfile>
static auto computeHarmonicConstant(
		const ProfileScalarType<ProfileTagParamterConstant> potential, 
		const ProfileScalarType<ProfileTagParamterConstant> totalEnergy, 
		const ProfileScalarType<ProfileTagParamterConstant> mass
	) -> const ProfileScalarType<ProfileTagParamterConstant>
{
	using ConstantsType = Constants<ProfileTagParamterConstant>;
	using ScalarType = typename ConstantsType::ScalarType;
	constexpr const auto reducedPlancksConstant 
			= Constants<ProfileTagParamterConstant>::reducedPlancksConstant;
	constexpr static const auto squareRoot = ConstantsType::squareRoot;
	return (
			ConstantsType::squareRootOfTwo 
			* squareRoot(potential - totalEnergy) 
			* squareRoot(mass)
		) / reducedPlancksConstant;
}

template<auto ProfileTagParamterConstant = defaultProfile>
static auto computeHarmonicConstant(
			const std::unique_ptr<SimulationParameters<ProfileTagParamterConstant>>& simulationParameters, 
			const RegionParameters<ProfileTagParamterConstant> regionParameters
		) -> const ProfileScalarType<ProfileTagParamterConstant> {
	return computeHarmonicConstant(
			regionParameters.potential, 
			simulationParameters->totalEnergy, 
			simulationParameters->mass
		);
}
template<auto ProfileTagParamterConstant = defaultProfile>
static auto computeTransmissionReflectionRatios(
		const ProfileScalarType<ProfileTagParamterConstant> fromHarmonicConstant, 
		const ProfileScalarType<ProfileTagParamterConstant> toHarmonicConstant
	) -> ScatteringCoefficients<ProfileTagParamterConstant>
{
	constexpr const auto profileTag = ProfileTagParamterConstant;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = typename ConstantsType::ScalarType;
	const ScalarType commonDenomonator (toHarmonicConstant + fromHarmonicConstant);
	return ScatteringCoefficients<profileTag>{
			(ScalarType{2} * fromHarmonicConstant) / commonDenomonator, 
			(fromHarmonicConstant - toHarmonicConstant) / commonDenomonator
		};
}

template<auto ProfileTagParamterConstant = defaultProfile>
static auto computeBoundry(
		const BoundryCoefficientsType<ProfileTagParamterConstant> fromBoundry, 
		const ScatteringCoefficients<ProfileTagParamterConstant> scatteringCoefficients
	) -> BoundryCoefficientsType<ProfileTagParamterConstant>
requires(isComplex<typename Constants<ProfileTagParamterConstant>::ScalarType> == false)
{
	constexpr const auto profileTag = ProfileTagParamterConstant;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	const ScalarType transmission = scatteringCoefficients.transmission;
	const ScalarType reflection = scatteringCoefficients.reflection;
	const ScalarType transmissionSquareRoot = ConstantsType::squareRoot(transmission);
	const ScalarType conjugateTransmissionSquareRoot 
			= ConstantsType::squareRoot(transmission);
	const ScalarType reflectionSquareRoot = ConstantsType::squareRoot(reflection);
	const ScalarType conjugateReflectionSquareRoot 
			= ConstantsType::squareRoot(reflection);
	// Seperated for numerical stability
	// Transmission //
	const ScalarType reflectionTransmissionTerm = (reflectionSquareRoot * fromBoundry.reflection) 
				/ conjugateTransmissionSquareRoot;
	const ScalarType intermediateTransmissionTransmissionTerm = -((reflectionSquareRoot * conjugateReflectionSquareRoot) 
				/ conjugateTransmissionSquareRoot);
	const ScalarType transmissionTransmissionTerm = (
			(reflectionTransmissionTerm + intermediateTransmissionTransmissionTerm) 
			* fromBoundry.transmission
		);
	const ScalarType toBoundryTransmission = reflectionTransmissionTerm + transmissionTransmissionTerm;
	// Reflection //
	const ScalarType reflectionReflectionTerm = (fromBoundry.reflection / conjugateTransmissionSquareRoot);
	const ScalarType transmissionReflectionTerm = fromBoundry.transmission 
			* (conjugateReflectionSquareRoot / conjugateTransmissionSquareRoot);
	const ScalarType toBoundryReflection = reflectionReflectionTerm + transmissionReflectionTerm;
	return BoundryCoefficientsType<profileTag>{
			toBoundryTransmission, 
			toBoundryReflection
		};
}

template<auto ProfileTagParamterConstant = defaultProfile>
static auto computeAmplitudeCoefficients( // TODO: Current is next, next is current
		const ProfileScalarType<ProfileTagParamterConstant> currentLength, 
		const ProfileScalarType<ProfileTagParamterConstant> nextLength, 
		const ProfileScalarType<ProfileTagParamterConstant> nextHarmonicConstant, 
		const BoundryCoefficientsType<ProfileTagParamterConstant> currentBoundry_, 
		const BoundryCoefficientsType<ProfileTagParamterConstant> nextBoundry_
	)
{
	constexpr const auto profileTag = ProfileTagParamterConstant;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	constexpr const auto exponential = ConstantsType::exponential;
	constexpr const auto raise = ConstantsType::raise;
	using CoefficientTermsType = ScatteringCoefficients<profileTag>;
	const auto termCoefficients = CoefficientTermsType {
			exponential(nextLength * nextHarmonicConstant), 
			exponential(currentLength * nextHarmonicConstant)
		};
	const auto termCoefficientsSquare = CoefficientTermsType {
			raise(termCoefficients.transmission, 2), 
			raise(termCoefficients.reflection, 2)
		};
	const auto currentBoundry = currentBoundry_.transmission + currentBoundry_.reflection;
	const auto nextBoundry = nextBoundry_.transmission + nextBoundry_.reflection;
	const ScalarType commonDenomonator 
			= termCoefficientsSquare.reflection - termCoefficientsSquare.transmission;
	const ScalarType transmissionNumerator 
			= (currentBoundry * termCoefficients.reflection) 
			- (nextBoundry * termCoefficients.transmission);
	const ScalarType reflectionNumeratorScalar 
			= termCoefficients.transmission * termCoefficientsSquare.reflection;
	const ScalarType reflectionNumerator 
			= (nextBoundry * reflectionNumeratorScalar) 
					- (currentBoundry * reflectionNumeratorScalar);
	return AmplitudeCoefficientsType<profileTag>{
			transmissionNumerator / commonDenomonator, 
			reflectionNumerator / commonDenomonator
		};
}

template<auto ProfileTagParamterConstant = defaultProfile>
struct RegionCoefficients;

template<auto ProfileTagParamterConstant = defaultProfile>
struct VirtualRegionCoefficients
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ThisType = VirtualRegionCoefficients<profileTag>;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	const ScalarType harmonicConstant;
	const BoundryCoefficientsType<profileTag> boundry;
	const RegionParameters<profileTag> regionParameters;
	const std::unique_ptr<SimulationParameters<profileTag>>& simulationParameters;

	explicit VirtualRegionCoefficients(
			const std::unique_ptr<SimulationParameters<profileTag>>& simulationParameters, 
			const RegionParameters<profileTag> regionParameters, 
			const BoundryCoefficientsType<profileTag> boundry
		) noexcept : 
			harmonicConstant(computeHarmonicConstant<profileTag>(simulationParameters, regionParameters)), 
			boundry(boundry), 
			regionParameters(regionParameters), 
			simulationParameters(simulationParameters)
		{}
	std::string toString() const noexcept
	{
		return allToString(
				"{.harmonicConstant=", harmonicConstant, 
				", .boundry=", boundry, 
				", .regionParameters=", regionParameters, 
				"}"
			);
	}
	friend std::ostream& operator<<(
				std::ostream& outputMediaStream, 
				const ThisType& virtualRegion
			) {
		outputMediaStream << virtualRegion.toString();
		return outputMediaStream;
	}

	constexpr auto makeNext(
			const RegionParameters<profileTag> regionParameters
		) const noexcept
	{
		return RegionCoefficients<profileTag>(
				simulationParameters, 
				regionParameters, 
				*this
			);
	}
};

template<auto ProfileTagParamterConstant>
struct RegionCoefficients
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	using ThisType = RegionCoefficients<profileTag>;
	using VirtualRegionType = VirtualRegionCoefficients<profileTag>;
	const ScalarType harmonicConstant;
	const ScatteringCoefficients<profileTag> derivedScatteringCoefficients;
	const BoundryCoefficientsType<profileTag> boundry;
	const AmplitudeCoefficientsType<profileTag> amplitudeCoefficients;
	const RegionParameters<profileTag> regionParameters;
	const std::unique_ptr<SimulationParameters<profileTag>>& simulationParameters;
	constexpr explicit RegionCoefficients(
			const std::unique_ptr<SimulationParameters<profileTag>>& simulationParameters, 
			const RegionParameters<profileTag> regionParameters, 
			const VirtualRegionType from
		) noexcept : 
			harmonicConstant(computeHarmonicConstant<profileTag>(simulationParameters, regionParameters)), 
			derivedScatteringCoefficients(computeTransmissionReflectionRatios<profileTag>(
					from.harmonicConstant, 
					harmonicConstant
				)), 
			boundry(computeBoundry<profileTag>(from.boundry, derivedScatteringCoefficients)), 
			amplitudeCoefficients(computeAmplitudeCoefficients<profileTag>(
					from.regionParameters.length, 
					regionParameters.length, 
					harmonicConstant, 
					from.boundry, 
					boundry
				)), 
			regionParameters(regionParameters), 
			simulationParameters(simulationParameters)
		{}
	
	constexpr ThisType makeNext(
			const RegionParameters<profileTag> regionParameters
		) const noexcept
	{
		return ThisType(
				simulationParameters, 
				regionParameters, 
				toVirtualRegionCoefficients()
			);
	}
	
	constexpr VirtualRegionType toVirtualRegionCoefficients() const noexcept {
		return VirtualRegionType(simulationParameters, regionParameters, boundry);
	}
	constexpr operator VirtualRegionType() const noexcept {
		return toVirtualRegionCoefficients();
	}
	std::string toString() const noexcept
	{
		return allToString(
				"{.harmonicConstant=", harmonicConstant, 
				", .derivedScatteringCoefficients=", derivedScatteringCoefficients, 
				", .boundry=", boundry, 
				", .amplitudeCoefficients=", amplitudeCoefficients, 
				", .regionParameters=", regionParameters, 
				"}"
			);
	}
	friend std::ostream& operator<<(
				std::ostream& outputMediaStream, 
				const ThisType& region
			) {
		outputMediaStream << region.toString();
		return outputMediaStream;
	}
};

template<auto ProfileTagParamterConstant = defaultProfile>
static auto computeWaveFunction(
		const RegionCoefficients<ProfileTagParamterConstant> current, 
		const VirtualRegionCoefficients<ProfileTagParamterConstant> from, 
		const ProfileScalarType<ProfileTagParamterConstant> step
	)
{
	using ConstantsType = Constants<ProfileTagParamterConstant>;
	using ScalarType = typename ConstantsType::ScalarType;
	constexpr static const auto exponential = ConstantsType::exponential;
	const auto transmission = current.amplitudeCoefficients.transmission;
	const auto reflection = current.amplitudeCoefficients.reflection;
	const auto harmonicConstant = current.harmonicConstant;
	std::vector<ScalarType> probabilityDensities;
	std::vector<ScalarType> positions;
	const ScalarType start = from.regionParameters.length; //std::min(from.regionParameters.length, current.regionParameters.length); 
	const ScalarType stop = current.regionParameters.length; //std::max(from.regionParameters.length, current.regionParameters.length); 
	const ScalarType stepScalar = start > stop ? -1 : 1;
	for(ScalarType position = start; position <= stop; position += (step * stepScalar))
	{
		probabilityDensities.push_back(
				(transmission * exponential(position * harmonicConstant)) 
						+ (reflection * exponential(-position * harmonicConstant))
			);
		positions.push_back(position);
	}
	return Data<ProfileTagParamterConstant>{
			Region<ProfileTagParamterConstant>{from.regionParameters.length, current.regionParameters.length}, 
			positions, 
			probabilityDensities
		};
}

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__COMPUTE__REGION__HPP

