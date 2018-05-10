#ifndef TAG_TABLE_H
#define TAG_TABLE_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"

namespace ns_osm {
class Tag_Table : public QTableWidget {
	Q_OBJECT
private:
	static const char*	KEY_HEADER;
	static const char*	VALUE_HEADER;
//	static const int	I_KEY;
//	static const int	I_VALUE;
	Osm_Info*			mp_info;
protected:
//	void				handle_event_delete	(Osm_Object& generic_emitter);
	void				apply				();
	void				clear				();
	void				fill				();
private slots:
	void				slot_cell_changed	(int row, int col);
public slots:
	void				slot_push_row		();
	void				slot_pop_row		();
	void				slot_delete_selected();
	void				slot_delete_all		();
//	void				slot_fill			();
	void				slot_update			();
public:
	void				set_info			(Osm_Info&);
	void				unset_info			();
	Tag_Table();
};
}

#endif // TAG_TABLE_H
