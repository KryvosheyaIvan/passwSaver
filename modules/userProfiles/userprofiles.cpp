#include "userprofiles.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>


userProfiles::userProfiles(QObject *parent) : QObject(parent)
{

}

/*
 * Adding user profile to file
 * returns true if success
*/
bool userProfiles::addUserProfile(QString username, QString pswd, QWidget *parent)
{
    if(username.length() < MIN_STRING_LEN) {
        QMessageBox::information(parent, "Incorrect value", "Too short username");
        return false;
    }
    else if(pswd.length() < MIN_STRING_LEN) {
        QMessageBox::information(parent, "Incorrect value", "Too short password");
        return false;
    }

    /* Open file for writing */
    QString appDir = QCoreApplication::applicationDirPath();
    QFile file(appDir + "/users.json");


    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "addUserProfile: failed to open file" << endl;
        return false;
    }
    QString strFile = file.readAll();

    //first need to read from file (in order to know what was written previously)
    QJsonDocument jsReadDoc;
    jsReadDoc = QJsonDocument::fromJson(strFile.toUtf8());
    file.close();


    if (jsReadDoc.isEmpty()) {
        qDebug() << "doc is empty" << endl;
    }
    else {
        qDebug() << "doc is not an empty" << endl;
    }

    QByteArray byteArray = jsReadDoc.toJson();
    qDebug() << byteArray << endl;

    QJsonObject jsObject;
    jsObject["username"] = username;
    jsObject["password"] = pswd;

    QJsonValue jsValue(jsObject);

    QJsonArray jsArray;

    if ( jsReadDoc.isArray()) {
        qDebug() << "doc is array" << endl;
        jsArray = jsReadDoc.array();        // copying old array
    }

    //need to know whether that username was not already registered
    if ( isUsernameExist(username, parent) )
    {
        QMessageBox::information(parent, "Registration", "That user was already registered!\n"
                                                         "Please choose another username..."
                                                         );
        return false;
    }

    jsArray.append(jsValue);

    //qDebug() << "freeing jsReadDoc" << endl;
    //~jsReadDoc();
    jsReadDoc.setArray(jsArray);


    if ( !file.open(QIODevice::WriteOnly)) {
        qDebug() << "addUserProfile: failed to open file 2" << endl;
        return false;
    }
    file.write(jsReadDoc.toJson());
    file.close();

    return true;
}

bool userProfiles::isUsernameExist(QString username, QWidget *parent)
{
    /* Open file for reading */
    QString appDir = QCoreApplication::applicationDirPath();
    QFile file(appDir + "/users.json");

    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << __FILE__ << __LINE__ << "isUsernameExist: failed to open file" << endl;
        return false;
    }

    QString strFile = file.readAll();
    QJsonDocument jsReadDoc;
    jsReadDoc = QJsonDocument::fromJson(strFile.toUtf8());
    file.close();

    //Error checking
    if(jsReadDoc.isNull())
    {
        qDebug() << __FILE__ << __LINE__ << "isUsernameExist: failed to parse json" << endl;
        QMessageBox::critical(parent, "Internal error", "Error parsing json");
        return false;
    }

    QJsonArray jsArray = jsReadDoc.array();
    QJsonValue jsVal;
    QJsonObject jsObj;
    QJsonArray::iterator it;
    int i = 0;
    for( it = jsArray.begin(); it != jsArray.end(); it++)
    {
       jsVal = jsArray.at(i);
       jsObj = jsVal.toObject();
       if ( jsObj.contains("username"))
       {
        //qDebug() << "[" << i << "]" << "Ok!" << endl;
        jsVal = jsObj.value("username");
        if ( jsVal.toString() == username)
        {
            qDebug() << __FILE__ <<__LINE__ << ": current user was previously registered!" << endl;
            /* If we here then current user was previously registered! */
            return true;
        }
       }
       i++;
    }

    /* current username is unique */
    return false;
}

/* Method returns true if current passwords exists */
bool userProfiles::isPswdExist(QString pswd, QWidget *parent)
{
    /* Open file for reading */
    QString appDir = QCoreApplication::applicationDirPath();
    QFile file(appDir + "/users.json");

    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << __FILE__ << __LINE__ << "isPswdExist: failed to open file" << endl;
        return false;
    }

    /* read file contant to a string */
    QString strFile = file.readAll();
    QJsonDocument jsReadDoc;
    jsReadDoc = QJsonDocument::fromJson(strFile.toUtf8());
    file.close();

    QJsonArray jsArray = jsReadDoc.array();
    QJsonValue jsVal;
    QJsonObject jsObj;
    QJsonArray::iterator it;
    int i = 0;
    for( it = jsArray.begin(); it != jsArray.end(); it++)
    {
       jsVal = jsArray.at(i);
       jsObj = jsVal.toObject();
       if ( jsObj.contains("password"))
       {
        qDebug() << "[" << i << "]" << "Ok!" << endl;
        jsVal = jsObj.value("password");
        //qDebug() << jsVal.toString() << ":" << pswd << endl;
        if ( jsVal.toString() == pswd)
        {
            qDebug() << "current pswd was previously registered!" << endl;
            /* If we here then current pswd was previously registered! */
            return true;
        }
       }
       i++;
    }

    /* current password was not registered */
    qDebug() << __FILE__ << __LINE__ << ": current password was not registered " << endl;
    return false;
}

/* Method called when user tries to Log In */
bool userProfiles::Login(QString username, QString pswd, QWidget *parent)
{
    bool isUsernameOk, isPswdOk;

    /* Check whether current user and password was registered earlier */
    isUsernameOk = isUsernameExist(username, parent);
    isPswdOk = isPswdExist(pswd, parent);

    if( isUsernameOk && isPswdOk ) {
        //QMessageBox::information(this,"Login","Successful login!");
        qDebug() << __FILE__ << __LINE__ << " Successful login!" << endl;
       return true;
    }
    else {
        qDebug() << __FILE__ << __LINE__ << " Login failed!" << endl;
        return false;
    }

    return false;
}

