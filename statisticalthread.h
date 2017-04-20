//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� statisticalthread.h
//���ߣ�   �г�     Version��1.0    Date��2016.10.27
//������   ͳ���߳�
//������
//�޸���ʷ��
//2016.12.05:sc:    ���#include <QStringList>��ȷ��Windows�µı���
//**************************************************

#ifndef STATISTICALTHREAD_H
#define STATISTICALTHREAD_H

#include <QThread>
#include <QStringList>

class StatisticalThread : public QThread
{
    Q_OBJECT

public:
    StatisticalThread();
    void run();

    //������Ҫͳ�ƴ�����ļ�
    void setFilesList(QStringList files);

signals:
    //����ͳ���ļ��������Ϣ
    void emitStatisticalStatus(QString,int,int,int,int);

private:
    //׼��ͳ�Ƶ��ļ��б�
    QStringList filesList;
};

#endif // STATISTICALTHREAD_H
