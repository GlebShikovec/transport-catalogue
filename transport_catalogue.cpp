#include "transport_catalogue.h"

void TransportCatalogue::AddBus(std::string busNumber, ERouteType routeType,
	size_t uniqueStops, size_t stopsOnRoute, double routeLength)
{
	m_busesInfo.emplace_back(Bus{ busNumber, routeType, uniqueStops, stopsOnRoute, routeLength });
	m_buses.emplace(std::make_pair(std::move(busNumber), &m_busesInfo.back()));
}

void TransportCatalogue::AddStop(std::string name, double latitude, double longitude)
{
	m_stopsCoords.emplace_back(Stop{ latitude, longitude });
	m_stops.emplace(std::make_pair(std::move(name), &m_stopsCoords.back()));
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

std::tuple<size_t, size_t, double> TransportCatalogue::GetRouteInfo(const Bus* ptr) const
{
	return std::make_tuple(ptr->uniqueStops, ptr->stopsOnRoute, ptr->routeLength);
}

const std::set<const Bus*, CmpBuses>* TransportCatalogue::GetBusesRelatedToStop(const Stop* ptr) const
{
	auto it = m_busesRelatedToStop.find(ptr);
	return it == m_busesRelatedToStop.end() ? nullptr : &it->second;
}

