#ifndef RESULT_CODES_H
#define RESULT_CODES_H

const int OSM_OK						= 0;
const int OSM_ERROR						= 1; /* Unspecified error */
const int OSM_ERROR_WRONG_XML_FORMAT	= 2; /* Unable to parse xml file */
const int OSM_ERROR_XML_FILE_NOT_EXISTS	= 3; /* No such xml file */

class Osm_Message {
public:
	                Osm_Message	();
					Osm_Message	(const Osm_Message&) = delete;
	Osm_Message&	operator=	(const Osm_Message&) = delete;
	const char*		operator()	(const int);
};

#endif // RESULT_CODES_H
