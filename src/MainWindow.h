#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PaintArea.h"

#include <QImage>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

   void updateColors();
   void updateColors(QPushButton *Color, QColor &currentColor);
private slots:
   void closeEvent(QCloseEvent *event) override;
   void OpenFile();
   void SaveFile(bool chooseLocation = false);
   void mousePressEvent(QMouseEvent *event) override;
   void mouseMoveEvent(QMouseEvent *event) override;
   void mouseReleaseEvent(QMouseEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override { paintArea->keyPressEvent(event); }
   void keyReleaseEvent(QKeyEvent *event) override { paintArea->keyReleaseEvent(event); }
   void wheelEvent(QWheelEvent *event) override { paintArea->wheelEvent(event); }

private:
   Ui::MainWindow *ui;
   PaintArea *paintArea;
   QImage image;
   bool dragging = false;
   QPoint lastMousePos;

   QColor activeColor = qRgb(0, 0, 0);   //Standart active black color
   QColor secondColor = qRgb(200, 0, 0); //Standart second red color
};

#endif // MAINWINDOW_H
