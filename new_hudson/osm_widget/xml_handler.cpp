#include "xml_handler.h"
using namespace ns_osm;

/* ===================== Constructors, destructors ===================== */

Xml_Handler::Xml_Handler()
    : m_nodes_hash		(*(new QHash<long long, ns_osm::Osm_Node*>)),
      m_ways_hash		(*(new QHash<long long, ns_osm::Osm_Way*>)),
      m_relations_hash	(*(new QHash<long long, ns_osm::Osm_Relation*>)),
      m_bounding_rect	(*(new QRectF)) {}

Xml_Handler::Xml_Handler(QHash<long long, Osm_Node*>& nodes_hash,
                         QHash<long long, Osm_Way*>& ways_hash,
                         QHash<long long, Osm_Relation*>& relations_hash,
                         QRectF& bounding_rect):
    m_nodes_hash(nodes_hash),
    m_ways_hash(ways_hash),
    m_relations_hash(relations_hash),
    m_bounding_rect(bounding_rect) {}

/* ===================== Protected methods ===================== */

void Xml_Handler::load_bound_from_xml(const QDomNode& node) {
	QDomNode	dom_node = node.firstChild();
	QDomElement dom_element;
	double minlat = 0.0;
	double minlon = 0.0;
	double maxlat = 0.0;
	double maxlon = 0.0;

	while (!dom_node.isNull()) {
		if(dom_node.isElement()) {
			dom_element = dom_node.toElement();
			if (!dom_element.isNull()) {
				if (dom_element.tagName() == Osm_Xml::BOUNDS) {
					minlat = dom_element.attribute(Osm_Xml::MINLAT, "").toDouble();
					minlon = dom_element.attribute(Osm_Xml::MINLON, "").toDouble();
					maxlat = dom_element.attribute(Osm_Xml::MAXLAT, "").toDouble();
					maxlon = dom_element.attribute(Osm_Xml::MAXLON, "").toDouble();
					break;
				}
			}
		}
		dom_node = dom_node.nextSibling();
	}

	//m_bounding_rect.setCoords(maxlon, maxlat, minlon, minlat);
	//m_bounding_rect.setCoords(minlon, minlat, maxlon, maxlat);
	m_bounding_rect.setLeft(minlon);
	m_bounding_rect.setBottom(minlat);
	m_bounding_rect.setRight(maxlon);
	m_bounding_rect.setTop(maxlat);
}

void Xml_Handler::load_nodes_from_xml(const QDomNode &node) {
	QDomNode	dom_node_outer = node.firstChild();
	QDomNode	dom_node_inner;
	QDomElement dom_element;
	Osm_Node*	p_node = nullptr;
	QString		attr_id;
	QString		attr_lat;
	QString		attr_lon;

	while (!dom_node_outer.isNull()) {
		p_node = nullptr;
		if (dom_node_outer.isElement()) {
			dom_element = dom_node_outer.toElement();
			if (!dom_element.isNull()) {
				if (dom_element.tagName() == Osm_Xml::NODE) {
					/* Fill attributes */
					attr_id = dom_element.attribute(Osm_Xml::ID, "");
					attr_lat = dom_element.attribute(Osm_Xml::LAT, "");
					attr_lon = dom_element.attribute(Osm_Xml::LON, "");
					p_node = new Osm_Node(attr_id, attr_lat, attr_lon);
					p_node->set_attr(Osm_Xml::VISIBLE, dom_element.attribute(Osm_Xml::VISIBLE, ""));
					p_node->set_attr(Osm_Xml::VERSION, dom_element.attribute(Osm_Xml::VERSION, ""));
					p_node->set_attr(Osm_Xml::CHANGESET, dom_element.attribute(Osm_Xml::CHANGESET, ""));
					p_node->set_attr(Osm_Xml::TIMESTAMP, dom_element.attribute(Osm_Xml::TIMESTAMP, ""));
					p_node->set_attr(Osm_Xml::USER, dom_element.attribute(Osm_Xml::USER, ""));
					p_node->set_attr(Osm_Xml::UID, dom_element.attribute(Osm_Xml::UID, ""));

					/* Fill tags */
					dom_node_inner = dom_node_outer.firstChild();
					while (!dom_node_inner.isNull()) {
						if (dom_node_inner.isElement()) {
							dom_element = dom_node_inner.toElement();
							if (!dom_element.isNull()) {
								if (dom_element.tagName() == Osm_Xml::TAG) {
									p_node->set_tag(dom_element.attribute(Osm_Xml::K, ""), dom_element.attribute(Osm_Xml::V, ""));
								}
							}
						}
						dom_node_inner = dom_node_inner.nextSibling();
					}
				}
			}
		}
		if (p_node != nullptr) {
			m_nodes_hash[p_node->get_id()] = p_node;
		}
		dom_node_outer = dom_node_outer.nextSibling();
	}
}

