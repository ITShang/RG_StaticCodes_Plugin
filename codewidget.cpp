#include "codewidget.h"
#include "ui_codewidget.h"
#include <stdlib.h>
#include <statisticalthread.h>
#include <parameter.h>

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QTextCodec>
#include <QProcess>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QVariant>
#include <QDateTime>

#include <QDebug>

CodeWidget::CodeWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeWidget)
{
    ui->setupUi(this);
    initConnect();
    initUiStatus();
    initToolTips();
    initData();
}

//��ʼ���ۺ�������
void CodeWidget::initConnect()
{
    connect(ui->selectFile,SIGNAL( clicked(bool) ),
            this,SLOT( selectFile(bool) ));
    connect(ui->selectDir,SIGNAL( clicked(bool) ),
            this,SLOT( selectDir(bool) ));
    connect(ui->start,SIGNAL( clicked(bool) ),
            this,SLOT( startStatiscal(bool) ));
    connect(ui->lookCodeContents,SIGNAL( clicked(bool) ),
            this,SLOT( lookFileContents(bool) ));
    connect(ui->set,SIGNAL( clicked(bool) ),
            this,SLOT( setParameters(bool) ));
    connect(ui->quit,SIGNAL( clicked(bool) ),
            this,SLOT( quitThis(bool) ));
    connect(ui->makeForFile,SIGNAL( clicked(bool) ),
            this,SLOT( makeForFile(bool) ));
    connect(ui->fileTableList,SIGNAL( itemClicked(QTableWidgetItem*) ),
            this,SLOT( tableItemClicked(QTableWidgetItem*) ));
    connect(ui->fileTableList->horizontalHeader(),SIGNAL(sectionClicked(int )),
            this, SLOT(mySort(int)));

}

//��ʼ������Ŀؼ��ɲ���״̬
void CodeWidget::initUiStatus()
{
    ui->fileTableList->horizontalHeader()->setSortIndicatorShown(true);
    ui->fileTableList->setEditTriggers( QAbstractItemView::NoEditTriggers );
    ui->fileTableList->setSelectionBehavior(QAbstractItemView::SelectRows); //��ѡ������
}

//��ʼ���ؼ�����ʾ��Ϣ
void CodeWidget::initToolTips()
{

}

//��ʼ�����ݳ�Ա
void CodeWidget::initData()
{
    paraDialog = new Parameter();
    format = paraDialog->getFormatList();

    stThread = NULL;

    files_TotleLines = 0;
    files_CodeLines = 0;
    files_NoteLines = 0;
    files_SpaceLines = 0;
    file_Numbers =0;
}

//ѡ����Ҫͳ�Ƶĵ����ļ�
void CodeWidget::selectFile(bool)
{

    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);

    QString currPath = QDir::currentPath();

    //���ö�ȡ���ļ�����
    QString formatString;
    for(int i=0;i<format.count();i++)
    {
        formatString += format.at(i);
        formatString += tr(" (");
        formatString += format.at(i);
        formatString += tr(")");
        if(i!=format.count()-1)
        {
            formatString += tr(";;");
        }
    }

    filesList.clear();
    filesList = QFileDialog::getOpenFileNames(
                                        this,
                                        tr("ѡ���ļ�"),
                                        currPath,
                                        formatString);
    if(filesList.count()>0)
    {
        ui->showFileList->setText(tr("��ѡ��")
                                  + QString::number(filesList.count())
                                  + tr("���ļ� ��")
                                  + filesList.first());
    }
    else
    {
        ui->statusBar->showMessage(tr("ȡ��ѡ��û��ѡ���ļ���"));
        return;
    }

    //��ս�����ʾ�ͱ����Ŀ
    clearAllTableItem();
    clearTotleLineEdit();

}

//ѡ����Ҫͳ�Ƶ��ļ���Ŀ¼
void CodeWidget::selectDir(bool)
{
    QString currPath = QDir::currentPath();
    QString path = QFileDialog::getExistingDirectory(this,
                                                 tr("ѡ���ļ���"),
                                                 currPath);

    if(path == NULL)
    {
        ui->statusBar->showMessage(tr("ȡ��ѡ��û��ѡ���ļ��С�"));
        return;
    }
    ui->showFileList->setText(tr("ѡ���ļ���Ŀ¼��")+path);
    filesList.clear();

    lookForFile(path);

    //��ս�����ʾ�ͱ����Ŀ
    clearAllTableItem();
    clearTotleLineEdit();
}

//��ʼͳ��
void CodeWidget::startStatiscal(bool)
{   
    if(stThread == NULL)
    {
        stThread = new StatisticalThread();
        connect(stThread,SIGNAL( emitStatisticalStatus(QString,int,int,int,int) ),
                this,SLOT( updateTable(QString,int,int,int,int) ));
    }
    stThread->setFilesList(filesList);
    clearAllTableItem();
    clearTotleLineEdit();
    stThread->run();
}

