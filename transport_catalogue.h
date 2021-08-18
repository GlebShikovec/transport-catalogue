#pragma once
#include <string>
#include <deque>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <tuple>

enum class ERouteType
{
	Circular,
	Pendulum
};

struct Stop
{
	double latitude;
	double longitude;
};

struct Bus
{
	std::string busNumber;
	std::unordered_set<const Stop *> route;
	ERouteType routeType;
	size_t uniqueStops;
	size_t stopsOnRoute;
	double routeLength;
};

class TransportCatalogue
{
public:
	void AddBus(std::string& busNumber, std::unordered_set<const Stop *> route, ERouteType routeType, size_t uniqueStops, size_t stopsOnRoute, double routeLength);
	void AddStop(std::string name, double latitude, double longitude);
	std::optional<const Bus*> FindBus(const std::string& name) const;
	const Stop* FindStop(const std::string& name) const;
	std::tuple<std::string, size_t, size_t, double> GetRouteInfo(const Bus* ptr) const;
private:
	std::unordered_map<std::string, const Bus*> m_buses;
	std::deque<const Bus*> m_busesInfo;
	std::unordered_map<std::string, const Stop*> m_stops;
	std::deque<const Stop*> m_stopsCoords;
};
