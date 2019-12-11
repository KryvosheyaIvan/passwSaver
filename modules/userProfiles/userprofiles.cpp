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
   QString appDir  = QCoreApplication::applicationDirPath();
   QString dataDir = (appDir+"/Database");
   QFile jsFileUsers(appDir + "/users.json");
   QJsonParseError jsonError;                                 //to check whether there is parse error  

   /* Form filename for user database (from username) */
   QString fileName = "/User_";
   fileName.append(currUser);
   fileName.append(".json");

   /* Check if folder "Database" exists,
    * if no, then create
    */
   if( !QDir(dataDir).exists())
   {
      QDir().mkdir(dataDir);
   }

   /* Database for current user*/
   QFile fileUsersPwdDB(dataDir + fileName);

   /* Open file with users list */
   if( !jsFileUsers.open(QIODevice::ReadWrite | QIODevice::Text) )
   {
       QMessageBox::critical(parent,"Adding Password to database", "Internal error.");
       return false;
   }

   // file was opened...

   QString strFileUsers = jsFileUsers.readAll(); // read file into string (users list)
   QJsonDocument jsDoc;
   jsDoc = QJsonDocument::fromJson(strFileUsers.toUtf8(), &jsonError); // try to recognize json-format

   //close file (on hard drive)
   jsFileUsers.close();

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
       QMessageBox::critical(parent,"Adding Password to database", "InternalError.");
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
               QJsonArray dataArray; // array "Data", where all ress and pwds will be saved to

               //form json object
               auto temp = QJsonObject(
               {
                  qMakePair(QString("lock"),  QJsonValue(lock)),
                  qMakePair(QString("key"),   QJsonValue(key)),
                  qMakePair(QString("descr"), QJsonValue(description)),
               });

               //put pwds into array
               dataArray.push_front(QJsonValue(temp));

               /* Check whether database file for current user exists */
               if( fileUsersPwdDB.exists())
               {
                   qDebug() << "is Data" << endl;

                   /* Open file for Reading */
                   if( !fileUsersPwdDB.open(QIODevice::ReadOnly) )
                   {
                       QMessageBox::critical(parent,"Adding Password to database", "Internal error.");
                       return false;
                   }

                   //file is opened...

                   /*  parse and modify content of the file */
                   /*  Open file. Conver it to QJsonDocument->QJsonObject,
                    *  then get initial array, copy it to newly created array.
                    *  Add new object to array(pwd,res,descr).
                    *  Delete from initial Jsonobject initial array,
                    *  put new array instead of old
                    */
                   QJsonDocument jsOldContent;

                   // read file into string (users data)
                   QString strOldFile = fileUsersPwdDB.readAll();

                   //close file
                   fileUsersPwdDB.close();

                   // try to recognize json-format
                   jsOldContent = QJsonDocument::fromJson(strOldFile.toUtf8(), &jsonError);

                   // check json for validity
                   if( jsOldContent.isNull() || !jsOldContent.isObject() )
                   {
                       //form error message
                       QString errorType = jsonError.errorString();
                       QString userMessage = "Internal error. Corrupted JSON structure.\n Error type: ";
                       userMessage.append(errorType);
                       QMessageBox::critical(parent,"Adding Password to database", userMessage);
                       return false;
                   }

                   //get jsObject from jsDocument
                   QJsonObject jsRootObj  = jsOldContent.object();

                   if( !jsRootObj.contains("Data"))
                   {
                       //form error message
                       QString errorType = jsonError.errorString();
                       QString userMessage = "Internal error. Corrupted JSON structure.\n Error type: ";
                       userMessage.append(errorType);
                       QMessageBox::critical(parent,"Adding Password to database", userMessage);
                       return false;
                   }

                   // check for validity and get array "Data" of interest
                   QJsonValue jsValue = jsRootObj.value("Data");

                   if( !jsValue.isArray())
                   {
                       //form error message
                       QString errorType = jsonError.errorString();
                       QString userMessage = "Internal error. Corrupted JSON structure.\n Error type: ";
                       userMessage.append(errorType);
                       QMessageBox::critical(parent,"Adding Password to database", userMessage);
                       return false;
                   }

                   QJsonArray jsArray = jsValue.toArray();

                   //push new values into array
                   jsArray.push_front(QJsonValue(temp));

                   //remove old array and insert new one
                   jsRootObj.remove("Data");
                   jsRootObj.insert("Data",QJsonValue(jsArray));
                   qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << jsRootObj << endl;

                   /* Open file for Writing (previous content is lost! */
                   if( !fileUsersPwdDB.open(QIODevice::WriteOnly) )
                   {
                       QMessageBox::critical(parent,"Adding Password to database", "Internal error.");
                       return false;
                   }

                   // write updates jsDoc into the file
                   QJsonDocument jsDocUpdated(jsRootObj);
                   fileUsersPwdDB.write(jsDocUpdated.toJson());
                   fileUsersPwdDB.close();
                   return true;

               }
               else
               {
                   // no file with users DB...

                   // create file and write first lock and key pair...
                   qDebug() << "There is no Data" << endl;

                   /*Open file for writing*/
                   if( !fileUsersPwdDB.open(QIODevice::WriteOnly) )
                   {
                       qDebug() << __FILE__ << __LINE__ << ": addLockKeyPair: failed to open file" << endl;
                       QMessageBox::critical(parent,"Adding Password to database", "Internal error.");
                       return false;
                   }
                   /* Insert named array "Data"
                    * "Data" = [  ... ]
                   */
                   jsObjectTemp["Data"] = dataArray;

                   /* Create JsonDoc from final object and write it to file */
                   // jsDoc will contain info about user owning this file and pwd for app
                   QJsonDocument jsDocFinal(jsObjectTemp);
                   fileUsersPwdDB.write(jsDocFinal.toJson());
                   fileUsersPwdDB.close();
               }
           }
       }
   }//end of for


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

    /* All fields are Ok */
    return true;
}