/* add new resource and password */
bool userProfiles::addLockKeyPair(QString currUser, QString lock, QString key, QString description, QWidget *parent)
{
   /* Open file for R/W */
   QString appDir = QCoreApplication::applicationDirPath();
   QFile jsFile(appDir + "/users.json");
   QJsonParseError jsonError;                                 //to check whether there is parse error  

   /* Form filename for user database (from username) */
   QString fileName = "/User_";
   fileName.append(currUser);
   fileName.append(".json");

   /* Database */
   QFile fileUserData(appDir + fileName);

   if( !jsFile.open(QIODevice::ReadWrite | QIODevice::Text) )
   {
       qDebug() << __FILE__ << __LINE__ << ": addLockKeyPair: failed to open file" << endl;
       return false;
   }

   // file was opened...

   QString strFile = jsFile.readAll(); // read file into string
   QJsonDocument jsDoc;
   jsDoc = QJsonDocument::fromJson(strFile.toUtf8(), &jsonError);

   jsFile.close(); //close file (on hard drive)

   /* Check json for validity */
   if(jsDoc.isNull())
   {
       /* Form error message */
       QString errorType = jsonError.errorString();
       QString userMessage = "Internal error. Invalid JSON file.\n Error type: ";
       userMessage.append(errorType);

       QMessageBox::critical(parent,"Adding Password to database", userMessage);
       qDebug() << __FILE__ << __LINE__ << ": addLockKeyPair: main file is not in JSON format!" << endl;
       return false;
   }

   // JSON format is OK...

   /* If it is not array -> close */
   if ( !jsDoc.isArray())
   {
       QMessageBox::critical(parent,"Adding Password to database", "InternalError");
       qDebug() << __FILE__ << __LINE__ << ": addLockKeyPair: JSON is not array!" << endl;
       return false;
   }

   //JSON is array...

   /* Finding JSON object with current user in the database */
   QJsonArray jsArray = jsDoc.array(); //get array

   //temporary json structures
   QJsonValue  jsValueTemp;
   QJsonObject jsObjectTemp;
   QJsonArray::iterator it;      //iterator
   int i;
   for( i = 0, it = jsArray.begin(); it != jsArray.end(); it++, i++)  //going through all elements of the array
   {
       jsValueTemp  = jsArray.at(i); //get current Value
       jsObjectTemp = jsValueTemp.toObject();

       if( jsObjectTemp.contains("username"))
       {
           jsValueTemp = jsObjectTemp.value("username"); //get username of current element

           /* Check if this is profile of current User */
           if( jsValueTemp.toString() == currUser)
           {
               /* Check whether database for current user exists */
               if( fileUserData.exists())
               {
                   qDebug() << "is Data" << endl;
                   return true;

               }
               else
               {
                   qDebug() << "There is no Data" << endl;
                   QJsonArray dataArray;
                   auto temp = QJsonObject(
                   {
                      qMakePair(QString("lock"),  QJsonValue(lock)),
                      qMakePair(QString("key"),   QJsonValue(key)),
                      qMakePair(QString("descr"), QJsonValue(description)),
                   });
                   dataArray.push_front(QJsonValue(temp));


                   /*Open file for writing*/
                   if( !fileUserData.open(QIODevice::WriteOnly) )
                   {
                       qDebug() << __FILE__ << __LINE__ << ": addLockKeyPair: failed to open file" << endl;
                       return false;
                   }
                   /* Insert named array "Data"
                    * "Data" = [  ... ]
                   */
                   jsObjectTemp["Data"] = dataArray;
                   qDebug() << jsObjectTemp << endl;

                   /* Create JsonDoc from final object and write it to file */
                   QJsonDocument finalJsonDoc(jsObjectTemp);
                   fileUserData.write(finalJsonDoc.toJson());
                   fileUserData.close();
               }
           }
       }
   }//end of for




   //QByteArray byteArray = jsDoc.toJson();
   //qDebug() << "file: " << byteArray << endl;

   /* Password and resource successfully added */
   return true;
}

/* validation of new password and resource for correctness */
bool userProfiles::checkNewPassword(QString lock, QString key1, QString key2, QWidget *parent)
{
    /* Check lock */
    if( lock.isEmpty())
    {
        QMessageBox::information(parent,"New password validation","Resource field is empty!");
        return false;
    }

    /* Check keys */
    if( key1.isEmpty() || key2.isEmpty() || (key1 != key2))
    {
        QMessageBox::information(parent,"New password validation","Password must be typed two times and be equal.");
        return false;
    }

    /* Check lengths */
    if( (lock.length() < MIN_STRING_LEN) )
    {
        QString temp("Resource must be at least ");
        QString length = QString::number(MIN_STRING_LEN); //convert number to QString
        QString temp2(" characters long.");

        //form string "Resource must be at least X characters long."
        temp.append(length);
        temp.append(temp2);

        // "Resource must be at least X characters long."
        QMessageBox::information(parent,"New password validation", temp);
        return false;
    }
    else if(key1.length() < MIN_STRING_LEN)
    {
        QString temp("Password must be at least ");
        QString length = QString::number(MIN_STRING_LEN); //convert number to QString
        QString temp2(" characters long.");

        //form string "Password must be at least X characters long."
        temp.append(length);
        temp.append(temp2);

        // "Password must be at least X characters long."
        QMessageBox::information(parent,"New password validation", temp);
        return false;
    }

    /* All field are Ok */
    return true;
}







