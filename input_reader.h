#pragma once
#include "transport_catalogue.h"
#include <string>

namespace input
{
	std::string GetToken(std::string& line, const std::string& delimiter);

	double ComputeDistanceBetweenStopsInRoute(const TransportCatalogue& transportCatalogue, 
		const std::string& prevStopName, const std::string& currStopName);

	void UpdateTransportCatalogue(TransportCatalogue& transportCatalogue);
}