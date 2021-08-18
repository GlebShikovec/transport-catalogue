#include "transport_catalogue.h"

void TransportCatalogue::AddBus(std::string& busNumber, std::unordered_set<const Stop*> route, ERouteType routeType,
	size_t uniqueStops, size_t stopsOnRoute, double routeLength)
{
	m_busesInfo.emplace_back(new Bus{ busNumber, route, routeType, uniqueStops, stopsOnRoute, routeLength });
	m_buses.emplace(std::make_pair(std::move(busNumber), m_busesInfo.back()));
}

void TransportCatalogue::AddStop(std::string name, double latitude, double longitude)
{
	m_stopsCoords.emplace_back(new Stop{ latitude, longitude });
	m_stops.emplace(std::make_pair(std::move(name), m_stopsCoords.back()));
}

std::optional<const Bus*> TransportCatalogue::FindBus(const std::string& name) const
{
	auto it = m_buses.find(name);
	return it == m_buses.end() ? std::nullopt : std::optional<const Bus*>{ it->second };
}

const Stop* TransportCatalogue::FindStop(const std::string& name) const
{
	return m_stops.find(name)->second;
}

std::tuple<std::string, size_t, size_t, double> TransportCatalogue::GetRouteInfo(const Bus * ptr) const
{
	return std::make_tuple(ptr->busNumber, ptr->uniqueStops, ptr->stopsOnRoute, ptr->routeLength);
}

