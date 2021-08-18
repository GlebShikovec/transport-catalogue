#pragma once
#include <string>
#include <deque>
#include <unordered_map>
#include <tuple>
#include <set>

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
	std::string name;
	ERouteType routeType;
	size_t uniqueStops;
	size_t stopsOnRoute;
	double routeLength;
};

struct CmpBuses
{
	bool operator() (const Bus* lhs, const Bus* rhs) const
	{
		return lhs->name < rhs->name;
	}
};

class TransportCatalogue
{
public:
	void AddBus(std::string busNumber, ERouteType routeType, size_t uniqueStops, size_t stopsOnRoute, double routeLength);
	void AddStop(std::string name, double latitude, double longitude);
	void AddBusRelatedToStop(const Stop* stopPtr, const std::string& busNumber);
	const Bus* FindBus(const std::string& name) const;
	const Stop* FindStop(const std::string& name) const;
	std::tuple<size_t, size_t, double> GetRouteInfo(const Bus* ptr) const;
	const std::set<const Bus*, CmpBuses>* GetBusesRelatedToStop(const Stop* ptr) const;
private:
	std::unordered_map<std::string, const Bus*> m_buses;
	std::deque<Bus> m_busesInfo;
	std::unordered_map<std::string, const Stop*> m_stops;
	std::deque<Stop> m_stopsCoords;
	std::unordered_map<const Stop*, std::set<const Bus*, CmpBuses>> m_busesRelatedToStop;
};