void Xml_Handler::load_ways_from_xml(const QDomNode &node) {
	QDomNode	dom_node_outer = node.firstChild();
	QDomNode	dom_node_inner;
	QDomElement dom_element;
	Osm_Way*	p_way = nullptr;

	while (!dom_node_outer.isNull()) {
		p_way = nullptr;
		if (dom_node_outer.isElement()) {
			dom_element = dom_node_outer.toElement();
			if (!dom_element.isNull()) {
				if (dom_element.tagName() == Osm_Xml::WAY) {
					/* Fill attributes */
					//attr_id = dom_element.attribute(Osm_Xml::ID, "");
					p_way = new Osm_Way(dom_element.attribute(Osm_Xml::ID, ""));
					p_way->set_attr(Osm_Xml::VISIBLE, dom_element.attribute(Osm_Xml::VISIBLE, ""));
					p_way->set_attr(Osm_Xml::VERSION, dom_element.attribute(Osm_Xml::VERSION, ""));
					p_way->set_attr(Osm_Xml::CHANGESET, dom_element.attribute(Osm_Xml::CHANGESET, ""));
					p_way->set_attr(Osm_Xml::TIMESTAMP, dom_element.attribute(Osm_Xml::TIMESTAMP, ""));
					p_way->set_attr(Osm_Xml::USER, dom_element.attribute(Osm_Xml::USER, ""));
					p_way->set_attr(Osm_Xml::UID, dom_element.attribute(Osm_Xml::UID, ""));

					/* Fill tags and nodes */
					dom_node_inner = dom_node_outer.firstChild();
					while (!dom_node_inner.isNull()) {
						if (dom_node_inner.isElement()) {
							dom_element = dom_node_inner.toElement();
							if (!dom_element.isNull()) {

								/* Fill tags */
								if (dom_element.tagName() == Osm_Xml::TAG) {
									p_way->set_tag(dom_element.attribute(Osm_Xml::K, ""), dom_element.attribute(Osm_Xml::V, ""));

									/* Fill nodes by references */
								} else if (dom_element.tagName() == Osm_Xml::ND) {
									p_way->push_node(m_nodes_hash[dom_element.attribute(Osm_Xml::REF, "").toLongLong()]);
								}
							}
						}
						dom_node_inner = dom_node_inner.nextSibling();
					}
				}
			}
		}
		if (p_way != nullptr) {
			m_ways_hash[p_way->get_id()] = p_way;
		}
		dom_node_outer = dom_node_outer.nextSibling();
	}
}

