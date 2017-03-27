#ifndef GestureTraining_H
#define GestureTraining_H
#include "gesturedrawer.h"
#include "gesturetemplate.h"
#include <QLineEdit>

class GestureTraining : public GestureDrawer
{
    Q_OBJECT
public:
    explicit GestureTraining(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);


signals:
    void newTemplate(GestureTemplate);

public slots:
    void createTemplate();

protected:
    QLineEdit* edit;
};

#endif // GestureTraining_H
