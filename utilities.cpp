#include "PASSMAN.h"

/*Function definations of Database Class*/

void Database::set_schema()
{
    try
    {
        stmt = con->createStatement();
        stmt->execute("CREATE DATABASE IF NOT EXISTS PASSMAN");
        con->setSchema("PASSMAN");
        delete stmt;
    }
    catch (sql::SQLException e)
    {
        cout << "Schema setting UNSUCCESSFULL. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}

void Database::create_table()
{
    try 
    {
        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS users (username VARCHAR(30) PRIMARY KEY, password VARCHAR(30));");
        stmt->execute("CREATE TABLE IF NOT EXISTS credentials (site VARCHAR(30), s_username VARCHAR(30), s_password VARCHAR(30), username VARCHAR(30), FOREIGN KEY (username) REFERENCES users(username));");
        delete stmt;
    }
    catch (sql::SQLException e)
    {
        cout << "Table Creation Unsuccessfull. Error message: " << e.what() << endl;
        system("pause");
        exit(0);
    }
}

/*Function definations of Database Class over*/

/*Function definations of User Class*/

void User::login()
{
    cout << "Enter Login credentials:" << endl;
    cout << "Username/Email:" << endl;
    cin >> username;
    cout << "Password:" << endl;
    cin >> password;

    //Check if user exist
    string exists_username = "", exists_password = "";
    pstmt = con->prepareStatement("SELECT * FROM Users WHERE username = ?;");
    pstmt->setString(1, username);
    result = pstmt->executeQuery();
    while (result->next())
    {
        exists_username = result->getString(1).c_str();
        exists_password = result->getString(2).c_str();
        break;
    }
    delete pstmt;

    int flag = 0;

    cout << endl;
    if (exists_username != username) cout << "User with username : " << username << " does not exists." << endl;
    else if(exists_password != password) cout << "Incorrest Password." << endl;
    else 
    {
        flag = 1;
        cout << "LOGIN SUCCESSFUL." << endl;
    }

    system("pause");
    system("cls");

    if (flag)
    {
        Credentials credentials(username);
        credentials.credentials_menu();
    }
    else user_menu();
}

void User::signup()
{
    cout << "Enter the Details to Register:" << endl;
    cout << "Username/Email:" << endl;
    cin >> username;
    cout << "Password:" << endl;
    cin >> password;

    //check if user already exists.
    string exists = "";
    pstmt = con->prepareStatement("SELECT * FROM users WHERE username = ?;");
    pstmt->setString(1, username);
    result = pstmt->executeQuery();
    while (result->next()) 
    {
        exists = result->getString(1).c_str();
        break;
    }
    delete pstmt;

    cout << endl;
    if (exists != "")
    {
        cout << "User already exists." << endl;
    }
    else
    {
        try 
        {
            pstmt = con->prepareStatement("INSERT INTO users(username, password) VALUES(?,?)");
            pstmt->setString(1, username);
            pstmt->setString(2, password);
            pstmt->executeQuery();
            delete pstmt;

            cout << "User registered Successfully." << endl;
        }
        catch (sql::SQLException e)
        {
            cout << "User not registerd successfully. Error message: " << e.what() << endl;
        }
    }

    system("pause");
    system("cls");
    user_menu();
}

void User::user_menu()
{
    int choice;
    cout << "Select Valid Option:" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Exit" << endl;
    cout << endl << "Enter your choice:" << endl;
    cin >> choice;

    system("cls");
    switch (choice)
    {
    case 1:
        login();
        break;
    case 2:
        signup();
        break;
    case 3:
        exit(1);
    default:
        cout << "----PLEASE ENTER VALID OPTION----" << endl;
        user_menu();
        break;
    }
}

/*Function definations of User Class over*/

/*Function definations of Credentials Class*/

Credentials::Credentials(string curr_user)
{
    current_user = curr_user;
}

void Credentials::getall_credentials()
{
    cout << "Select site to view it's credentials." << endl;
    cout << "Press 0 to go BACK." << endl;
    cout << endl;

    for (int cred = 0; cred < credentials.size(); cred++)
    {
        cout << cred + 1 << ". " << credentials[cred][0] << endl;
    }

    int choice;
    cout << endl << "Select the valid option:" << endl;
    cin >> choice;

    system("cls");
    if (choice > credentials.size())
    {
        cout << "----PLEASE SELECT VALID OPTION----" << endl;
        getall_credentials();
    }
    else if(choice == 0)
    {
        credentials_menu();
    }
    else
    {
        get_site_credentials(--choice);
    }
}

void Credentials::get_site_credentials(int index)
{
    cout << "Site: " << credentials[index][0] << endl;
    cout << "Username: " << credentials[index][1] << endl;
    cout << "Password: " << credentials[index][2] << endl;
    
    system("pause");
    system("cls");
    getall_credentials();
}

void Credentials::saved_credentials()
{
    if (credentials.size() == 0) 
    {
        cout << "NO CREDENTIALS SAVED YET." << endl;
        cout << endl;
        system("pause");
        system("cls");
        credentials_menu();
    }
    else 
    {
        getall_credentials();
    }

}

void Credentials::add_credentials()
{
    cout << "Add new credentials:" << endl;
    cout << "Enter site name:" << endl;
    cin >> site_name;
    cout << "Enter site username/email:" << endl;
    cin >> site_username;
    cout << "Enter site password:" << endl;
    cin >> site_password;

    system("cls");
    cout << "----PLEASE CONFIRM DETAILS----" << endl;
    cout << "Site: " << site_name << endl;
    cout << "Username: " << site_username << endl;
    cout << "Password: " << site_password << endl
        << endl;

    int choice;
    cout << "Press 1 to SAVE" << endl;
    cout << "Press 0 to DISCARD" << endl;
    cin >> choice;

    cout << endl;
    if (choice == 1)
    {
        try
        {
            pstmt = con->prepareStatement("INSERT INTO CREDENTIALS(site, s_username, s_password, username) VALUES(?,?,?,?)");
            pstmt->setString(1, site_name);
            pstmt->setString(2, site_username);
            pstmt->setString(3, site_password);
            pstmt->setString(4, current_user);
            pstmt->executeQuery();
            delete pstmt;

            cout << "CREDENTIALS SAVED SUCCESSFULLY." << endl;
        }
        catch (sql::SQLException e)
        {
            cout << "Credentials addition unsuccessful. Error message: " << e.what() << endl;
        }
    }
    else cout << "CREDENTIALS DISCARDED." << endl;

    system("pause");
    system("cls");
    credentials_menu();
}

void Credentials::credentials_menu()
{
    try
    {
        pstmt = con->prepareStatement("SELECT site, s_username, s_password FROM credentials WHERE username = ?");
        pstmt->setString(1, current_user);
        result = pstmt->executeQuery();
        delete pstmt;

        while (result->next())
        {
            vector<string> cred;
            cred.push_back(result->getString(1).c_str());
            cred.push_back(result->getString(2).c_str());
            cred.push_back(result->getString(3).c_str());
            credentials.push_back(cred);
        }
    }
    catch (sql::SQLException e)
    {
        cout << "Error while fetching credentials. Error message: " << e.what() << endl;
    }

    int choice;
    cout << "Select a Valid Option:" << endl;
    cout << "1. Show saved credentials" << endl;
    cout << "2. Add new credentials" << endl;
    cout << "3. Logout" << endl;
    cout << "4. Exit" << endl;
    cout << endl << "Enter your choice:" << endl;
    cin >> choice;

    system("cls");
    User user;

    switch (choice)
    {
    case 1:
        saved_credentials();
        break;
    case 2:
        add_credentials();
        break;
    case 3:
        user.user_menu();
        break;
    case 4:
        exit(1);
    default:
        cout << "----PLEASE ENTER VALID OPTION----" << endl;
        credentials_menu();
        break;
    }
}
