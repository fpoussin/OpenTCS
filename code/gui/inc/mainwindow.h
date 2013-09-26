#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

private slots:
    void connect(void);
    void disconnect(void);
    void update(void);
    void loadConfig(void);
    void saveConfig(void);

    void getConfig(void);
    void applyConfig(void);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
