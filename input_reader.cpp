#include "input_reader.h"
#include "geo.h"
#include <iostream>
#include <sstream>
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
		return ComputeDistance(transportCatalogue.FindStop(prevStopName)->coords,
			transportCatalogue.FindStop(currStopName)->coords);
	}

	void UpdateTransportCatalogue(TransportCatalogue& transportCatalogue)
	{
		// Stops processing.
		std::stringstream ssBusRequests;
		std::stringstream ssStopRequests;
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
				double longitude = std::stod(GetToken(line, ", "s));
				ssStopRequests << stopName << ": "s << line << std::endl;
				transportCatalogue.AddStop(std::move(stopName), latitude, longitude);
			}
			else if (requestType == "Bus"s)
			{
				ssBusRequests << std::move(line) << std::endl;
			}
		}

		// Process distances between stops.
		while (std::getline(ssStopRequests, line))
		{
			std::string fromStop = GetToken(line, ": "s);
			while (line.find("m "s) != std::string::npos)
			{
				unsigned int distance = std::stoul(GetToken(line, "m to "s));
				std::string toStop = GetToken(line, ", "s);
				transportCatalogue.AddDistanceBetweenStops(fromStop, toStop, distance);
			}
		}

		// Process buses and related stops.
		while (std::getline(ssBusRequests, line))
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
			double geographicRouteLength = 0.0;
			unsigned int facticalRouteLength = 0;
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
					// TODO DRY
					geographicRouteLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, prevStopName, currStopName);;
					facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(prevStopName, currStopName);
					if (routeType == ERouteType::Pendulum)
					{
						facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(currStopName, prevStopName);
					}
				}
				else
				{
					firstStopName = currStopName;
				}
			}

			if (routeType == ERouteType::Circular)
			{
				++stopsOnRoute;
				geographicRouteLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, currStopName, firstStopName);
				facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(currStopName, firstStopName);
			}
			else if (routeType == ERouteType::Pendulum)
			{
				stopsOnRoute = stopsOnRoute * 2 + 1;
				prevStopName = std::move(currStopName);
				currStopName = GetToken(line, delimiter);
				route.emplace(transportCatalogue.FindStop(currStopName));
				geographicRouteLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, prevStopName, currStopName);
				facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(prevStopName, currStopName) + 
					transportCatalogue.GetDistanceBetweenStops(currStopName, prevStopName);
				geographicRouteLength *= 2;
			}

			transportCatalogue.AddBus(busNumber, routeType, route.size(), stopsOnRoute, facticalRouteLength, 
				static_cast<double>(facticalRouteLength) / geographicRouteLength);

			for (const auto& stop : route)
			{
				transportCatalogue.AddBusRelatedToStop(stop, busNumber);
			}
		}
	}
}
