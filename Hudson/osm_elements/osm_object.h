#ifndef OSM_OBJECT_H
#define OSM_OBJECT_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif // include guard QT_CORE_H

class Osm_Object {
private:
	typedef QHash<long long, Osm_Object*> Osm_Object_Hash;

	const long long						OSM_ID;
	const long long						INNER_ID;
	static Osm_Object_Hash				s_all_nodes_ptrs;
	static Osm_Object_Hash				s_all_ways_ptrs;
	static Osm_Object_Hash				s_all_relations_ptrs;
	static long long					s_osm_id_bound;
	static long long					s_inner_id_bound;
	QMap<QString, QString>				m_attrmap;
	QMap<QString, QString>				m_tagmap;
	Osm_Object_Hash						m_parents;
	Osm_Object_Hash						m_children;

	static Osm_Object_Hash&				get_working_hash(const Osm_Object::Type);
	static void							reg_osm_object	(Osm_Object*);
	static void							unreg_osm_object(Osm_Object*);
	void								add_child		(Osm_Object*);
	void								add_parent		(Osm_Object*);
	void								remove_child	(Osm_Object*);
	void								remove_parent	(Osm_Object*);
	virtual void						handle_child_del(Osm_Object*) = 0;
protected:
	enum class Type {NODE, WAY, RELATION};

	static Osm_Object*					get_obj_by_id	(const Type obj_type, const long long &obj_id);
	const QHash<long long, Osm_Object*>	get_children	() const;
	const QHash<long long, Osm_Object*>	get_parents		() const;
	virtual const Type					get_type		() const = 0;
	void								reg_child		(Osm_Object* child);
	void								unreg_child		(Osm_Object* child);
	long long							get_inner_id	() const;
public:
	QString								get_attr_value	(const QString& key) const;
	QString								get_tag_value	(const QString& key) const;
	const QMap<QString, QString>&		get_tag_map		() const;
	const QMap<QString, QString>&		get_attr_map	() const;
	long long							get_id			() const;
	void								set_tag			(const QString& key, const QString& value);
	void								set_attr		(const QString& key, const QString& value);
	void								remove_tag		(const QString& key);
	virtual void						clear_tags		();
	virtual bool						is_valid		() const = 0;
	                                    Osm_Object		(const QString& id);
										Osm_Object		();
	virtual								~Osm_Object		();
};	// class Osm_Object

#endif // include guard OSM_OBJECT_H
