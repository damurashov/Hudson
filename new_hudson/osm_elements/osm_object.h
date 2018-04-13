#ifndef OSM_OBJECT_H
#define OSM_OBJECT_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif // include guard QT_CORE_H

namespace ns_osm {

class Osm_Object {
protected:
	enum class Type;
private:
	const long long							OSM_ID;
	const long long							INNER_ID;
	const Type								TYPE;
	static long long						s_osm_id_bound;
	static long long						s_inner_id_bound;
	QMap<QString, QString>					m_attrmap;
	QMap<QString, QString>					m_tagmap;
	QHash<long long, Osm_Object*>			m_parents;
	QHash<long long, Osm_Object*>			m_children;
	unsigned								mn_parents;
	unsigned								mn_children;
	bool									f_is_valid;

	void									add_child		(Osm_Object*);
	void									add_parent		(Osm_Object*);
	void									remove_child	(Osm_Object*);
	void									remove_parent	(Osm_Object*);
	virtual void							handle_child_del(Osm_Object*) = 0;
											Osm_Object();
protected:
	enum class Type {NODE, WAY, RELATION};

	const QHash<long long, Osm_Object*>&	get_children	() const;
	const QHash<long long, Osm_Object*>&	get_parents		() const;
	const Type								get_type		() const;
	void									set_valid		(bool f_valid);
	void									reg_child		(Osm_Object* child);
	void									unreg_child		(Osm_Object* child);
	long long								get_inner_id	() const;
	unsigned								count_parents	() const;
	unsigned								count_children	() const;
public:
	QString									get_attr_value	(const QString& key) const;
	QString									get_tag_value	(const QString& key) const;
	const QMap<QString, QString>&			get_tag_map		() const;
	const QMap<QString, QString>&			get_attr_map	() const;
	long long								get_id			() const;
	void									set_tag			(const QString& key, const QString& value);
	void									set_attr		(const QString& key, const QString& value);
	void									remove_tag		(const QString& key);
	void									clear_tags		();
	bool									is_valid		() const;
											Osm_Object		(const QString& id, const Type);
											Osm_Object		(const Type);
	virtual									~Osm_Object		();
};	// class Osm_Object
}

#endif // include guard OSM_OBJECT_H
