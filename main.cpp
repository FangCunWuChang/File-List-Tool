#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>

void DFS(QStringList *pl,QString path,int mode,QString root,QString cheat)
{
    QDir dir(path);
    dir.setFilter(QDir::Dirs);
    for(uint i=0;i<dir.count();i++){
        if(!(dir[i]=="."||dir[i]=="..")){
            QString subpath=path+"/"+dir[i];
            QString printpath=subpath;
            if(mode==2||mode==3){
                printpath=printpath.right(printpath.size()-root.size());
                if(mode==3){
                    printpath.prepend(cheat);
                }
                if(mode==2){
                    printpath.prepend("..");
                }
            }
            qDebug()<<"     "<<printpath.toStdString().c_str();
            pl->append(subpath);
            DFS(pl,subpath,mode,root,cheat);
        }
    }
}

QStringList FS(QStringList *pl,int mode,QString root,QString cheat)
{
    QStringList result;
    for(int i=0;i<pl->size();i++){
        QDir dir(pl->at(i));
        dir.setFilter(QDir::Files);
        for(uint j=0;j<dir.count();j++){
            QString file=pl->at(i)+"/"+dir[j];
            QString printfile=file;
            if(mode==2||mode==3){
                printfile=printfile.right(printfile.size()-root.size());
                if(mode==3){
                    printfile.prepend(cheat);
                }
                if(mode==2){
                    printfile.prepend("..");
                }
            }
            qDebug()<<"     "<<printfile.toStdString().c_str();
            result.append(printfile);
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    qDebug("File list tool v1.0");
    qDebug("Copyright 2021 Wuchang.All rights reserved.");

    QString path=QCoreApplication::applicationDirPath();

    QTextStream qin(stdin);

    bool runflag=true;

    do{

        QStringList pathlist;



        qDebug()<<"workPath:"<<path.toStdString().c_str();

        qDebug("1 Standard Search  2 Search Without workPath  3 Path cheater  4 Change workPath  5 Quit");

        QString modein;
        qin>>modein;

        if(modein.size()!=1){
            qDebug("[ERROR]Bad mode!");
            continue;
        }

        int mode=1;

        mode=modein.toInt();

        if(mode<=0||mode>5){
            qDebug("[ERROR]Bad mode!");
            continue;
        }

        if(mode==5){
            return 0;
        }

        if(mode==4){
            qDebug("Input workPath:");
            QString patht;
            qin>>patht;

            QDir dt(patht);
            if(dt.exists()){
                path=dt.path();
                qDebug()<<"[DONE]Set workPath:"<<path.toStdString().c_str();
            }else{
                qDebug()<<"[ERROR]Bad path:"<<patht.toStdString().c_str();
            }
            continue;
        }

        QString cheat;

        if(mode==3){
            qDebug("Input cheat path:");

            qin>>cheat;
            while(cheat.endsWith('/')){
                cheat=cheat.left(cheat.size()-1);
            }
        }

        qDebug("Searching Paths...");
        {
            QString subpath=path;
            QString printpath=subpath;
            if(mode==2||mode==3){
                printpath=printpath.right(printpath.size()-path.size());
                if(mode==3){
                    printpath.prepend(cheat);
                }
                if(mode==2){
                    printpath.prepend("..");
                }
            }
            qDebug()<<"    "<<printpath.toStdString().c_str();
            pathlist.append(subpath);
        }
        DFS(&pathlist,path,mode,path,cheat);

        qDebug("Searching Files...");
        QStringList files=FS(&pathlist,mode,path,cheat);

        for(int i=0;i<pathlist.size();i++){
            if(mode==2||mode==3){
                QString pt=pathlist.at(i);
                pt=pt.right(pt.size()-path.size());
                if(mode==3){
                    pt.prepend(cheat);
                }
                if(mode==2){
                    pt.prepend("..");
                }
            }
        }

        qDebug("Finished!");
        qDebug("%d paths and %d files totally",pathlist.size(),files.size());
        qDebug("Save results?[y/N]");

        QString savef="N";
        qin>>savef;

        if(savef=="y"||savef=="Y"||savef=="Yes"||savef=="yes"||savef=="YES"){
            qDebug("Save results to:");
            QString sp;
            qin>>sp;

            QFile file(sp);
            if(file.open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Truncate)){
                QString text("File list tool v1.0\nCopyright 2021 Wuchang.All rights reserved.\n\n");
                text.append("workPath:"+path+"\n\n");
                text.append("Paths:\n");
                for(int i=0;i<pathlist.size();i++){
                    text.append(pathlist.at(i)+"\n");
                }
                text.append("\n");
                text.append("Files:\n");
                for(int i=0;i<files.size();i++){
                    text.append(files.at(i)+"\n");
                }
                text.append("\n");
                text.append(QString::asprintf("%d paths and %d files totally",pathlist.size(),files.size()));
                text.append("\n");
                text.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

                if(file.write(QByteArray(text.toUtf8()))>-1){
                    qDebug()<<"[DONE]Write results to file:"<<sp.toStdString().c_str();
                }else{
                    qDebug()<<"[ERROR]Can't write file:"<<sp.toStdString().c_str();
                }

                file.close();
            }else{
                qDebug()<<"[ERROR]Can't open file:"<<sp.toStdString().c_str();
            }
        }else{
            qDebug("Unsave results.");
        }

        qDebug("\n\n\n\n");

    }while(runflag);

    return 0;
}
