#include "userprofiles.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>
#include <QVector>


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

/* Method returns true if current passwords exists in progam USERS profile */
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
                   jsArray.push_back(QJsonValue(temp));

                   //remove old array and insert new one
                   jsRootObj.remove("Data");
                   jsRootObj.insert("Data",QJsonValue(jsArray));
                   //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << jsRootObj << endl;

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
                   //oldDeb qDebug() << "There is no Data" << endl;

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
bool userProfiles::checkNewPassword(QString currUser,QString descr, QString lock, QString key1, QString key2, QWidget *parent)
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

    /* New fields must be different from other existing field at least in 1 section!
     * old description != new description OR
     * old login       != new login OR
     * old password    != new password
     */

    // store lines numbers of occurences of lock, key, descr of the DB into vector
    QVector<int> loginMatches = getDataOccurences(currUser, RESOURCE,    lock,  nullptr);
    QVector<int> pwdMatches   = getDataOccurences(currUser, PASSWORD,    key1,  nullptr);
    QVector<int> descrMatches = getDataOccurences(currUser, DESCRIPTION, descr, nullptr);

    bool isSameFields = isSameFieldsExist(descrMatches, loginMatches, pwdMatches, nullptr);
    if( isSameFields)
    {
        QMessageBox::information(parent,"New password validation","There is already exacty the same password, login and description");
        return false;
    }

    /* All fields are Ok */
    return true;
}

