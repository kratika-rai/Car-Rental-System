CREATE DATABASE CarRental;

USE CarRental;

CREATE TABLE customers (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100),
    phone VARCHAR(20)
);

CREATE TABLE cars (
    id INT AUTO_INCREMENT PRIMARY KEY,
    model VARCHAR(100),
    reg_no VARCHAR(20),
    available BOOLEAN DEFAULT true,
    rent_per_day FLOAT
);

CREATE TABLE bookings (
    id INT AUTO_INCREMENT PRIMARY KEY,
    customer_id INT,
    car_id INT,
    days INT,
    total_cost FLOAT,
    FOREIGN KEY (customer_id) REFERENCES customers(id),
    FOREIGN KEY (car_id) REFERENCES cars(id)
);
