#include "stat_reader.h"
#include <iostream>
#include <string>
#include <optional>
#include <iomanip>
#include <vector>

namespace stat_output
{
	using namespace std::literals::string_literals;

	void TransportCatalogueRequests(const TransportCatalogue& transportCatalogue)
	{
		std::string line;
		std::getline(std::cin, line);
		size_t numberOfRequests = std::stoul(line);
		std::vector<std::string> routeInfoRequests;
		for (size_t i = 0; i < numberOfRequests; ++i)
		{
			std::getline(std::cin, line);
			std::string delimiter = " "s;
			size_t pos = line.find(delimiter);
			std::string requestName = line.substr(0, pos);

			if (requestName == "Bus"s)
			{
				routeInfoRequests.emplace_back(line.substr(pos + delimiter.length()));

			}
		}

		for (const auto& busNumber : routeInfoRequests)
		{
			const std::optional<const Bus *> bus = transportCatalogue.FindBus(busNumber);
			if (bus.has_value())
			{
				size_t uniqueStops;
				size_t stopsOnRoute;
				double routeLength;
				std::tie(std::ignore, uniqueStops, stopsOnRoute, routeLength) = transportCatalogue.GetRouteInfo(bus.value());
				std::cout << std::setprecision(6) << "Bus " << busNumber << ": " << stopsOnRoute << " stops on route, "
					<< uniqueStops << " unique stops, " << routeLength << " route length" << std::endl;
			}
			else
			{
				std::cout << "Bus " << busNumber << ": not found" << std::endl;
			}
		}
	}
}

