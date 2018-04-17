#ifndef OSM_OBJECT_H
#define OSM_OBJECT_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif // include guard QT_CORE_H

#include "osm_subscriber.h"

namespace ns_osm {

class Osm_Object {
protected:
	enum class Type;
private:
	const long long					OSM_ID;
	const long long					INNER_ID;
	const Type						TYPE;
	static long long				s_osm_id_bound;
	static long long				s_inner_id_bound;
	QMap<QString, QString>			m_attrmap;
	QMap<QString, QString>			m_tagmap;
	QList<Osm_Subscriber*>			m_subscribers;
	bool							f_is_valid;

	                                Osm_Object			();
protected:
	enum class Type {NODE, WAY, RELATION};

	const Type						get_type			() const;
	long long						get_inner_id		() const;
	void							set_valid			(bool f_valid);
	void							emit_delete			();
	void							emit_update			();
	                                Osm_Object			(const Type);
									Osm_Object			(const QString& id, const Type);
									Osm_Object			(const Osm_Object&) = delete;
	Osm_Object&						operator=			(const Osm_Object&) = delete;
public:
	void							add_subscriber		(Osm_Subscriber&);
	void							remove_subscriber	(Osm_Subscriber&);
	QString							get_attr_value		(const QString& key) const;
	QString							get_tag_value		(const QString& key) const;
	const QMap<QString, QString>&	get_tag_map			() const;
	const QMap<QString, QString>&	get_attr_map		() const;
	long long						get_id				() const;
	void							set_tag				(const QString& key, const QString& value);
	void							set_attr			(const QString& key, const QString& value);
	void							remove_tag			(const QString& key);
	void							clear_tags			();
	bool							is_valid			() const;
	virtual							~Osm_Object			();
};	// class Osm_Object

}

#endif // include guard OSM_OBJECT_H
