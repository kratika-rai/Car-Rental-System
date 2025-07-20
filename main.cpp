#include <iostream>
#include <iomanip>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

sql::mysql::MySQL_Driver *driver;
sql::Connection *con;

void connectDB() {
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "your_password");
    con->setSchema("CarRental");
}

void registerCar() {
    string model, reg_no;
    float rent;
    cout << "Enter model: ";
    cin >> model;
    cout << "Enter registration no: ";
    cin >> reg_no;
    cout << "Enter rent per day: ";
    cin >> rent;

    sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO cars(model, reg_no, rent_per_day) VALUES (?, ?, ?)");
    pstmt->setString(1, model);
    pstmt->setString(2, reg_no);
    pstmt->setDouble(3, rent);
    pstmt->execute();
    delete pstmt;

    cout << "Car registered successfully.\n";
}

void registerCustomer() {
    string name, phone;
    cout << "Enter customer name: ";
    cin >> name;
    cout << "Enter phone: ";
    cin >> phone;

    sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO customers(name, phone) VALUES (?, ?)");
    pstmt->setString(1, name);
    pstmt->setString(2, phone);
    pstmt->execute();
    delete pstmt;

    cout << "Customer registered successfully.\n";
}

void bookCar() {
    int cust_id, car_id, days;
    cout << "Enter customer ID: ";
    cin >> cust_id;
    cout << "Enter car ID: ";
    cin >> car_id;
    cout << "Enter number of days: ";
    cin >> days;

    sql::PreparedStatement *check = con->prepareStatement("SELECT available, rent_per_day FROM cars WHERE id = ?");
    check->setInt(1, car_id);
    sql::ResultSet *res = check->executeQuery();

    if (res->next() && res->getBoolean("available")) {
        float rent = res->getDouble("rent_per_day");
        float total = rent * days;

        sql::PreparedStatement *book = con->prepareStatement(
            "INSERT INTO bookings(customer_id, car_id, days, total_cost) VALUES (?, ?, ?, ?)"
        );
        book->setInt(1, cust_id);
        book->setInt(2, car_id);
        book->setInt(3, days);
        book->setDouble(4, total);
        book->execute();

        sql::PreparedStatement *update = con->prepareStatement("UPDATE cars SET available = false WHERE id = ?");
        update->setInt(1, car_id);
        update->execute();

        cout << "Car booked! Total cost: ₹" << total << endl;

        delete book;
        delete update;
    } else {
        cout << "Car not available!\n";
    }

    delete res;
    delete check;
}

void returnCar() {
    int booking_id;
    cout << "Enter booking ID to return car: ";
    cin >> booking_id;

    sql::PreparedStatement *get = con->prepareStatement("SELECT car_id FROM bookings WHERE id = ?");
    get->setInt(1, booking_id);
    sql::ResultSet *res = get->executeQuery();

    if (res->next()) {
        int car_id = res->getInt("car_id");
        sql::PreparedStatement *update = con->prepareStatement("UPDATE cars SET available = true WHERE id = ?");
        update->setInt(1, car_id);
        update->execute();

        cout << "Car returned successfully.\n";

        delete update;
    } else {
        cout << "Invalid booking ID!\n";
    }

    delete res;
    delete get;
}

void showMenu() {
    cout << "\n==== Car Rental System ====\n";
    cout << "1. Register Car\n";
    cout << "2. Register Customer\n";
    cout << "3. Book Car\n";
    cout << "4. Return Car\n";
    cout << "5. Exit\n";
    cout << "===========================\n";
}

int main() {
    try {
        connectDB();

        int choice;
        while (true) {
            showMenu();
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: registerCar(); break;
                case 2: registerCustomer(); break;
                case 3: bookCar(); break;
                case 4: returnCar(); break;
                case 5: cout << "Exiting...\n"; return 0;
                default: cout << "Invalid option.\n";
            }
        }

    } catch (sql::SQLException &e) {
        cerr << "MySQL error: " << e.what() << endl;
    }
    return 0;
}
