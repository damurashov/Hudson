#ifndef OSM_XML_H
#define OSM_XML_H

namespace ns_osm {

struct Xml_Handler::Osm_Xml {
	static const char* BOUNDS;
	static const char* MINLAT;
	static const char* MINLON;
	static const char* MAXLAT;
	static const char* MAXLON;
	static const char* ID;
	static const char* VISIBLE;
	static const char* VERSION;
	static const char* CHANGESET;
	static const char* TIMESTAMP;
	static const char* USER;
	static const char* UID;
	static const char* LAT;
	static const char* LON;
	static const char* K;
	static const char* V;
	static const char* REF;
	static const char* TYPE;
	static const char* ROLE;

	static const char* OSM;
	static const char* NODE;
	static const char* WAY;
	static const char* RELATION;
	static const char* MEMBER;
	static const char* TAG;
	static const char* ND;
};

}/* namespace */
#endif // OSM_XML_H
