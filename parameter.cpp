#include "parameter.h"
#include "ui_parameter.h"
#include <QDebug>

Parameter::Parameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameter)
{
    ui->setupUi(this);
    initFormats();
    initListWidget();
    initConnect();
}

//��ʼ���ṩ���û�ѡ��ĸ�ʽ�б�
void Parameter::initFormats()
{
    formats += tr("*.h");
    formats += tr("*.cpp");
    formats += tr("*.idl");

    formatList = formats;
}

//��ʼ���ۺ���
void Parameter::initConnect()
{
    connect(ui->selectAllItem,SIGNAL( stateChanged(int) ),
            this,SLOT( selectAllItems(int) ));
    connect(ui->formatListWidget,SIGNAL( itemClicked(QListWidgetItem*) ),
            this,SLOT( setItemChecked(QListWidgetItem*) ));
    connect(ui->decide,SIGNAL( clicked(bool) ),
            this,SLOT( setDefineFormat(bool) ));
    connect(ui->cancel,SIGNAL( clicked(bool) ),
            this,SLOT( cancelFormat(bool) ));
}

//��ʼ����ȡ�ļ�����
void Parameter::initListWidget()
{
    int i=0;
    foreach(QString format,formats)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(format);
        item->setCheckState(Qt::Checked);
        ui->formatListWidget->insertItem(i,item);
        i++;
    }
}

//���ȫѡ
void Parameter::selectAllItems(int state)
{
    switch(state)
    {
    case 0:
        for(int i=0;i<ui->formatListWidget->count();i++)
        {
            ui->formatListWidget->item(i)->setCheckState(Qt::Unchecked);
        }
        break;
    case 2:
        for(int i=0;i<ui->formatListWidget->count();i++)
        {
            ui->formatListWidget->item(i)->setCheckState(Qt::Checked);
        }
        break;
    }
}

//����б�ѡ��״̬
void Parameter::setItemChecked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
    {

    }
}

//ȷ������
void Parameter::setDefineFormat(bool)
{
    formatList.clear();
    for(int i=0;i<ui->formatListWidget->count();i++)
    {
        if(ui->formatListWidget->item(i)->checkState() == Qt::Checked)
        {
            setFormats(i);
        }
    }
    accept();
}

//ȷ������ĸ�ʽ����
void Parameter::setFormats(int index)
{
    formatList.append(formats.at(index));
}

//ȡ������
void Parameter::cancelFormat(bool)
{
    this->hide();
}

//��ȡ�����еĶ�ȡ�ļ������б�
QStringList Parameter::getFormatList()
{
    return formatList;
}

//��������ļ���Ϣ
void Parameter::readIniFile()
{

}

Parameter::~Parameter()
{
    delete ui;
}
