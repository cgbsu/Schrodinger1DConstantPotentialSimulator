#include <Data.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

QT_USE_NAMESPACE

template<auto ProfileTagParamterConstant = defaultProfile>
struct ChartView : public QChartView
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using DataSetType = Data<profileTag>;
	ChartView(QWidget* parent, std::initializer_list<DataSetType> regions)
			: QChartView(new QChart(), parent), regions(scatterSeriesFromData(regions)) {
		for(QScatterSeries* region : this->regions)
			this->chart()->addSeries(region);
	}
	ChartView(QWidget* parent = nullptr) : QChartView(new QChart(), parent) {}
	virtual ~ChartView() {}
	bool replaceRegion(size_t regionIndex, const DataSetType& region)
	{
		if(regionIndex < regions.size())
		{
			chart()->removeSeries(regions[regionIndex]);
			chart()->addSeries(region.toScatterSeries());
			chart()->createDefaultAxes();
			return true;
		}
		return false;
	}
	protected: 
		std::vector<QScatterSeries*> regions;
};

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

