#include "osm_object.h"
using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/

//QHash<long long, Osm_Object*>	Osm_Object::s_all_nodes_ptrs;
//QHash<long long, Osm_Object*>	Osm_Object::s_all_relations_ptrs;
//QHash<long long, Osm_Object*>	Osm_Object::s_all_ways_ptrs;
long long						Osm_Object::s_osm_id_bound(-1);
long long						Osm_Object::s_inner_id_bound(0xFFFFFFFFFFFFFFFF);

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Object::Osm_Object() :
    OSM_ID(s_osm_id_bound--),
	INNER_ID(s_inner_id_bound++),
	TYPE(Osm_Object::Type::NODE)
{
	f_is_valid = true;
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	mn_children = 0;
	mn_parents = 0;
	//reg_osm_object(this);
}

Osm_Object::Osm_Object(const Osm_Object::Type type) :
	OSM_ID(s_osm_id_bound--),
	INNER_ID(s_inner_id_bound++),
	TYPE(type)
{
	f_is_valid = true;
	mn_children = 0;
	mn_parents = 0;
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	//reg_osm_object(this);
}


Osm_Object::Osm_Object(const QString &id, const Osm_Object::Type type):
    OSM_ID(id.toLongLong()),
	INNER_ID(s_inner_id_bound++),
	TYPE(type)
{
	f_is_valid = true;
	mn_children = 0;
	mn_parents = 0;
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	if (s_osm_id_bound >= OSM_ID)
		s_osm_id_bound = (OSM_ID - 1);
	//reg_osm_object(this);
}

Osm_Object::~Osm_Object() {
	for (QHash<long long, Osm_Object*>::iterator it = m_parents.begin(); it != m_parents.end(); ++it) {
		it.value()->remove_child(this);
	}
	for (QHash<long long, Osm_Object*>::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		it.value()->remove_parent(this);
	}
	//unreg_osm_object(this);
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/


//void Osm_Object::reg_osm_object(Osm_Object* object) {
//	QHash<long long, Osm_Object*>& hash = get_working_hash(object->get_type());
//	hash[object->get_id()] = object;
//}

//void Osm_Object::unreg_osm_object(Osm_Object* object) {
//	QHash<long long, Osm_Object*>& hash = get_working_hash(object->get_type());
//	hash.remove(object->get_id());
//}

void Osm_Object::add_child(Osm_Object* ptr_child) {
	m_children[ptr_child->get_inner_id()] = ptr_child;
	mn_children++;
}

void Osm_Object::add_parent(Osm_Object* ptr_parent) {
	m_parents[ptr_parent->get_inner_id()] = ptr_parent;
	mn_parents++;
}

void Osm_Object::remove_child(Osm_Object* ptr_child) {
	m_children.remove(ptr_child->get_inner_id());
	mn_children--;
	handle_child_del(ptr_child);
}

void Osm_Object::remove_parent(Osm_Object* ptr_parent) {
	m_parents.remove(ptr_parent->get_inner_id());
	mn_parents--;
}

/*================================================================*/
/*                                                                */
/*                      Protected methods                         */
/*                                                                */
/*================================================================*/



//QHash<long long, Osm_Object*>& Osm_Object::get_working_hash(const Osm_Object::Type object_type) {
//	switch (object_type) {
//	case Type::NODE:
//		return s_all_nodes_ptrs;
//	case Type::WAY:
//		return s_all_ways_ptrs;
//	case Type::RELATION:
//		return s_all_ways_ptrs;
//	}
//}

//Osm_Object* Osm_Object::get_obj_by_id(const Type type, const long long &obj_id) {
//	QHash<long long, Osm_Object*>& hash = get_working_hash(type);
//	return hash[obj_id];
//}

const QHash<long long, Osm_Object*>& Osm_Object::get_children() const {
	return m_children;
}

const QHash<long long, Osm_Object*>& Osm_Object::get_parents() const {
	return m_parents;
}

const Osm_Object::Type Osm_Object::get_type() const {
	return TYPE;
}

void Osm_Object::set_valid(bool f_valid) {
	f_is_valid = f_valid;
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

long long Osm_Object::get_inner_id() const {
	return INNER_ID;
}

unsigned Osm_Object::count_parents() const {
	return mn_parents;
}

unsigned Osm_Object::count_children() const {
	return mn_children;
}

/*================================================================*/
/*                                                                */
/*                        Public methods                          */
/*                                                                */
/*================================================================*/


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
	return OSM_ID;
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

bool Osm_Object::is_valid() const {
	return f_is_valid;
}
