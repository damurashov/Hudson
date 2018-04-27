#ifndef VIEW_HANDLER_H
#define VIEW_HANDLER_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"

namespace ns_osm {

class View_Handler : public QWidget, public Osm_Subscriber {
	Q_OBJECT
public:
	enum Tool {
		TOOL_HAND,
		TOOL_NODE,
		TOOL_WAY,
		TOOL_CURSOR
	};
private slots:
	void			slot_blank_area_clicked	(QPointF, Qt::MouseButton);
	void			slot_node_clicked		(Osm_Node*, Qt::MouseButton);
	void			slot_edge_clicked		(QPointF,
	                                         Osm_Way*,
	                                         Osm_Node*,
	                                         Osm_Node*,
	                                         Qt::MouseButton);
private:
	Osm_Map&		m_map;
	Tool			m_current_tool;
	QGraphicsScene*	mp_scene;
	QGraphicsView*	mp_view;
	QSplitter*		mp_splitter;
	/* Info table will be here soon */
protected:
	void			emplace					(Osm_Node*);
	void			emplace					(Osm_Way*);
public:
	void			set_info_table_enabled	(bool f);
	void			set_editable			(bool f);
	void			set_tool				(Tool);
	void			handle_event_delete		(Osm_Node&) override;
	void			handle_event_delete		(Osm_Way&) override;
	void			handle_event_update		(Osm_Node&) override;
	void			handle_event_update		(Osm_Way&) override;
	                View_Handler			(Osm_Map&);
					View_Handler			(const View_Handler&);
					View_Handler			() = delete;
	View_Handler&	operator=				(const View_Handler) = delete;
}; /* class */

} /* namespace */

#endif // VIEW_HANDLER_H