/* returns array of DB (one type) according to elementType-->(getTypeFlag enum)  */
QVector<QString> userProfiles::getArrayElement(QString strUsername, int elementType, QWidget *parent)
{
    const QString strResource = "lock";
    const QString strPwd      = "key";
    const QString strDescr    = "descr";

    QString strElemType;                        // holds string (1 of 3 upper)

    // define input element tpe to return
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


    QVector<QString> vecElements;                                // return vector
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

/* Stores lines numbers of occurences oflock || key || descr of the DB into vector
 * brief: Function gets string of type (getTypeFlag enum)
 * and loops through DB,
 * if that string == to some string in DB,
 * function ads the number (index) of its object into output vector.
 *
 * if "google" of type DESCRIPTION
 * output: 2, 4, 6
 * --> meaning that there is already "google"
 * as a description for objects with indexes 2, 4 and 6
 * in the database
 */
QVector<int> userProfiles::getDataOccurences(QString currUser,int elementType, QString strToCheck, QWidget *parent)
{
    QVector<int> vecLinesOccurence;

    // vector of descriptions, passwords or logins depending on elementType
    QVector<QString> tempData;

    /* Open file for R/W */
    QString appDir  = QCoreApplication::applicationDirPath();
    QString dataDir = (appDir+"/Database");
    //QJsonParseError jsonError;                                 //to check whether there is parse error

    /* Form filename for user database (from username) */
    QString fileName = "/User_";
    fileName.append(currUser);
    fileName.append(".json");

    /* Database for current user*/
    QFile fileUsersPwdDB(dataDir + fileName);

    /* Check if folder "Database" exists,
     * if no, then create
     */
    if( !QDir(dataDir).exists())
    {
        // no folder - no same data, return empty vector
        return vecLinesOccurence;
    }

    switch(elementType)
    {
      case DESCRIPTION:
       tempData = getArrayElement(currUser, userProfiles::DESCRIPTION, parent);
      break;

      case RESOURCE:
       tempData = getArrayElement(currUser, userProfiles::RESOURCE, parent);
      break;

      case PASSWORD:
       tempData = getArrayElement(currUser, userProfiles::PASSWORD, parent);
      break;
    }

    QVector<QString>::iterator it;
    int i = 0;
    QString strData;
    for( it = tempData.begin(); it != tempData.end(); it++, i++ )
    {
      strData = tempData.at(i);
      if( strData == strToCheck)
      {
          vecLinesOccurence.push_back(i);
      }
    }


    // return array
    return vecLinesOccurence;
}

/* Function gets vectors of lines (3), where some string is repeated and checks, whether
*  in all 3 vectors is the same line.
*  If true --> there is exactly the same descr+pwd+login in the db
*  If false --> new field is unique
*/
bool userProfiles::isSameFieldsExist( QVector<int> &descrIdxs, QVector<int> &loginIdxs, QVector<int> &pwdIdxs, QWidget *parent)
{
    //if at least 1 field is empty, the no occurences could be
    if( descrIdxs.isEmpty() || loginIdxs.isEmpty() || pwdIdxs.isEmpty())
    {
        // at least one field is unique (no matter what exactly)
        return false;
    }

    QVector<int>::iterator it_i, it_j, it_k;
    int i = 0, j = 0, k = 0;

    // going through all elements and compare one with each other
    for( it_i = descrIdxs.begin(), i = 0; it_i != descrIdxs.end(); it_i++, i++ )
    {
      // get current description match line number
      int descrLineNext = descrIdxs.at(i);

      for( it_j = loginIdxs.begin(), j = 0; it_j != loginIdxs.end(); it_j++, j++ )
      {
          // get current login match line number
          int loginLineNext = loginIdxs.at(j);

          for( it_k = pwdIdxs.begin(), k = 0; it_k != pwdIdxs.end(); it_k++, k++ )
          {
              // get current password match line number
             int pwdLineNext = pwdIdxs.at(k);

             // compare
             if( descrLineNext==loginLineNext && descrLineNext == pwdLineNext)
             {
                 //there is line exactly the same!
                 return true;
             }
          }
      }
    }

  // no identical occurences in the DB
  return false;
}

/* opens password databse and copies its content into QString fileContent
 * or returns fail
 */
bool userProfiles::getPwdDB(QString moduleName, QString username, QString &fileContent, QWidget *parent)
{
    /* Open file for Reading */
    QString appDir  = QCoreApplication::applicationDirPath();
    QString dataDir = (appDir+"/Database");
    //QJsonParseError jsonError;                                 //to check whether there is parse error

    /* Form filename for user database (from username) */
    QString fileName = "/User_";
    fileName.append(username);
    fileName.append(".json");

    /* Database for current user*/
    QFile fileUsersPwdDB(dataDir + fileName);

    /* Check if folder "Database" exists,
     */
    if( !QDir(dataDir).exists())
    {
        // no folder - no DB
        return false;
    }

    /* Open file with users list */
    if( !fileUsersPwdDB.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QMessageBox::critical(parent, tr("Getting database"), tr("Could not open database."));
        return false;
    }

    // file was opened...

    // copy content to reference
    fileContent = fileUsersPwdDB.readAll(); // read file into string (users list)

    // close file
    fileUsersPwdDB.close();

    /* Normal end of function */
    return true;
}

bool userProfiles::deleteLockKeyPair(QString currUser, QString lock, QString key, QString description, QWidget *parent)
{
    bool isFounded = false;
    const QString moduleName = "Deleting password.";

    // PWD DB will be here
    QString strPwdDB;

    //get db content into strPwdDB
    bool isGotPwdDB = this->getPwdDB(moduleName, currUser, strPwdDB, parent);
    if ( !isGotPwdDB)
    {
        /* fail + info msg */
        return false;
    }

    // convert String to jsDocument
    QJsonDocument jsDoc = QJsonDocument::fromJson(strPwdDB.toUtf8());

    /* Check json for validity */
    bool isJsValid = isJSONvalid( moduleName, jsDoc, parent);
    if ( !isJsValid)
    {
        /* fail + info msg */
        return false;
    }

    if( jsDoc.isObject())
    {
        QJsonObject jsObjRoot = jsDoc.object();

        if( jsObjRoot.contains("Data"))
        {
            QJsonValue  jsValueTemp  = jsObjRoot.value("Data");
            QJsonObject jsObjectTemp;

            if( jsValueTemp.isArray())
            {
               QJsonArray jsArrayData = jsValueTemp.toArray();

               //here need to find appropriate field
               QJsonArray::iterator it;
               int i = 0;
               for( i = 0, it = jsArrayData.begin(); it != jsArrayData.end(); it++, i++)  //going through all elements of the array
               {
                  jsValueTemp = jsArrayData.at(i);
                  jsObjectTemp = jsValueTemp.toObject();

                  if( jsObjectTemp.contains("key") && jsObjectTemp.contains("descr") &&  jsObjectTemp.contains("lock"))
                  {
                      QJsonValue jsValKey   = jsObjectTemp.value("key");
                      QJsonValue jsValDescr = jsObjectTemp.value("descr");
                      QJsonValue jsValLock  = jsObjectTemp.value("lock");

                      QString strKey   = jsValKey.toString();
                      QString strDescr = jsValDescr.toString();
                      QString strLock  = jsValLock.toString();

                      //check for equaity (identity) with input parameters
                      if( strKey == key && strDescr == description && strLock == lock )
                      {
                          jsArrayData.removeAt(i);
                          isFounded = true;
                          break; // force quit for loop
                      }
                  }
               }//end of for

               if( isFounded)
               {
                   jsObjRoot.remove("Data");
                   jsObjRoot.insert("Data", QJsonValue(jsArrayData));

                   QJsonDocument jsDocNew(jsObjRoot);

                   //write changes to DB file
                   rewritePwdDB(moduleName, currUser, jsDocNew, parent);
                   return true;
               }
            }
        }
    }



    return false;
}

/* Check json for validity */
bool userProfiles::isJSONvalid(QString strModule, QJsonDocument &jsDoc, QWidget *parent)
{
    /* Check json for validity */
    if(jsDoc.isNull())
    {
        QMessageBox::critical(parent, strModule , tr("Internal error. Invalid JSON file."));
        //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ <<"main file is not in JSON format!" << endl;
        return false;
    }
    else
    {
        /* Normal end */
        return true;
    }
}

bool userProfiles::rewritePwdDB(QString moduleName, QString username, QJsonDocument &jsDoc, QWidget *parent)
{
    /* Open file for Writing */
    QString appDir  = QCoreApplication::applicationDirPath();
    QString dataDir = (appDir+"/Database");

    /* Form filename for user database (from username) */
    QString fileName = "/User_";
    fileName.append(username);
    fileName.append(".json");

    /* Database for current user*/
    QFile fileUsersPwdDB(dataDir + fileName);

    /* Check if folder "Database" exists,
     */
    if( !QDir(dataDir).exists())
    {
        // no folder - no DB
        return false;
    }

    /* Open file with users list */
    if( !fileUsersPwdDB.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::critical(parent, tr("Rewriting database"), tr("Could not open database."));
        return false;
    }

    // file was opened...

    fileUsersPwdDB.write(jsDoc.toJson());

    // close file
    fileUsersPwdDB.close();

    /* Normal end of function */
    return true;
    return true;
}

