//�鿴ѡ���ļ��ı���Դ����
void CodeWidget::lookFileContents(bool)
{
    if(ui->fileTableList->rowCount()<=0)
    {
        ui->statusBar->showMessage(tr("û�п�����ʾ���ļ�"));
        return;
    }
    int row = ui->fileTableList->currentRow();

    if(row<0)
    {
        ui->statusBar->showMessage(tr("δѡ����Բ������ļ�Ŀ¼"));
        return;
    }
    QString fileName = ui->fileTableList->item(row,0)->toolTip();

    //�򿪶���ı��ؼ��±����򣬴�Ԥ���ļ�


#ifdef Q_OS_MAC
    QProcess *icemProcess=new QProcess;
    QStringList arguments;
    QString bash=tr("open");
    arguments<<"-e"<<fileName;
    icemProcess->startDetached(bash,arguments);
#else
    QDesktopServices look;
    bool r = look.openUrl(QUrl(fileName));
    if(r == false)
    {
        QMessageBox::warning(this,tr("��ʾ"),tr("δ����ȷ���ļ�������ϵ������Ա"));
    }
#endif
}

//���ò���
void CodeWidget::setParameters(bool)
{
    if(paraDialog == NULL)
    {
        paraDialog = new Parameter();
    }

    if(paraDialog->exec())
    {
        format = paraDialog->getFormatList();
    }
}

//�ر����
void CodeWidget::quitThis(bool)
{
    this->close();
}

//���±���е�����
void CodeWidget::updateTable(QString fileName,int totleLines,int codeLines,int noteLines,int spaceLines)
{
    file_Numbers++;
    files_TotleLines += totleLines;
    files_CodeLines += codeLines;
    files_NoteLines += noteLines;
    files_SpaceLines += spaceLines;

    FileResults *fr = new FileResults;
    fr->filePath = fileName;
    fr->totleLines = totleLines;
    fr->codeLines = codeLines;
    fr->noteLines = noteLines;
    fr->spaceLines = spaceLines;
    fileResults.append(fr);

    ui->totleLines->setText(QString::number(files_TotleLines)
                            + tr("(��")
                            + QString::number(file_Numbers)
                            + tr("���ļ�)"));
    ui->totleCodeLines->setText(QString::number(files_CodeLines)
                                + tr("(")
                                + QString::number((float)files_CodeLines/files_TotleLines*100,'f',2)
                                + tr("%)"));
    ui->totleNoteLines->setText(QString::number(files_NoteLines)
                                + tr("(")
                                + QString::number((float)files_NoteLines/files_TotleLines*100,'f',2)
                                + tr("%)"));
    ui->totleSapceLines->setText(QString::number(files_SpaceLines)
                                 + tr("(")
                                 + QString::number((float)files_SpaceLines/files_TotleLines*100,'f',2)
                                 + tr("%)"));

    addNewItem();
    QFileInfo info(fileName);
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,0)->setText(info.fileName());
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,0)->setToolTip(fileName);


    ui->fileTableList->item(ui->fileTableList->rowCount()-1,1)->setData(2,QVariant(totleLines));
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,2)->setData(2,QVariant(codeLines));
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,3)->setData(2,QVariant(noteLines));
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,4)->setData(2,QVariant(spaceLines));

    ui->statusBar->showMessage(fileName+tr(" ͳ�����"));

}

//���һ���µ���Ŀ
void CodeWidget::addNewItem()
{
    ui->fileTableList->insertRow(ui->fileTableList->rowCount());

    for(int i=0;i<ui->fileTableList->columnCount();i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem();

        if(i == 0)
        {
            item->setTextColor(QColor(0,0,255,255));
        }
        ui->fileTableList->setItem(ui->fileTableList->rowCount()-1,i,item);
    }
}

//ɾ��һ����Ŀ
void CodeWidget::deleteItem()
{

}

//��ձ������ʾ����Ϣ
void CodeWidget::clearTable()
{

}

//��ձ����Ŀ
void CodeWidget::clearAllTableItem()
{
    for(int i=ui->fileTableList->rowCount()-1;i>=0;i--)
    {
        ui->fileTableList->removeRow(i);
    }
}

//��ս����·���ͳ����ʾ������
void CodeWidget::clearTotleLineEdit()
{
    files_TotleLines = 0;
    files_CodeLines = 0;
    files_NoteLines = 0;
    files_SpaceLines = 0;
    file_Numbers = 0;
    foreach (FileResults* fr, fileResults)
    {
        delete fr;
        fr = NULL;
    }
    fileResults.clear();

    ui->totleLines->setText(tr(""));
    ui->totleCodeLines->setText(tr(""));
    ui->totleNoteLines->setText(tr(""));
    ui->totleSapceLines->setText(tr(""));
}

//����ָ���ļ�Ŀ¼�µ��ļ�
void CodeWidget::lookForFile(const QString &path)
{
    QDir dir(path);

    foreach(QString file,dir.entryList(format,QDir::Files))
    {
        file = path + QDir::separator() + file;
        filesList.append(file);
    }

    foreach(QString subDir,dir.entryList(QDir::Dirs
                                         | QDir::NoDotAndDotDot))
    {
        lookForFile(path + QDir::separator() +subDir);
    }

    ui->showFileList->setText(tr("ѡ���ļ���Ŀ¼��")
                              + path
                              + tr("������ļ�")
                              + QString::number(filesList.count())
                              + tr("��"));
}

