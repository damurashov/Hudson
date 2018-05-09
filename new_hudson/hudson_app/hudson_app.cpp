#include "hudson_app.h"

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Hudson_App::Hudson_App(QWidget* p_parent) : QMainWindow(p_parent) {
	QAction*	p_act_load_xml	= new QAction("Load from xml");
	QMenu*		p_menu_file		= new QMenu("File");
	QMenuBar*	p_menubar		= menuBar();

	mp_osm_widget = new Osm_Widget;
	setCentralWidget(mp_osm_widget);

	p_act_load_xml->setText("Load...");
	p_act_load_xml->setToolTip("Loads the content from OSM XML FILE");
	p_act_load_xml->setWhatsThis("Loads the content from OSM XML FILE");
	connect(p_act_load_xml, SIGNAL(triggered()), SLOT(slot_load_from_xml()));
	p_menu_file->addAction(p_act_load_xml);
	p_menubar->addMenu(p_menu_file);
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Hudson_App::slot_load_from_xml() {
	Osm_Message msg;
	int			errcode;
	QString		filename = QFileDialog::getOpenFileName(this,
	                                                    tr("Open File"),
	                                                    "",
	                                                    tr("OSM-file (*.osm)"));
	if (filename == "") {
		return;
	}
	if ((errcode = mp_osm_widget->load_from_xml(filename)) != OSM_OK) {
		QMessageBox* p_messagebox = new QMessageBox(QMessageBox::Warning,
		                                            "Cannot load from file",
		                                            msg(errcode),
		                                            QMessageBox::Ok);
	}
}
