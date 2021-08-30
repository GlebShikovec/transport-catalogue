#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

int main()
{
	using namespace transport_catalogue;

	TransportCatalogue transportCatalogue;
	input_requests_processing::UpdateTransportCatalogue(transportCatalogue);
	output_requests_processing::TransportCatalogueRequests(transportCatalogue);

	return 0;
}
