//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� codewidget.h
//���ߣ�   �г�     Version��1.0    Date��2016.10.27
//������   ���������
//������
//�޸���ʷ��
//2016.12.04:sc:    ��ӽ���������ܣ���������ܣ���Ԫ�񲻿ɱ༭��
//                  ����ļ�����idl; �����Windows�²鿴�ļ�Դ�빦�ܣ�
//2016.12.05:sc:    ���ƽ���ļ��������ܣ����Ʊ�ͷ�����ܣ�
//                  ���ƽ��������ʾ���ܣ�
//**************************************************

#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QMainWindow>
#include <QStringList>
class QTableWidgetItem;
class StatisticalThread;
class Parameter;

namespace Ui {
class CodeWidget;
}

class CodeWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit CodeWidget(QWidget *parent = 0);
    ~CodeWidget();
    struct FileResults
    {
        QString filePath;   //�ļ�ȫ·��
        int totleLines;     //�ļ�������
        int codeLines;      //��������
        int noteLines;      //ע����
        int spaceLines;     //����
    };

private slots:
    void selectFile(bool);
    void selectDir(bool);
    void startStatiscal(bool);
    void lookFileContents(bool);
    void setParameters(bool);
    void quitThis(bool);
    void makeForFile(bool);
    void updateTable(QString fileName,int totleLines, int codeLines, int noteLines, int spaceLines);
    void tableItemClicked(QTableWidgetItem *);
    void mySort(int);

private:
    void initConnect();
    void initUiStatus();
    void initToolTips();
    void initData();
    void lookForFile(const QString &path);
    void addNewItem();
    void deleteItem();
    void clearTable();
    void clearAllTableItem();
    void clearTotleLineEdit();
    QString makeHtmlSontents();

    Ui::CodeWidget *ui;
    StatisticalThread *stThread;    //ͳ�Ʋ����߳�
    Parameter *paraDialog;          //���ò�������
    QList<FileResults*> fileResults;//�ļ�ͳ�ƽ������
    QStringList filesList;          //��Ҫͳ�Ƶ��ļ���·������
    QStringList format;             //��Ҫͳ���������ļ�����
    int files_TotleLines;           //ͳ�Ƶ������ļ���������
    int files_CodeLines;            //ͳ�Ƶ������ļ��Ĵ�������
    int files_NoteLines;            //ͳ�Ƶ������ļ���ע������
    int files_SpaceLines;           //ͳ�Ƶ������ļ��Ŀ�������
    int file_Numbers;               //�Ѿ�ͳ�Ƶ��ļ�

};

#endif // CODEWIDGET_H