void Xml_Handler::load_relations_from_xml(const QDomNode &node) {
	struct Pending_Relation {
		Osm_Relation* p_master;
		long long	  slave_id;
		QString		  slave_role;
	};

	QList<Pending_Relation>				pending_relations;
	QList<Pending_Relation>::iterator	it_pr;
	QDomNode							dom_node_outer = node.firstChild();
	QDomNode							dom_node_inner;
	QDomElement 						dom_element;
	Osm_Relation*						p_rel = nullptr;
	QString								attr_id;
	QString								attr_type;
	long long							ref;

	while (!dom_node_outer.isNull()) {
		p_rel = nullptr;
		if (dom_node_outer.isElement()) {
			dom_element = dom_node_outer.toElement();
			if (!dom_element.isNull()) {
				if (dom_element.tagName() == Osm_Xml::RELATION) {

					/* Fill attributes */
					attr_id = dom_element.attribute(Osm_Xml::ID, "");
					p_rel = new Osm_Relation(attr_id);
					p_rel->set_attr(Osm_Xml::VISIBLE, dom_element.attribute(Osm_Xml::VISIBLE, ""));
					p_rel->set_attr(Osm_Xml::VERSION, dom_element.attribute(Osm_Xml::VERSION, ""));
					p_rel->set_attr(Osm_Xml::CHANGESET, dom_element.attribute(Osm_Xml::CHANGESET, ""));
					p_rel->set_attr(Osm_Xml::TIMESTAMP, dom_element.attribute(Osm_Xml::TIMESTAMP, ""));
					p_rel->set_attr(Osm_Xml::USER, dom_element.attribute(Osm_Xml::USER, ""));
					p_rel->set_attr(Osm_Xml::UID, dom_element.attribute(Osm_Xml::UID, ""));

					/* Fill tags and members */
					dom_node_inner = dom_node_outer.firstChild();
					while (!dom_node_inner.isNull()) {
						if (dom_node_inner.isElement()) {
							dom_element = dom_node_inner.toElement();
							if (!dom_element.isNull()) {

								/* Fill tags */
								if (dom_element.tagName() == Osm_Xml::TAG) {
									p_rel->set_tag(dom_element.attribute(Osm_Xml::K, ""), dom_element.attribute(Osm_Xml::V, ""));

								/* Fill members */
								} else if (dom_element.tagName() == Osm_Xml::MEMBER) {
									ref = dom_element.attribute(Osm_Xml::REF, "").toLongLong();
									attr_type = dom_element.attribute(Osm_Xml::TYPE);
									if (attr_type == Osm_Xml::NODE) {
										p_rel->add(m_nodes_hash[ref], dom_element.attribute(Osm_Xml::ROLE));
									} else if (attr_type == Osm_Xml::WAY) {
										p_rel->add(m_ways_hash[ref], dom_element.attribute(Osm_Xml::ROLE));
									} else if (attr_type == Osm_Xml::RELATION) {
										pending_relations.push_front(Pending_Relation());
										Pending_Relation& r_pr = pending_relations.front();
										r_pr.p_master = p_rel;
										r_pr.slave_id = ref;
										r_pr.slave_role = dom_element.attribute(Osm_Xml::ROLE);
									}
								}
							}
						}
						dom_node_inner = dom_node_inner.nextSibling();
					}
				}
			}
		}
		if (p_rel != nullptr) {
			m_relations_hash[p_rel->get_id()] = p_rel;
		}
		dom_node_outer = dom_node_outer.nextSibling();
	}

	/* Handle pending relations */
	for (it_pr = pending_relations.begin(); it_pr != pending_relations.end(); ++it_pr) {
		it_pr->p_master->add(m_relations_hash[it_pr->slave_id], it_pr->slave_role);
	}
}

/* ===================== Public methods ===================== */

bool Xml_Handler::load_from_xml(const QString &xml_path) {
	QDomDocument dom_document;
	QFile		 file(xml_path);
	QDomElement  dom_element;
	bool	     f_done = true;

	if (file.open(QIODevice::ReadOnly)) {
		if (dom_document.setContent(&file)) {
			dom_element = dom_document.documentElement();
			load_bound_from_xml(dom_element);
			load_nodes_from_xml(dom_element);
			load_ways_from_xml(dom_element);
			load_relations_from_xml(dom_element);
		} else {
			f_done = false;
		}
	} else {
		f_done = false;
	}

	return f_done;
}
