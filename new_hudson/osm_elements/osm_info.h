#ifndef OSM_INFO_H
#define OSM_INFO_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif /* Include guard QT_CORE_H */

namespace ns_osm {

class Osm_Info{
	QMap<QString, QString>			m_attrmap;
	QMap<QString, QString>			m_tagmap;
	const long long					OSM_ID;
	static long long				s_osm_id_bound;
public:
	QString							get_attr_value	(const QString& key) const;
	QString							get_tag_value	(const QString& key) const;
	const QMap<QString, QString>&	get_tag_map		() const;
	const QMap<QString, QString>&	get_attr_map	() const;
	long long						get_id			() const;
	void							set_tag			(const QString& key, const QString& value);
	void							set_attr		(const QString& key, const QString& value);
	void							remove_tag		(const QString& key);
	void							clear_tags		();
	                                Osm_Info		();
									Osm_Info		(const QString& id);
									Osm_Info		(long long id);
									Osm_Info		(const Osm_Info&);
	Osm_Info&						operator=		(const Osm_Info&);
	virtual							~Osm_Info		() = default;
};

}

#endif // OSM_INFO_H
