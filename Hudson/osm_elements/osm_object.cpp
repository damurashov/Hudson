#include "osm_object.h"

// ==================== Static members ====================
QHash<long long, Osm_Object*>	Osm_Object::s_all_nodes_ptrs;
QHash<long long, Osm_Object*>	Osm_Object::s_all_relations_ptrs;
QHash<long long, Osm_Object*>	Osm_Object::s_all_ways_ptrs;
long long						Osm_Object::s_osm_id_bound(-1);
long long						Osm_Object::s_inner_id_bound(0xFFFFFFFFFFFFFFFF);

// =============== Constructors, destructors =================
Osm_Object::Osm_Object() :
    OSM_ID(s_osm_id_bound--),
    INNER_ID(s_inner_id_bound++)
{
	m_attrmap[QString("id")] = QString::number(ID);
	reg_osm_object(this);
}


Osm_Object::Osm_Object(const QString &id):
    OSM_ID(id.toLongLong()),
    INNER_ID(s_inner_id_bound++)
{
	m_attrmap[QString("id")] = QString::number(ID);
	if (s_id_bound <= ID)
		s_id_bound = (ID - 1);
	reg_osm_object(this);
}

Osm_Object::~Osm_Object() {
	for (QHash<long long, Osm_Object*>::iterator it = m_parents.begin(); it != m_parents.end(); ++it) {
		it.value()->remove_child(this);
	}
	for (QHash<long long, Osm_Object*>::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		it.value()->remove_parent(this);
	}
	unreg_osm_object(this);
}

// ==================== Private methods ======================
Osm_Object::Osm_Object_Hash& Osm_Object::get_working_hash(const Osm_Object::Type object_type) {
	switch (object_type) {
	case Type::NODE:
		return s_all_nodes_ptrs;
	case Type::WAY:
		return s_all_ways_ptrs;
	case Type::RELATION:
		return s_all_ways_ptrs;
	}
}

void Osm_Object::reg_osm_object(Osm_Object* object) {
	Osm_Object_Hash& hash = get_working_hash(object->get_type());
	hash[object->get_id()] = object;
}

void Osm_Object::unreg_osm_object(Osm_Object* object) {
	Osm_Object_Hash& hash = get_working_hash(object->get_type());
	hash.remove(object->get_id());
}

void Osm_Object::add_child(Osm_Object* ptr_child) {
	m_children[ptr_child->get_inner_id()] = ptr_child;
}

void Osm_Object::add_parent(Osm_Object* ptr_parent) {
	m_parents[ptr_parent->get_inner_id()] = ptr_parent;
}

void Osm_Object::remove_child(Osm_Object* ptr_child) {
	m_children.remove(ptr_child->get_inner_id());
	handle_child_del(ptr_child);
}

void Osm_Object::remove_parent(Osm_Object* ptr_parent) {
	m_parents.remove(ptr_parent->get_inner_id());
}


// ==================== Protected methods ====================
Osm_Object* Osm_Object::get_obj_by_id(const Type type, const long long &obj_id) {
	Osm_Object_Hash& hash = get_working_hash(type);
	return hash[obj_id];
}

const QHash<long long, Osm_Object*>& Osm_Object::get_children() const {
	return m_children;
}

const QHash<long long, Osm_Object*>& Osm_Object::get_parents() const {
	return m_parents;
}

void Osm_Object::reg_child(Osm_Object* ptr_child) {
	if (ptr_child != nullptr) {
		add_child(ptr_child);
		ptr_child->add_parent(this);
	}
}

void Osm_Object::unreg_child(Osm_Object* ptr_child) {
	if (ptr_child != nullptr) {
		remove_child(ptr_child);
		ptr_child->remove_parent(this);
	}
}


// ==================== Public methods ====================
QString Osm_Object::get_attr_value(const QString& key) const {
	return m_attrmap[key];
}

QString Osm_Object::get_tag_value(const QString &key) const {
	return m_tagmap[key];
}

const QMap<QString, QString>& Osm_Object::get_tag_map() const {
	return m_tagmap;
}

const QMap<QString, QString>& Osm_Object::get_attr_map() const {
	return m_attrmap;
}

long long Osm_Object::get_id() const {
	return ID;
}

void Osm_Object::set_tag(const QString &key, const QString &value) {
	m_tagmap[key] = value;
}

void Osm_Object::set_attr(const QString &key, const QString &value) {
	if (key == "id")
		return;
	m_attrmap[key] = value;
}

void Osm_Object::remove_tag(const QString &key) {
	m_tagmap.remove(key);
}


void Osm_Object::clear_tags() {
	m_tagmap.clear();
}
