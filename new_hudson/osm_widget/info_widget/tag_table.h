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
	static const int	KEY_COL;
	static const int	VALUE_COL;
	Osm_Info*			mp_info;
protected:
	//QString				get_at				(int row, int col);
	//void				remove_at			(int row);
	//void				set_at				(int row, int col, const QString& = "");
public slots:
	void				slot_push_row		();
	void				slot_pop_row		();
	void				slot_delete_selected();
	void				slot_delete_all		();
//	void				slot_update			();
public:
	void				set_info			(Osm_Info&);
	void				unset_info			();
	Tag_Table();
};
}

#endif // TAG_TABLE_H
