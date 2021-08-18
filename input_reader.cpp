#include "input_reader.h"
#include "geo.h"
#include <iostream>
#include <sstream>
#include <set>
#include <unordered_set>

namespace input
{
	using namespace std::literals::string_literals;

	std::string GetToken(std::string& line, const std::string& delimiter)
	{
		size_t pos = line.find(delimiter);
		std::string token = std::move(line.substr(0, pos));
		line.erase(0, pos + delimiter.length());
		return token;
	}

	double ComputeDistanceBetweenStopsInRoute(const TransportCatalogue& transportCatalogue, 
		const std::string& prevStopName, const std::string& currStopName)
	{
		auto prevStop = transportCatalogue.FindStop(prevStopName);
		auto currStop = transportCatalogue.FindStop(currStopName);
		return ComputeDistance({ prevStop->latitude, prevStop->longitude }, { currStop->latitude, currStop->longitude });
	}

	void UpdateTransportCatalogue(TransportCatalogue& transportCatalogue)
	{
		std::stringstream ss_buses;
		std::string line;
		std::getline(std::cin, line);
		size_t numberOfRequests = std::stoul(line);
		for (size_t i = 0; i < numberOfRequests; ++i)
		{
			std::getline(std::cin, line);
			std::string requestType = GetToken(line, " "s);

			if (requestType == "Stop"s)
			{
				std::string stopName = GetToken(line, ": "s);
				double latitude = std::stod(GetToken(line, ", "s));
				double longitude = std::stod(line);
				transportCatalogue.AddStop(std::move(stopName), latitude, longitude);
			}
			else if (requestType == "Bus"s)
			{
				ss_buses << std::move(line) << std::endl;
			}
		}

		//auto cmpBuses = [](const Bus* a, const Bus* b) { return a->name < b->name; };
		std::unordered_map<const Stop*, std::set<const Bus*, CmpBuses>> busesRelatedToStop;
		while (std::getline(ss_buses, line))
		{
			std::string busNumber = GetToken(line, ": "s);
			ERouteType routeType;
			std::string delimiter;
			std::string firstStopName;
			if (line.find(" - "s) == std::string::npos)
			{
				routeType = ERouteType::Circular;
				delimiter = " > "s;
			}
			else
			{
				routeType = ERouteType::Pendulum;
				delimiter = " - "s;
			}
			std::unordered_set<const Stop *> route;
			double routeLength = 0.0;
			std::string prevStopName;
			std::string currStopName;
			size_t stopsOnRoute = 0;
			for (; line.find(delimiter) != std::string::npos; ++stopsOnRoute)
			{
				prevStopName = std::move(currStopName);
				currStopName = GetToken(line, delimiter);
				route.emplace(transportCatalogue.FindStop(currStopName));
				if (stopsOnRoute)
				{
					routeLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, prevStopName, currStopName);
				}
				else
				{
					firstStopName = currStopName;
				}
			}

			if (routeType == ERouteType::Circular)
			{
				++stopsOnRoute;
				routeLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, currStopName, firstStopName);
			}
			else if (routeType == ERouteType::Pendulum)
			{
				stopsOnRoute = stopsOnRoute * 2 + 1;
				prevStopName = std::move(currStopName);
				route.emplace(transportCatalogue.FindStop(line));
				routeLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, prevStopName, line);
				routeLength *= 2;
			}

			transportCatalogue.AddBus(busNumber, routeType, route.size(), stopsOnRoute, routeLength);

			for (const auto& stop : route)
			{
				transportCatalogue.AddBusRelatedToStop(stop, busNumber);
			}
		}
	}
}
