#include <Constants.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__DATA__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__DATA__HPP

QT_USE_NAMESPACE
using namespace QtCharts;

template<auto ProfileTagParamterConstant = defaultProfile>
struct Region
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ScalarType = Constants<profileTag>::ScalarType;
	const ScalarType from;
	const ScalarType to;
};

template<auto ProfileTagParamterConstant = defaultProfile>
struct Data
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ScalarType = Constants<profileTag>::ScalarType;
	const Region<profileTag> region;
	const std::vector<ScalarType> data;
	constexpr const ScalarType stepSize() const noexcept {
		return (region.to - region.from) / static_cast<ScalarType>(data.size());
	}

	QScatterSeries* toScatterSeries() const noexcept
	{
		QScatterSeries* newSeries = new QScatterSeries;
		const size_t pointCount = data.size();
		const ScalarType stepSize = this->stepSize();
		for(size_t ii = 0; ii < pointCount; ++ii)
			newSeries->append(region.from + stepSize * ii, data[ii]);
		return newSeries;
	}
};

template<auto ProfileTagParamterConstant = defaultProfile>
static const std::vector<QScatterSeries*> scatterSeriesFromData(
		const std::vector<Data<ProfileTagParamterConstant>> data
	)
{
	std::vector<QScatterSeries*> series;
	for(const auto& region : data)
		series.push_back(region.toScatterSeries());
	return series;
}

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__DATA__HPP

