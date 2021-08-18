#include "stat_reader.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>

namespace stat_output
{
	using namespace std::literals::string_literals;

	void TransportCatalogueRequests(const TransportCatalogue& transportCatalogue)
	{
		std::stringstream requests;
		std::string line;
		std::getline(std::cin, line);
		size_t numberOfRequests = std::stoul(line);
		for (size_t i = 0; i < numberOfRequests; ++i)
		{
			std::getline(std::cin, line);
			requests << std::move(line) << std::endl;
		}

		while (std::getline(requests, line))
		{
			std::string delimiter = " "s;
			size_t pos = line.find(delimiter);
			std::string requestName = line.substr(0, pos);
			std::string request = line.substr(pos + delimiter.length());
			if (requestName == "Bus"s)
			{
				const Bus* bus = transportCatalogue.FindBus(request);
				if (bus != nullptr)
				{
					auto [uniqueStops, stopsOnRoute, routeLength] = transportCatalogue.GetRouteInfo(bus);
					std::cout << std::setprecision(6) << "Bus " << request << ": " << stopsOnRoute << " stops on route, "
						<< uniqueStops << " unique stops, " << routeLength << " route length" << std::endl;
				}
				else
				{
					std::cout << "Bus " << request << ": not found" << std::endl;
				}
			}
			else if (requestName == "Stop"s)
			{
				const Stop* stop = transportCatalogue.FindStop(request);
				if (stop != nullptr)
				{
					auto busesByStop = transportCatalogue.GetBusesRelatedToStop(stop);
					if (busesByStop != nullptr)
					{
						std::cout << "Stop " << request << ": buses";
						for (const auto& bus : *busesByStop)
						{
							std::cout << " " << bus->name;
						}
						std::cout << std::endl;
					}
					else
					{
						std::cout << "Stop " << request << ": no buses" << std::endl;
					}
				}
				else
				{
					std::cout << "Stop " << request << ": not found" << std::endl;
				}
			}
		}
	}
}

