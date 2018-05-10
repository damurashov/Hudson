#ifndef INFO_WIDGET_H
#define INFO_WIDGET_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"
#include "tag_table.h"

namespace ns_osm {
class Info_Widget : public QWidget, public Osm_Subscriber {
	Q_OBJECT
private:
	Tag_Table*	mp_tag_table;
protected:
	void		handle_event_delete	(Osm_Node&) override;
	void		handle_event_delete	(Osm_Way&) override;
public slots:
	void		slot_object_selected(Osm_Node&);
	void		slot_object_selected(Osm_Way&);
public:
	            Info_Widget			(QWidget* p_parent = nullptr);
};
}

#endif // INFO_WIDGET_H
