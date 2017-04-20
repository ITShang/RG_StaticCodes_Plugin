#include "statisticalfunction.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

StatisticalFunction::StatisticalFunction(QObject *parent) : QObject(parent)
{
    file = NULL;
}

bool StatisticalFunction::setFile(QString fileName)
{
    if(file == NULL)
    {
        file = new QFile(fileName);
    }

    if(!file->open(QIODevice::ReadOnly))
    {
        return false;
    }

    return true;
}

QString StatisticalFunction::getFileName()
{
    return file->fileName();
}

void StatisticalFunction::Statistical()
{
    int totleNum = 0;
    int codeNum = 0;
    int noteNum = 0;
    int spaceNum = 0;
    bool startShiftNote = false;
    int startAreaNote = 0;
    int codeAfterNote=0;    // */���ͺ��д������գ������ǲ����д���
    bool isSpaceLine=true;  //��ǰ���Ƿ��ǿ��У�Ĭ�ϳ�ʼ���ǿ���
    bool isCodeLine = false;//��ǰ���Ƿ��д��룬Ĭ��Ϊû�д���
    QChar ch;
    QChar preCh;

    if(file->size() == 0){
        fileLines = 0;
        codeLines = 0;
        noteLines = 0;
        spaceLines = 0;
        return;
    }


    QTextStream reader(file);

    while(!reader.atEnd())
    {
        reader>>ch;

        if(ch == '/'){
            isSpaceLine = false;
            if(preCh == '/'){
                if(startShiftNote){
                    noteNum++;
                    startShiftNote = false; //������ע��
                    reader.readLine();
                    totleNum++;
#ifdef Q_OS_MAC
                    isSpaceLine = true;
                    ch = '\xa';
#else
                    isSpaceLine = true;
                    ch = '\n';
#endif
                    if(codeAfterNote >0){
                        codeNum++;
                        codeAfterNote=0;
                    }
                }
            }
            else if(preCh == '*'){
                noteNum += startAreaNote; //��ʱΪ */ ����ע�ͽ������ͣ���ǰ��ע����������Ҫ��������ע�ͼ�����
                startAreaNote = 0;          //����ע�ͼ���������,��������ע��
            }
            else{
                if(startAreaNote == 0){     //���� '/' Ϊ������ע��,��ʱ����ע����Ч
                    startShiftNote = true;  //��ʼ��ע��
                }
            }
        }
        else if(ch == '*'){
            isSpaceLine = false;
            if(preCh == '/'){
                startAreaNote++;    // ��ʱΪ /* ���ͣ�����ע�ͼ�������һ
            }
            if(startShiftNote == true)
            {
                startShiftNote = false;
            }
        }
        else if(ch == '\xa'){
            if(startAreaNote>0){
                startAreaNote++;    // �����ʱ������ע�ͼ���������0����ǰ�Ļ�������ע�ͷ�Χ�ڣ�����ע�ͼ�������һ
            }
            else if(isSpaceLine == true)
            {
                spaceNum++;
            }
            else if(codeAfterNote >0){
                codeNum++;
                codeAfterNote=0;
            }
            totleNum++;
            isSpaceLine = true;
        }
        else if(ch == '\r'){

        }
        else if(ch == '\n'){

            if(startAreaNote>0){
                startAreaNote++;    // �����ʱ������ע�ͼ���������0����ǰ�Ļ�������ע�ͷ�Χ�ڣ�����ע�ͼ�������һ
            }
            else if(isSpaceLine == true)
            {
                spaceNum++;
            }
            else if(codeAfterNote >0 ){
                codeNum++;
                codeAfterNote=0;
            }
            totleNum++;
            isSpaceLine = true;
            isCodeLine = false; //1.8
        }
        else if(ch == '\t'){

        }
        else if(ch == ' '){

        }
        else if(ch == '\0'){

        }
        else{
            if(startAreaNote == 0){
                codeAfterNote++;
                isCodeLine = true;  //1.8
            }

            isSpaceLine = false;
        }

        preCh = ch;
    }

    fileLines = totleNum;
    codeLines = codeNum;
    noteLines = noteNum;
    spaceLines = spaceNum;
}

StatisticalFunction::~StatisticalFunction()
{
    file->close();
    delete file;
}
