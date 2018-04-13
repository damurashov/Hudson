#include "osm_relation.h"
using namespace ns_osm;
// ========================= Constructors, destructors ==================

Osm_Relation::Osm_Relation(const QString& id) : Osm_Object(id, Osm_Object::Type::RELATION) {
	mn_nodes = 0;
	mn_ways = 0;
	mn_relations = 0;
	//m_size = 0;
	//reg_osm_object(this);
}

Osm_Relation::Osm_Relation() : Osm_Object(Osm_Object::Type::RELATION) {
	mn_nodes = 0;
	mn_ways = 0;
	mn_relations = 0;
	//m_size = 0;
	//reg_osm_object(this);
}

Osm_Relation::~Osm_Relation() {
	//unreg_osm_object(this);
}

// ========================== Private methods ===========================

void Osm_Relation::handle_child_del(Osm_Object* ptr_child) {
	Osm_Object::Type object_type = (static_cast<Osm_Relation*>(ptr_child)->get_type());

	switch (object_type) {
	case Osm_Object::Type::NODE:
		m_nodes_list.removeOne(static_cast<Osm_Node*>(ptr_child));
		mn_nodes--;
		//m_size--;
		m_roles_hash.remove(static_cast<Osm_Relation*>(ptr_child)->get_inner_id());
		break;
	case Osm_Object::Type::WAY:
		m_ways_list.removeOne(static_cast<Osm_Way*>(ptr_child));
		mn_ways--;
		//m_size--;
		m_roles_hash.remove(static_cast<Osm_Relation*>(ptr_child)->get_inner_id());
		break;
	case Osm_Object::Type::RELATION:
		m_relations_list.removeOne(static_cast<Osm_Relation*>(ptr_child));
		mn_relations--;
		//m_size--;
		m_roles_hash.remove(static_cast<Osm_Relation*>(ptr_child)->get_inner_id());
		break;
	}
}
// =========================== Public methods ===========================

void Osm_Relation::add(Osm_Object* ptr_object, const QString& role) {
	Osm_Object::Type object_type;

	if (ptr_object == nullptr) {
		set_valid(false);
		return;
	} else if (has_object(ptr_object)) {
		return;
	}

	reg_child(ptr_object);
	object_type = static_cast<Osm_Relation*>(ptr_object)->get_type();

	switch (object_type) {
	case Osm_Object::Type::NODE:
		m_nodes_list.push_back(static_cast<Osm_Node*>(ptr_object));
		mn_nodes++;
		//m_size++;
		break;
	case Osm_Object::Type::WAY:
		m_ways_list.push_back(static_cast<Osm_Way*>(ptr_object));
		mn_ways++;
		//m_size++;
		break;
	case Osm_Object::Type::RELATION:
		m_relations_list.push_back(static_cast<Osm_Relation*>(ptr_object));
		mn_relations++;
		//m_size++;
		break;
	}
	set_role(ptr_object, role);
}

void Osm_Relation::remove(Osm_Object* ptr_object) {
	if (has_object(ptr_object)) {
		unreg_child(ptr_object);
	}
}

bool Osm_Relation::has_object(Osm_Object* ptr_object) const {
	if (ptr_object != nullptr) {
		return get_children().contains(static_cast<Osm_Relation*>(ptr_object)->get_inner_id());
	}
	return false;
}

void Osm_Relation::set_role(Osm_Object* ptr_object, const QString& role) {
	if (has_object(ptr_object)) {
		m_roles_hash[static_cast<Osm_Relation*>(ptr_object)->get_inner_id()] = role;
	}
}

unsigned Osm_Relation::get_size() const {
	return count_children();
}

const QString Osm_Relation::get_role(Osm_Object* ptr_object) const {
	if (has_object(ptr_object)) {
		return m_roles_hash[static_cast<Osm_Relation*>(ptr_object)->get_inner_id()];
	}
	return QString("");
}

const QList<Osm_Node*>& Osm_Relation::get_nodes() const {
	return m_nodes_list;
}

const QList<Osm_Way*>& Osm_Relation::get_ways() const {
	return m_ways_list;
}

const QList<Osm_Relation*>& Osm_Relation::get_relations() const {
	return m_relations_list;
}

unsigned short Osm_Relation::count_nodes() const {
	return mn_nodes;
}

unsigned short Osm_Relation::count_ways() const {
	return mn_ways;
}

unsigned short Osm_Relation::count_relations() const {
	return mn_relations;
}
