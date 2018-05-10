#include "hudson_app.h"

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Hudson_App::Hudson_App(QWidget* p_parent) : QMainWindow(p_parent) {
	QAction*	p_act_load_xml	= new QAction("Load from xml");
	QAction*	p_act_save_xml	= new QAction("Save to xml");
	QMenu*		p_menu_file		= new QMenu("File");
	QMenuBar*	p_menubar		= menuBar();
//	QToolBar*	p_toolbar_draw	= new QToolBar("Tools");

	mp_osm_widget = new Osm_Widget;
	setCentralWidget(mp_osm_widget);

	p_act_load_xml->setText("Load...");
	p_act_load_xml->setToolTip("Loads the content from OSM XML file");
	p_act_load_xml->setWhatsThis("Loads the content from OSM XML file");
	connect(p_act_load_xml, SIGNAL(triggered()), SLOT(slot_load_from_xml()));
	p_menu_file->addAction(p_act_load_xml);

	p_act_save_xml->setText("Save...");
	p_act_save_xml->setToolTip("Saves the current content to a OSM XML file");
	p_act_save_xml->setWhatsThis("Saves the current content to a OSM XML file");
	connect(p_act_save_xml, SIGNAL(triggered()), SLOT(slot_save_to_xml()));
	p_menu_file->addAction(p_act_save_xml);

	p_menubar->addMenu(p_menu_file);
//	addToolBar(p_toolbar);
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
	                                                    tr("*.osm"));
	if (filename == "") {
		return;
	}
	if ((errcode = mp_osm_widget->load_from_xml(filename)) != OSM_OK) {
		QMessageBox* p_messagebox = new QMessageBox(QMessageBox::Warning,
		                                            "Cannot load from file",
		                                            msg(errcode),
		                                            QMessageBox::Ok);
		p_messagebox->exec();
	}
}

void Hudson_App::slot_save_to_xml() {
	Osm_Message msg;
	int errcode;
	QString filename = QFileDialog::getSaveFileName(this,
	                                                tr("Save file"),
	                                                "",
	                                                tr("OSM-file (*.osm)"));
	if ((errcode = mp_osm_widget->save_to_xml(filename)) != OSM_OK) {
		QMessageBox* p_messagebox = new QMessageBox(QMessageBox::Warning,
		                                            "Cannot save to file",
		                                            msg(errcode),
		                                            QMessageBox::Ok);
		p_messagebox->exec();
	}
}
