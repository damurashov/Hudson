#include "xml_handler.h"
using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Xml_Handler::Xml_Handler(Osm_Map& map) : m_map(map) {}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Xml_Handler::load_bound_from_xml(const QDomNode& node) {
	QDomNode	dom_node = node.firstChild();
	QDomElement dom_element;
	QRectF		bound;
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
	bound.setLeft(minlon);
	bound.setBottom(minlat);
	bound.setRight(maxlon);
	bound.setTop(maxlat);
	m_map.set_bound(bound);
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
			m_map.add(p_node);
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

									/* Push referenced nodes into the way */
								} else if (dom_element.tagName() == Osm_Xml::ND) {
									p_way->push_node(m_map.get_node(dom_element.attribute(Osm_Xml::REF, "").toLongLong()));
								}
							}
						}
						dom_node_inner = dom_node_inner.nextSibling();
					}
				}
			}
		}
		if (p_way != nullptr) {
			//m_ways_hash[p_way->get_id()] = p_way;
			m_map.add(p_way);
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
										p_rel->add(m_map.get_node(ref), dom_element.attribute(Osm_Xml::ROLE));
									} else if (attr_type == Osm_Xml::WAY) {
										p_rel->add(m_map.get_way(ref), dom_element.attribute(Osm_Xml::ROLE));
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
			m_map.add(p_rel);
			//m_relations_hash[p_rel->get_id()] = p_rel;
		}
		dom_node_outer = dom_node_outer.nextSibling();
	}

	/* Handle pending relations */
	for (it_pr = pending_relations.begin(); it_pr != pending_relations.end(); ++it_pr) {
		it_pr->p_master->add(m_map.get_relation(it_pr->slave_id), it_pr->slave_role);
	}
}

void Xml_Handler::compose_attrs_and_tags(QDomDocument& doc,
                                         QDomElement& node,
                                         const Osm_Info& info) {
	node.setAttribute(Osm_Xml::ID, info.get_attr_value(Osm_Xml::ID));
	node.setAttribute(Osm_Xml::VISIBLE, info.get_attr_value(Osm_Xml::VISIBLE));
	node.setAttribute(Osm_Xml::VERSION, info.get_attr_value(Osm_Xml::VERSION));
	node.setAttribute(Osm_Xml::TIMESTAMP, info.get_attr_value(Osm_Xml::TIMESTAMP));
	node.setAttribute(Osm_Xml::USER, info.get_attr_value(Osm_Xml::USER));
	node.setAttribute(Osm_Xml::UID, info.get_attr_value(Osm_Xml::UID));

	for (auto it = info.get_tag_map().cbegin(); it != info.get_tag_map().cend(); ++it) {
		QDomElement tag = doc.createElement(Osm_Xml::TAG);
		tag.setAttribute(Osm_Xml::K, it.key());
		tag.setAttribute(Osm_Xml::V, it.value());
		node.appendChild(tag);
	}
}

void Xml_Handler::compose_bound(QDomDocument& doc, QDomElement& node_osm) {
	QRectF bound(m_map.get_bound());
	QDomElement bounds = doc.createElement(Osm_Xml::BOUNDS);

	bounds.setAttribute(Osm_Xml::MINLON, bound.left());
	bounds.setAttribute(Osm_Xml::MAXLON, bound.right());
	bounds.setAttribute(Osm_Xml::MINLAT, bound.bottom());
	bounds.setAttribute(Osm_Xml::MAXLAT, bound.top());
	node_osm.appendChild(bounds);
}

void Xml_Handler::compose_nodes(QDomDocument &doc, QDomElement &node_osm) {
	for (auto it = m_map.nbegin(); it != m_map.nend(); ++it) {
		QDomElement node = doc.createElement(Osm_Xml::NODE);
		node_osm.appendChild(node);
		node.setAttribute(Osm_Xml::LAT, (*it)->get_attr_value(Osm_Xml::LAT));
		node.setAttribute(Osm_Xml::LON, (*it)->get_attr_value(Osm_Xml::LON));
		compose_attrs_and_tags(doc, node, **it);
	}
}

void Xml_Handler::compose_ways(QDomDocument &doc, QDomElement &node_osm) {
	for (auto it = m_map.wbegin(); it != m_map.wend(); ++it) {
		QDomElement way = doc.createElement(Osm_Xml::WAY);
//		node_osm.appendChild(way);
		compose_attrs_and_tags(doc, way, **it);
		for (auto waynode = (*it)->get_nodes_list().cbegin(); waynode != (*it)->get_nodes_list().cend(); ++waynode) {
			QDomElement ref = doc.createElement(Osm_Xml::ND);
			ref.setAttribute(Osm_Xml::REF, (*waynode)->get_id());
			way.appendChild(ref);
		}
		node_osm.appendChild(way);
	}
}

