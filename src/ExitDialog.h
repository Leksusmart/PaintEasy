#include <QCloseEvent>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

class ExitDialog : public QDialog
{
   Q_OBJECT

public:
   ExitDialog(QWidget *parent = nullptr)
      : QDialog(parent)
   {
      setFixedSize(300, 150);
      setWindowIcon(QIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning)));
      setWindowTitle("Exit");
      QVBoxLayout *Vlayout = new QVBoxLayout(this);

      QLabel *label = new QLabel("Хотите сохранить изменения?", this);
      label->setAlignment(Qt::AlignCenter);
      Vlayout->addWidget(label);

      QPushButton *yesButton = new QPushButton("Yes", this);
      QPushButton *noButton = new QPushButton("No", this);
      yesButton->setStyleSheet("QPushButton{border:2px solid black; border-radius:5px; "
                               "background-color:rgb(100,200,100);}QPushButton:hover{background-color:rgb(100,170,100);}QPushButton:pressed{background-color:rgb(100,140,100);}");
      noButton->setStyleSheet("QPushButton{border:2px solid black; border-radius:5px; "
                              "background-color:rgb(200,100,100);}QPushButton:hover{background-color:rgb(170,100,100);}QPushButton:pressed{background-color:rgb(140,100,100);}");
      yesButton->setFixedSize(130, 35);
      noButton->setFixedSize(130, 35);

      QGroupBox *group = new QGroupBox(this);
      group->setMaximumHeight(47);
      group->setStyleSheet("border:none;");
      Vlayout->addWidget(group);

      QHBoxLayout *Hlayout = new QHBoxLayout(group);
      Hlayout->addWidget(yesButton);
      Hlayout->addWidget(noButton);

      connect(yesButton, &QPushButton::clicked, [this]() {
         status = 1;
         close();
      });
      connect(noButton, &QPushButton::clicked, [this]() {
         status = 2;
         close();
      });
   }
   unsigned short int status = 0;
};
