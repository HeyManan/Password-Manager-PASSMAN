#include "PASSMAN.h"

extern sql::Driver* driver = get_driver_instance();;
extern sql::Connection* con = driver->connect(server, db_username, db_password);

int main()
{
    Database db;
    db.set_schema();
	db.create_table();


    User user;
    user.user_menu();
    return 0;
}