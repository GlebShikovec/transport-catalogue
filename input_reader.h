#pragma once

#include "transport_catalogue.h"

#include <string>

namespace transport_catalogue
{
	namespace input_requests_processing
	{
		namespace detail
		{
			std::string GetToken(std::string& line, const std::string& delimiter);

			double ComputeDistanceBetweenStopsInRoute(const TransportCatalogue& transportCatalogue,
				const std::string& prevStopName, const std::string& currStopName);
		}

		void UpdateTransportCatalogue(TransportCatalogue& transportCatalogue);
	}
}
