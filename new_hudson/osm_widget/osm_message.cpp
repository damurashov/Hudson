#include "osm_message.h"

Osm_Message::Osm_Message() {}

const char* Osm_Message::operator()(const int code) {
	switch (code) {
	case OSM_OK:
		return "OK! ";
	case OSM_ERROR:
		return "Unspecified error. ";
	case OSM_ERROR_WRONG_XML_FORMAT:
		return "Unable to parse xml file. ";
	case OSM_ERROR_XML_FILE_NOT_EXISTS:
		return "No such xml file. ";
	case OSM_ERROR_CANNOT_WRITE_FILE:
		return "Cannot write file. ";
	}
	return "Unknown result code. ";
}
