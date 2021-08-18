#include "transport_catalogue.h"

void TransportCatalogue::AddBus(std::string busNumber, ERouteType routeType,
	size_t uniqueStops, size_t stopsOnRoute, double facticalRouteLength, double routeCurvature)
{
	m_busesInfo.emplace_back(Bus{ busNumber, routeType, uniqueStops, stopsOnRoute, facticalRouteLength, routeCurvature });
	m_buses.emplace(std::move(busNumber), &m_busesInfo.back());
}

void TransportCatalogue::AddStop(std::string name, double latitude, double longitude)
{
	m_stopsCoords.emplace_back(Stop{ latitude, longitude });
	m_stops.emplace(std::move(name), &m_stopsCoords.back());
}

void TransportCatalogue::AddBusRelatedToStop(const Stop* stopPtr, const std::string& busNumber)
{
	m_busesRelatedToStop[stopPtr].emplace(FindBus(busNumber));
}

const Bus* TransportCatalogue::FindBus(const std::string& name) const
{
	auto it = m_buses.find(name);
	return it == m_buses.end() ? nullptr : it->second;
}

const Stop* TransportCatalogue::FindStop(const std::string& name) const
{
	auto it = m_stops.find(name);
	return it == m_stops.end() ? nullptr : it->second;
}

std::tuple<size_t, size_t, double, double> TransportCatalogue::GetRouteInfo(const Bus* ptr) const
{
	return std::make_tuple(ptr->uniqueStops, ptr->stopsOnRoute, ptr->facticalRouteLength, ptr->routeCurvature);
}

const std::set<const Bus*, CmpBuses>* TransportCatalogue::GetBusesRelatedToStop(const Stop* ptr) const
{
	auto it = m_busesRelatedToStop.find(ptr);
	return it == m_busesRelatedToStop.end() ? nullptr : &it->second;
}

void TransportCatalogue::AddDistanceBetweenStops(const std::string& fromStop, const std::string& toStop, double distance)
{
	m_stopsDistances.emplace(std::make_pair(FindStop(fromStop), FindStop(toStop)), distance);
}

double TransportCatalogue::GetDistanceBetweenStops(const std::string& fromStop, const std::string& toStop)
{
	double geodesicalDistance;
	auto itForward = m_stopsDistances.find(std::make_pair(FindStop(fromStop), FindStop(toStop)));
	if (itForward == m_stopsDistances.end())
	{
		auto itBack = m_stopsDistances.find(std::make_pair(FindStop(toStop), FindStop(fromStop)));
		geodesicalDistance = itBack->second;
	}
	else
	{
		geodesicalDistance = itForward->second;
	}
	return geodesicalDistance;
}

