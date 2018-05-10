#ifndef HUDSON_APP_H
#define HUDSON_APP_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_widget.h"

class Hudson_App : public QMainWindow {
	Q_OBJECT
private:
	Osm_Widget* mp_osm_widget;
public slots:
	void slot_load_from_xml();
	void slot_save_to_xml();
public:
	explicit Hudson_App(QWidget *parent = nullptr);
};

#endif // HUDSON_APP_H
