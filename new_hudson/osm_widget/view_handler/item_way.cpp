#include "item_way.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Way::Item_Way(const Osm_Map& map,
                   Osm_Way& way,
                   QObject *p_parent)
                   : QObject(p_parent),
                     m_map(map),
                     m_way(way)
{
	subscribe(m_way);
	compose_group();
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Item_Way::compose_group() {
	for (auto it = m_edges.begin(); it != m_edges.end(); ++it) {

	}
}
