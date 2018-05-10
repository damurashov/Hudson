#include "tag_table.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/
const char* Tag_Table::KEY_HEADER = "Tag";
const char* Tag_Table::VALUE_HEADER = "Value";

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Tag_Table::Tag_Table() {
	QStringList headers;

	headers << KEY_HEADER << VALUE_HEADER;
	setHorizontalHeaderLabels(headers);
	mp_info = nullptr;
	clear();
	QObject::connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(slot_cell_changed(int,int)));
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

void Tag_Table::slot_cell_changed(int row, int col) {
	apply();
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Tag_Table::apply() {
	if (mp_info == nullptr) {
		return;
	}
	mp_info->clear_tags();
	for (int row = 0; row < rowCount(); ++row) {
		QString key(item(row, 0)->data(Qt::DisplayRole).toString());
		QString value(item(row, 1)->data(Qt::DisplayRole).toString());
		if (key != "" && value != "") {
			mp_info->set_tag(key, value);
		}
	}
}

void Tag_Table::clear() {
	clearContents();
}

void Tag_Table::fill() {
	if (mp_info == nullptr) {
		return;
	}
	for (auto it = mp_info->get_tag_map().cbegin(); it != mp_info->get_tag_map().cend(); ++it) {
		int row = rowCount();
		slot_push_row();
		item(row, 0)->setData(Qt::DisplayRole, it.key());
		item(row,1)->setData(Qt::DisplayRole, it.value());
	}
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Tag_Table::slot_push_row() {
	insertRow(rowCount());
}

void Tag_Table::slot_pop_row() {
	removeRow(rowCount() - 1);
	apply();
}

void Tag_Table::slot_delete_selected() {
	QList<QTableWidgetSelectionRange>	selected(selectedRanges());
	int									row_min = rowCount() + 1;
	int									row_max = -1;

	if (selected.empty()) {
		return;
	}

	for (auto it = selected.begin(); it != selected.end(); ++it) {
		if (it->bottomRow() < row_min) {
			row_min = it->bottomRow();
		}
		if (it->topRow() > row_max) {
			row_max = it->topRow();
		}
	}

	for (int i = 0; i < row_max - row_min + 1; ++i) {
		removeRow(row_min);
	}
	apply();
}

void Tag_Table::slot_delete_all() {
	clear();
	apply();
}

void Tag_Table::slot_update() {
	clear();
	fill();
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Tag_Table::set_info(Osm_Info& info) {
	if (&info == mp_info) {
		return;
	}
	mp_info = &info;
	slot_update();
}

void Tag_Table::unset_info() {
	mp_info = nullptr;
	clear();
}