//����ͳ�ƽ��
void CodeWidget::makeForFile(bool)
{
    if(fileResults.count()<=0)
    {
        QMessageBox::warning(this,tr("��ʾ"),tr("�޿��ý��������"));
        return;
    }
    QString contents ;          //�������ļ�����->��Ҫд�뵽�ļ����ַ���

    //V2.0 д�뵽HTML�ļ���
    contents = makeHtmlSontents();

    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("�����ļ�"),
                                 tr("����ͳ�ƽ��"),tr("HTML (*.html)"));
    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        return;
    }

    QTextStream writer(&file);
    writer.setCodec(QTextCodec::codecForName("GB2312"));
    writer<<contents;

    file.close();
}

//����ָ����ʽ��Html�����ļ�����
QString CodeWidget::makeHtmlSontents()
{
    QString preMess;
    preMess += tr("<html>");
    preMess += tr("<body>");
    preMess += tr("<div align="">");
    preMess += tr("<p align=\"center\">");
    preMess += tr("<font face=\"����_GB2312\" size=\"5\">ͳ�ƽ��");
    preMess += tr("<p align=\"center\"><font face=\"Times New Roman\" size=\"3\">");
    preMess += QDateTime::currentDateTime().toString(tr("yyyy-MM-dd hh:mm:ss"));
    preMess += tr("<br><br>");

    preMess += tr("<div align=\"center\">");
    preMess += tr("<center>");
    preMess += tr("<table border=\"1\" width=\"100%\" cellspacing=\"1\"><br>");
    preMess += tr("<tr>");
    preMess += tr("<td width=\"15%\" align=\"center\"><font color=\"#800080\">�ļ�����</font></td>");
    preMess += tr("<td width=\"15%\" align=\"center\"><font color=\"#800080\">������</font></td>");
    preMess += tr("<td width=\"15%\" align=\"center\"><font color=\"#800080\">��������</font></td>");
    preMess += tr("<td width=\"15%\" align=\"center\"><font color=\"#800080\">ע������</font></td>");
    preMess += tr("<td width=\"15%\" align=\"center\"><font color=\"#800080\">�հ�����</font></td>");
    preMess += tr("</tr>");

    foreach (FileResults* fr, fileResults)
    {
        preMess += tr("<td width=\"40%\">");
        preMess += fr->filePath ;
        preMess += tr("</td>");
        preMess += tr("<td width=\"15%\">");
        preMess += QString::number(fr->totleLines);
        preMess += tr("</td>");
        preMess += tr("<td width=\"15%\">");
        preMess += QString::number(fr->codeLines);
        preMess += tr("</td>");
        preMess += tr("<td width=\"15%\">");
        preMess += QString::number(fr->noteLines);
        preMess += tr("</td>");
        preMess += tr("<td width=\"15%\">");
        preMess += QString::number(fr->spaceLines);
        preMess += tr("</td>");
        preMess += tr("</tr>");
    }

    //ͳ�ƽ������
    preMess += tr("<td width=\"40%\">");
    preMess += tr("ͳ�ƽ������");
    preMess += tr("</td>");
    preMess += tr("<td width=\"15%\">");
    preMess += ui->totleLines->text();
    preMess += tr("</td>");
    preMess += tr("<td width=\"15%\">");
    preMess += ui->totleCodeLines->text();
    preMess += tr("</td>");
    preMess += tr("<td width=\"15%\">");
    preMess += ui->totleNoteLines->text();
    preMess += tr("</td>");
    preMess += tr("<td width=\"15%\">");
    preMess += ui->totleSapceLines->text();
    preMess += tr("</td>");
    preMess += tr("</tr>");

    //����β��־
    preMess += tr("</table></center></div></body></html>");
    preMess += tr("<br><br><br>");

    return preMess;

}

//��Ӧ������¼�
void CodeWidget::tableItemClicked(QTableWidgetItem *item)
{
    if(item->column() == 0)
    {
        ui->statusBar->showMessage(item->toolTip());
    }
    else
    {
        ui->statusBar->showMessage(ui->fileTableList->item(item->row(),0)->toolTip());
    }
}

//�����ͷ����
void CodeWidget::mySort(int column)
{
    qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
           <<column
           <<"\n";
    ui->fileTableList->sortByColumn(column);
}

CodeWidget::~CodeWidget()
{
    if(stThread)
    {
        stThread->quit();
        stThread->wait();
        stThread->deleteLater();
    }

    if(paraDialog)
    {
        delete paraDialog;
        paraDialog = NULL;
    }

    foreach (FileResults* fr, fileResults)
    {
        delete fr;
        fr = NULL;
    }
    fileResults.clear();

    delete ui;
}