void Xml_Handler::compose_relations(QDomDocument &doc, QDomElement &node_osm) {
	for (auto it = m_map.rbegin(); it != m_map.rend(); ++it) {
		QDomElement relation = doc.createElement(Osm_Xml::RELATION);
		compose_attrs_and_tags(doc, relation, **it);
		for (auto node = (*it)->get_nodes().cbegin(); node != (*it)->get_nodes().cend(); ++node) {
			QDomElement member = doc.createElement(Osm_Xml::MEMBER);
			member.setAttribute(Osm_Xml::TYPE, Osm_Xml::NODE);
			member.setAttribute(Osm_Xml::REF, QString(QString::number((*node)->get_id())));
			member.setAttribute(Osm_Xml::ROLE, (*it)->get_role((*node)));
			relation.appendChild(member);
		}
		for (auto way = (*it)->get_ways().cbegin(); way != (*it)->get_ways().cend(); ++way) {
			QDomElement member = doc.createElement(Osm_Xml::MEMBER);
			member.setAttribute(Osm_Xml::TYPE, Osm_Xml::WAY);
			member.setAttribute(Osm_Xml::REF, QString(QString::number((*way)->get_id())));
			member.setAttribute(Osm_Xml::ROLE, (*it)->get_role(*way));
			relation.appendChild(member);
		}
		for (auto rel = (*it)->get_relations().cbegin(); rel != (*it)->get_relations().cend(); ++rel) {
			QDomElement member = doc.createElement(Osm_Xml::MEMBER);
			member.setAttribute(Osm_Xml::TYPE, Osm_Xml::RELATION);
			member.setAttribute(Osm_Xml::REF, QString(QString::number((*rel)->get_id())));
			member.setAttribute(Osm_Xml::ROLE, (*it)->get_role(*rel));
			relation.appendChild(member);
		}
		node_osm.appendChild(relation);
	}
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

int Xml_Handler::load_from_xml(const QString &xml_path) {
	QDomDocument dom_document;
	QFile		 file(xml_path);
	QDomElement  dom_element;
	int			 code = OSM_OK;

	m_map.clear();
	if (file.open(QIODevice::ReadOnly)) {
		if (dom_document.setContent(&file)) {
			dom_element = dom_document.documentElement();
			load_bound_from_xml(dom_element);
			load_nodes_from_xml(dom_element);
			load_ways_from_xml(dom_element);
			load_relations_from_xml(dom_element);
		} else {
			//f_done = false;
			code = OSM_ERROR_WRONG_XML_FORMAT;
		}
	} else {
		//f_done = false;
		code = OSM_ERROR_XML_FILE_NOT_EXISTS;
	}
//	if (code == OSM_OK) {
//		m_map.fit_bounding_rect();
//	}

	//return f_done;
	return code;
}

int Xml_Handler::save_to_xml(const QString& xml_path) {
	QDomDocument	out_xml;
	QDomElement		node_osm = out_xml.createElement(Osm_Xml::OSM);
	QFile			file(xml_path);

	node_osm.setAttribute(Osm_Xml::VERSION, "0.6");
	node_osm.setAttribute(Osm_Xml::GENERATOR, "Hudson");
	out_xml.appendChild(node_osm);

//	QDomElement tag_bound = out_xml.createAttribute(Osm_Xml::BOUNDS);
//	QRectF bound(m_map.get_bound());
//	tag_bound.setAttribute(Osm_Xml::MINLON, QString(QString::number(bound.left())));
//	tag_bound.setAttribute(Osm_Xml::MAXLON, QString(QString::number(bound.right())));
//	tag_bound.setAttribute(Osm_Xml::MINLAT, QString(QString::number(bound.bottom())));
//	tag_bound.setAttribute(Osm_Xml::MAXLAT, QString(String::number(bound.top())));
//	node_osm.appendChild(tag_bound);

	compose_bound(out_xml, node_osm);
	compose_nodes(out_xml, node_osm);
	compose_ways(out_xml, node_osm);
	compose_relations(out_xml, node_osm);

	if (file.open(QIODevice::WriteOnly)) {
		QTextStream(&file) << out_xml.toString();
		file.close();
	} else {
		return OSM_ERROR_CANNOT_WRITE_FILE;
	}
	return OSM_OK;
}
