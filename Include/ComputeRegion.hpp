#include <Data.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__COMPUTE__REGION__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__COMPUTE__REGION__HPP

template<auto ProfileTagParamterConstant = defaultProfile>
struct ScatteringCoefficients
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ConstantsType = Constants<profileTag>;
	using ScalarType = ConstantsType::ScalarType;
	const ScalarType transmission;
	const ScalarType reflection;
};

template<auto ProfileTagParamterConstant = defaultProfile>
using BoundryCoefficientsType = ScatteringCoefficients<ProfileTagParamterConstant>;

template<auto ProfileTagParamterConstant = defaultProfile>
static auto computeHarmonicConstant(
		const typename Constants<ProfileTagParamterConstant>::ScalarType potential, 
		const typename Constants<ProfileTagParamterConstant>::ScalarType totalEnergy, 
		const typename Constants<ProfileTagParamterConstant>::ScalarType mass
	) -> const typename Constants<ProfileTagParamterConstant>::ScalarType
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
static auto computeTransmissionReflectionRatios(
		const typename Constants<ProfileTagParamterConstant>::ScalarType fromHarmonicConstant, 
		const typename Constants<ProfileTagParamterConstant>::ScalarType toHarmonicConstant
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

	
		
#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__COMPUTE__REGION__HPP

