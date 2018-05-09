#ifndef OSM_VIEW_H
#define OSM_VIEW_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

namespace ns_osm {

class Osm_View : public QGraphicsView {
	Q_OBJECT
signals:
	void	signal_blank_area_clicked	(QPointF, Qt::MouseButton);
private:
	QPointF	m_last_pos;

//	void	scale_view					(double scale_factor);
protected:
	QPointF	get_last_pos				() const;
	void	set_last_pos				(const QPointF&);
	void	wheelEvent					(QWheelEvent *event) override;
//	void	mousePressEvent				(QMouseEvent *event) override;
//	void	mouseMoveEvent				(QMouseEvent *event) override;
	void	mouseReleaseEvent			(QMouseEvent *event) override;
public:
	        Osm_View					();
	virtual	~Osm_View					();
};
}

#endif // OSM_VIEW_H
