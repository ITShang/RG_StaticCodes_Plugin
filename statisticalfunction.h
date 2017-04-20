//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� statisticalfunction.h
//���ߣ�   �г�     Version��1.0    Date��2016.XX.XX
//������   ͳ�Ʒ�������
//������
//�޸���ʷ��
//2016.12.05:sc:    ������Windowsƽ̨�µĿ���ͳ�ƹ���
//                  ������Mac�½��Windows�´������ļ�ͳ�ƹ���
//**************************************************

#ifndef STATISTICALFUNCTION_H
#define STATISTICALFUNCTION_H

#include <QObject>
class QFile;

class StatisticalFunction : public QObject
{
    Q_OBJECT
public:
    explicit StatisticalFunction(QObject *parent = 0);
    ~StatisticalFunction();
    bool setFile(QString fileName); //������Ҫͳ�Ƶ�һ���ļ�
    QString getFileName();          //��ȡ��ǰͳ�Ƶ��ļ���
    void Statistical();             //ͳ�Ƶĺ���������

    //��ȡͳ�ƽ��:�ļ�������
    int  getTotleLines(){return  fileLines;}

    //��ȡͳ�ƽ��:�ļ���������
    int  getCodeLines(){return  codeLines;}

    //��ȡͳ�ƽ��:ע������
    int  getNoteLines(){return  noteLines;}

    //��ȡͳ�ƽ��:�ո�����
    int  getSpaceLines(){return  spaceLines;}


signals:

public slots:

private:
    QFile *file;        //��Ҫ����ͳ�Ƶ��ļ�
    int fileLines;      //�ļ�����������
    int codeLines;      //����������
    int noteLines;      //ע��������
    int spaceLines;     //�ո�����

};

#endif // STATISTICALFUNCTION_H
