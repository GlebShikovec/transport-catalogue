#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

int main()
{
	TransportCatalogue transportCatalogue;
	input::UpdateTransportCatalogue(transportCatalogue);
	stat_output::TransportCatalogueRequests(transportCatalogue);
	return 0;
}
