#include "statisticalthread.h"
#include "statisticalfunction.h"

StatisticalThread::StatisticalThread()
{

}

void StatisticalThread::setFilesList(QStringList files)
{
    filesList.clear();
    filesList = files;
}

void StatisticalThread::run()
{
    foreach (QString file, filesList)
    {

        //����ͳ�Ʒ�������ÿ���ļ��д�����
        StatisticalFunction function;
        function.setFile(file);
        function.Statistical();
        emit emitStatisticalStatus(function.getFileName(),
                                function.getTotleLines(),
                                function.getCodeLines(),
                                function.getNoteLines(),
                                function.getSpaceLines());
    }
}


