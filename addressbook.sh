#!/bin/bash

DB_FILE="address_book.txt"

create_db() {
    if [ ! -f "$DB_FILE" ]; then
        touch "$DB_FILE"
        echo "Database created successfully as $DB_FILE."
    else
        echo "Database '$DB_FILE' already exists."
    fi
}

insert_record() {
    echo "--- Insert New Contact ---"
    read -p "Enter Name: " name
    read -p "Enter Phone: " phone
    read -p "Enter Email: " email

    if [ -z "$name" ] || [ -z "$phone" ] || [ -z "$email" ]; then
        echo "Error: All fields are required!"
        return
    fi

    echo "$name,$phone,$email" >> "$DB_FILE"
    echo "Contact added successfully!"
}

view_records() {
    echo "--- Address Book Contacts ---"
    if [ ! -s "$DB_FILE" ]; then
        echo "The address book is empty."
        return
    fi

    printf "%-20s %-15s %-25s\n" "Name" "Phone" "Email"
    echo "------------------------------------------------------------"

    while IFS=, read -r name phone email; do
        printf "%-20s %-15s %-25s\n" "$name" "$phone" "$email"
    done < "$DB_FILE"
}

delete_record() {
    echo "--- Delete Contact ---"
    read -p "Enter the exact name of the contact to delete: " search_name

    if grep -q "^$search_name," "$DB_FILE"; then
        grep -v "^$search_name," "$DB_FILE" > temp.txt
        mv temp.txt "$DB_FILE"
        echo "Contact '$search_name' deleted successfully."
    else
        echo "Contact '$search_name' not found."
    fi
}

modify_record() {
    echo "--- Modify Contact ---"
    read -p "Enter the exact name of the contact to modify: " search_name

    if grep -q "^$search_name," "$DB_FILE"; then
        echo "Contact found. Enter new details:"
        read -p "Enter New Phone: " new_phone
        read -p "Enter New Email: " new_email

        if [ -z "$new_phone" ] || [ -z "$new_email" ]; then
            echo "Error: Phone and Email cannot be empty!"
            return
        fi

        grep -v "^$search_name," "$DB_FILE" > temp.txt
        echo "$search_name,$new_phone,$new_email" >> temp.txt
        mv temp.txt "$DB_FILE"
        echo "Contact updated successfully."
    else
        echo "Contact '$search_name' not found."
    fi
}

# --- Main Menu Loop ---
while true; do
    echo ""
    echo "===== ADDRESS BOOK ====="
    echo "1. Create Database File"
    echo "2. Insert Contact"
    echo "3. View All Contacts"
    echo "4. Delete Contact"
    echo "5. Modify Contact"
    echo "6. Exit"
    read -p "Choose an option [1-6]: " choice
    echo ""

    case $choice in
        1) create_db ;;
        2) [ -f "$DB_FILE" ] && insert_record || echo "Error: Create the database file first (Option 1)." ;;
        3) [ -f "$DB_FILE" ] && view_records || echo "Error: Create the database file first (Option 1)." ;;
        4) [ -f "$DB_FILE" ] && delete_record || echo "Error: Create the database file first (Option 1)." ;;
        5) [ -f "$DB_FILE" ] && modify_record || echo "Error: Create the database file first (Option 1)." ;;
        6) echo "Exiting Address Book. Goodbye!"; exit 0 ;;
        *) echo "Invalid option! Please choose a number between 1 and 6." ;;
    esac
done