QVector<QString> userProfiles::getArrayElement(QString strUsername, int elementType, QWidget *parent)
{
    const QString strResource = "lock";
    const QString strPwd      = "key";
    const QString strDescr    = "descr";
    QString strElemType;

    switch (elementType) {
       case userProfiles::PASSWORD:
        strElemType = strPwd;
        break;
       case userProfiles::RESOURCE:
        strElemType = strResource;
        break;
       case userProfiles::DESCRIPTION:
        strElemType = strDescr;
        break;
    default:
        break;
    }


    QVector<QString> vecElements;                             // return vector
    QString strAppDir = QCoreApplication::applicationDirPath();  // application folder
    QString strDataDir= (strAppDir+"/Database");                 // DB folder

    QString strUsrPwdFile = "/User_";                            // Users password DB name
    strUsrPwdFile.append(strUsername);                           //
    strUsrPwdFile.append(".json");                               //

    QJsonParseError jsonError;                                   //possible error type holder

    QFile fileUsersPwdDB(strDataDir + strUsrPwdFile);            // File - DB for current user

    // Open file of users passwords (if exists)
    if( !QDir(strDataDir).exists() || !fileUsersPwdDB.exists())
    {
        //no directory or DBfile - no database

        //return empty vector
        vecElements.clear();
        return vecElements;
    }

    // opening DB
    if( !fileUsersPwdDB.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(parent, "Loading database", "Internal error.");
        return vecElements;
    }

    //file is opened...

    //read file content into string
    QJsonDocument jsDocRoot;
    QString strFileContent = fileUsersPwdDB.readAll();
    jsDocRoot = QJsonDocument::fromJson(strFileContent.toUtf8(), &jsonError);

    //check for validity
    if( jsDocRoot.isNull() || !jsDocRoot.isObject())
    {
        //form error message
        QString errorType = jsonError.errorString();
        QString userMessage = "Internal error. Corrupted JSON structure.\n Error type: ";
        userMessage.append(errorType);
        QMessageBox::critical(parent,"Loading database", userMessage);
        return vecElements;
    }

    //close file
    fileUsersPwdDB.close();

    //parse to object
    QJsonObject jsObjRoot = jsDocRoot.object();

    if(jsObjRoot.contains("Data"))
    {
        QJsonValue jsValTemp = jsObjRoot.value("Data");

        //if array - continue
        if( jsValTemp.isArray())
        {
            // get array Data and fill vector with actual values
            QJsonArray jsArrData = jsValTemp.toArray();


            QJsonValue jsValTemp;       // res + pwd + descr
            QJsonObject jsObjTemp;      // res + pwd + descr
            QJsonArray::iterator it;    // iterators
            QString strTemp;
            int i = 0;
            for( it = jsArrData.begin(); it != jsArrData.end(); it++, i++)
            {
               jsValTemp = jsArrData.at(i);
               if(jsValTemp.isObject())
               {
                  jsObjTemp = jsValTemp.toObject();

                  //get lock(resource)
                  if( jsObjTemp.contains(strElemType))
                  {
                      jsValTemp = jsObjTemp.value(strElemType);
                      strTemp = jsValTemp.toString();
                      //push lock to return vector
                      vecElements.push_back(strTemp);
                  }
               }
            }
        }
        //else ret empty vector
    }

    //else...

    //empty
    return vecElements;
}





























