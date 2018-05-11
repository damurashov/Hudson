#include "tag_table.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/
const char* Tag_Table::KEY_HEADER = "Tag";
const char* Tag_Table::VALUE_HEADER = "Value";
const int Tag_Table::KEY_COL = 0;
const int Tag_Table::VALUE_COL = 1;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Tag_Table::Tag_Table() {
	QStringList headers;
	insertColumn(0);
	insertColumn(1);
	headers << KEY_HEADER << VALUE_HEADER;
	setHorizontalHeaderLabels(headers);
	mp_info = nullptr;
	clear();
//	QObject::connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(slot_cell_changed(int,int)));
}
/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Tag_Table::slot_push_row() {
	int row = rowCount();
	insertRow(row);
	setItem(row, KEY_COL, new QTableWidgetItem(""));
	setItem(row, VALUE_COL, new QTableWidgetItem(""));
}

void Tag_Table::slot_pop_row() {
	removeRow(rowCount() - 1);
}

void Tag_Table::slot_delete_selected() {
	QList<QTableWidgetSelectionRange> selected(selectedRanges());
	int row_min = rowCount() + 1;
	int row_max = -1;

	if (selected.empty()) {
		return;
	}
	for (auto it = selected.begin(); it != selected.end(); ++it) {
		if (row_min < it->topRow()) {
			row_min = it->topRow();
		}
		if (row_max > it->bottomRow()) {
			row_max = it->bottomRow();
		}
	}

	for (int i = 0; i < row_max - row_min + 1; ++i) {
		removeColumn(row_min);
	}
}

void Tag_Table::slot_delete_all() {
	setRowCount(0);
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Tag_Table::set_info(Osm_Info& info) {
	int row = 0;

	if (&info == mp_info) {
		return;
	}
	unset_info();
	mp_info = &info;

	setRowCount(mp_info->get_tag_map().size());
	for (auto it = mp_info->get_tag_map().cbegin(); it != mp_info->get_tag_map().cend(); ++it) {
		setItem(row, KEY_COL, new QTableWidgetItem(it.key()));
		setItem(row, VALUE_COL, new QTableWidgetItem(it.value()));
		row++;
	}
}

void Tag_Table::unset_info() {
	if (mp_info != nullptr) {
		mp_info->clear_tags();
	}
	for (int row = 0; row < rowCount(); ++row) {
		QString key = (item(row, KEY_COL) ? item(row, KEY_COL)->data(Qt::DisplayRole).toString() : "");
		QString value = (item(row, VALUE_COL) ? item(row, VALUE_COL)->data(Qt::DisplayRole).toString() : "");
		if (key != "") {
			mp_info->set_tag(key, value);
		}
	}
	mp_info = nullptr;
	slot_delete_all();
}
