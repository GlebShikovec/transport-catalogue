#pragma once
#include <string>
#include <deque>
#include <unordered_map>
#include <tuple>
#include <set>
#include "geo.h"
#include <utility>

enum class ERouteType
{
	Circular,
	Pendulum
};

struct Stop
{
	Coordinates coords;
};

struct Bus
{
	std::string name;
	ERouteType routeType;
	size_t uniqueStops;
	size_t stopsOnRoute;
	double facticalRouteLength;
	double routeCurvature;
};

struct CmpBuses
{
public:
	bool operator() (const Bus* lhs, const Bus* rhs) const
	{
		return lhs->name < rhs->name;
	}
};

struct PairPtrHash
{
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const
	{
		auto h1 = std::hash<const void*>{}(p.first);
		auto h2 = std::hash<const void*>{}(p.second);
		std::size_t seed = 0;
		seed ^= h1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

class TransportCatalogue
{
public:
	void AddBus(std::string busNumber, ERouteType routeType, size_t uniqueStops, size_t stopsOnRoute, 
		double facticalRouteLength, double routeCurvature);
	void AddStop(std::string name, double latitude, double longitude);
	void AddBusRelatedToStop(const Stop* stopPtr, const std::string& busNumber);
	const Bus* FindBus(const std::string& name) const;
	const Stop* FindStop(const std::string& name) const;
	std::tuple<size_t, size_t, double, double> GetRouteInfo(const Bus* ptr) const;
	const std::set<const Bus*, CmpBuses>* GetBusesRelatedToStop(const Stop* ptr) const;

	void AddDistanceBetweenStops(const std::string& fromStop, const std::string& toStop, double distance);
	double GetDistanceBetweenStops(const std::string& fromStop, const std::string& toStop);
private:
	std::unordered_map<std::string, const Bus*> m_buses;
	std::deque<Bus> m_busesInfo;
	std::unordered_map<std::string, const Stop*> m_stops;
	std::deque<Stop> m_stopsCoords;
	std::unordered_map<const Stop*, std::set<const Bus*, CmpBuses>> m_busesRelatedToStop;
	std::unordered_map<std::pair<const Stop*, const Stop*>, double, PairPtrHash> m_stopsDistances;
};
