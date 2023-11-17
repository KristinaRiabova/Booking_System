#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

class Ticket {
private:
    std::string passengerName;
    int row;
    int seatNumber;
    std::string date;
    std::string flightNumber;
    int price;
    int confirmationID;
    bool isReturned;

public:
    Ticket(const std::string& name, int row, int seat, const std::string& date,
           const std::string& flightNumber, int price, int confirmationID)
            : passengerName(name), row(row), seatNumber(seat), date(date),
              flightNumber(flightNumber), price(price), confirmationID(confirmationID),
              isReturned(false) {
    }

    std::string getPassengerName() const {
        return passengerName;
    }

    int getRow() const {
        return row;
    }

    int getSeat() const {
        return seatNumber;
    }

    bool getIsReturned() const {
        return isReturned;
    }

    void setReturned() {
        isReturned = true;
    }

    std::string getDate() const {
        return date;
    }

    std::string getFlightNumber() const {
        return flightNumber;
    }

    int getPrice() const {
        return price;
    }

    int getConfirmationID() const {
        return confirmationID;
    }
};
class Airplane {
private:
    std::string date;
    std::string flightNumber;
    int seatsPerRow;
    int rangeStart1, rangeEnd1, price1, rangeStart2, rangeEnd2, price2;
    std::vector<std::vector<bool>> seatAvailability;
    std::vector<std::vector<int>> seatPrices;
    int totalrows;
    int totalseats;

public:
    Airplane(const std::string &date, const std::string &flightNumber, int seatsPerRow,
             int rangeStart1, int rangeEnd1, int price1,
             int rangeStart2, int rangeEnd2, int price2)
            : date(date), flightNumber(flightNumber), seatsPerRow(seatsPerRow),
              rangeStart1(rangeStart1), rangeEnd1(rangeEnd1), price1(price1),
              rangeStart2(rangeStart2), rangeEnd2(rangeEnd2), price2(price2) {
        seatAvailability.resize(rangeEnd2, std::vector<bool>(seatsPerRow, true));
        seatPrices.resize(rangeEnd2, std::vector<int>(seatsPerRow, 0));
        totalrows = rangeEnd2;
        totalseats = seatsPerRow * rangeEnd2;

        for (int row = 1; row <= totalrows; ++row) {
            for (int seat = 1; seat <= seatsPerRow; ++seat) {
                int price;
                if (row >= rangeStart1 && row <= rangeEnd1) {
                    price = price1;
                } else if (row >= rangeStart2) {
                    price = price2;
                } else {
                    std::cerr << "Invalid seat range for seat " << seat << " in row " << row << std::endl;
                    continue;
                }
                seatPrices[row - 1][seat - 1] = price;
            }
        }
    }

    std::string getDate() const {
        return date;
    }

    std::string getFlightNumber() const {
        return flightNumber;
    }

    bool isSeatAvailable(int row, int seatNumber) const {
        if (row >= 1 && row <= totalrows && seatNumber >= 1 && seatNumber <= seatsPerRow) {
            return seatAvailability[row - 1][seatNumber - 1];
        }
        return false;
    }

    void setSeatAvailability(int row, int seatNumber, bool availability) {
        if (row >= 1 && row <= totalrows && seatNumber >= 1 && seatNumber <= seatsPerRow) {
            seatAvailability[row - 1][seatNumber - 1] = availability;
        }
    }

    void setSeatPrice(int row, int seatNumber, int price) {
        if (row >= 1 && row <= totalrows && seatNumber >= 1 && seatNumber <= seatsPerRow) {
            seatPrices[row - 1][seatNumber - 1] = price;
        }
    }

    bool bookSeat(int row, int seatNumber) {
        if (isSeatAvailable(row, seatNumber)) {
            seatAvailability[row - 1][seatNumber - 1] = false;
            int price = getPriceForSeat(row, seatNumber);
            setSeatPrice(row, seatNumber, price);
            return true;
        }
        return false;
    }

    int getPriceForSeat(int row, int seatNumber) const {
        if (row >= 1 && row <= totalrows) {
            return seatPrices[row - 1][seatNumber - 1];
        }
        return -1;
    }

    void displayAvailableSeats() const {
        std::cout << date << " " << flightNumber << ":\n";

        int seatCounter = 1; // Counter for seat numbers

        for (int row = 1; row <= totalrows; ++row) {
            for (int seat = 1; seat <= seatsPerRow; ++seat) {
                char seatLabel = 'A' + (seat - 1);

                if (isSeatAvailable(row, seat)) {
                    int price = getPriceForSeat(row, seat);
                    std::cout << seatCounter << seatLabel << " $" << price << "  ";
                } else {
                    std::cout << "BOOKED ";
                }

                seatCounter++;
            }
            std::cout << std::endl;
        }

        std::cout << "Total Rows: " << totalrows << std::endl;
        std::cout << "Total Seats: " << totalseats << std::endl;
    }

};


class FileHandler {
public:
    static bool readFile(const std::string& fileName, std::vector<std::string>& lines) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cout << "Error: Unable to open the file: " << fileName << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        file.close();
        return true;
    }

    static bool writeFile(const std::string& fileName, const std::vector<std::string>& lines) {
        std::ofstream file(fileName);
        if (!file.is_open()) {
            std::cout << "Error: Unable to open the file for writing: " << fileName << std::endl;
            return false;
        }

        for (const std::string& line : lines) {
            file << line << '\n';
        }

        file.close();
        return true;
    }
};

class ConfigReader {
private:
    FileHandler fileHandler;
    std::string fileName;

public:
    ConfigReader(const std::string& configFileName) : fileName(configFileName) {}

    bool readConfig(std::vector<Airplane>& airplanes) {
        std::vector<std::string> lines;

        if (!fileHandler.readFile(fileName, lines)) {
            std::cout << "Error: Unable to read the configuration file." << std::endl;
            return false;
        }

        for (const std::string& line : lines) {
            std::istringstream ss(line);
            std::string date, flightNumber;
            int seatsPerRow;

            std::map<int, int> seatPrices;


            int rangeStart1, rangeEnd1, price1, rangeStart2, rangeEnd2, price2;


            std::vector<std::string> tokens;
            std::string token;
            while (ss >> token) {
                if (token.find('-') != std::string::npos) {
                    // Handle ranges by splitting them into two separate tokens
                    std::istringstream rangeStream(token);
                    std::string rangeToken;
                    while (std::getline(rangeStream, rangeToken, '-')) {
                        tokens.push_back(rangeToken);
                    }
                } else {
                    tokens.push_back(token);
                }
            }


            if (tokens.size() != 9) {
                std::cerr << "Invalid configuration line: " << line << std::endl;
                continue;
            }


            std::cout << "Processing line: " << line << std::endl;

            date = tokens[0];
            flightNumber = tokens[1];
            seatsPerRow = std::stoi(tokens[2]);
            rangeStart1 = std::stoi(tokens[3]);
            rangeEnd1 = std::stoi(tokens[4]);

            // Remove '$' and convert to int
            std::string price1_str = tokens[5];
            price1_str.erase(std::remove(price1_str.begin(), price1_str.end(), '$'), price1_str.end());

            try {
                price1 = std::stoi(price1_str, nullptr);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid price format: " << price1_str << std::endl;
                continue; // Skip to the next line
            }

            // Add the new range and price to the seatPrices map
            for (int seat = rangeStart1; seat <= rangeEnd1; seat++) {
                seatPrices[seat] = price1;
            }

            rangeStart2 = std::stoi(tokens[6]);
            rangeEnd2 = std::stoi(tokens[7]);


            std::string price2_str = tokens[8];
            price2_str.erase(std::remove(price2_str.begin(), price2_str.end(), '$'), price2_str.end());

            try {
                price2 = std::stoi(price2_str, nullptr);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid price format: " << price2_str << std::endl;
                continue; // Skip to the next line
            }

            // Add the second range and price to the seatPrices map
            for (int seat = rangeStart2; seat <= rangeEnd2; seat++) {
                seatPrices[seat] = price2;
            }
            for (int seat = rangeStart1; seat <= rangeEnd1; seat++) {
                seatPrices[seat] = price1;
            }

            for (int seat = rangeStart2; seat <= rangeEnd2; seat++) {
                seatPrices[seat] = price2;
            }

            airplanes.push_back(Airplane(date, flightNumber, seatsPerRow, rangeStart1, rangeEnd1, price1, rangeStart2, rangeEnd2, price2));
        }

        return true;
    }
};

class BookingSystem {
private:
    std::vector<Airplane> airplanes;
    std::vector<Ticket> bookedTickets;
    int bookingConfirmationID = 1;
    FileHandler fileHandler;

public:
    BookingSystem(const std::string& configFileName) : fileHandler() {
        ConfigReader configReader(configFileName);
        configReader.readConfig(airplanes);
    }


    void checkAvailability(const std::string& date, const std::string& flightNumber) {
        for (const Airplane& airplane : airplanes) {
            if (airplane.getDate() == date && airplane.getFlightNumber() == flightNumber) {
                airplane.displayAvailableSeats();
                return;
            }
        }
        std::cout << "Flight not found." << std::endl;
    }

    bool bookTicket(const std::string& date, const std::string& flightNumber, const std::string& seat, const std::string& passengerName) {
        for (Airplane& airplane : airplanes) {
            if (airplane.getDate() == date && airplane.getFlightNumber() == flightNumber) {
                int row = std::stoi(seat.substr(0, seat.size() - 1));
                int seatNumber = seat[seat.size() - 1] - 'A' + 1;
                if (airplane.isSeatAvailable(row, seatNumber)) {
                    if (airplane.bookSeat(row, seatNumber)) {
                        int price = airplane.getPriceForSeat(row, seatNumber);
                        Ticket ticket(passengerName, row, seatNumber, date, flightNumber, price, bookingConfirmationID);
                        bookedTickets.push_back(ticket);
                        std::cout << "Confirmed with ID: " << bookingConfirmationID << std::endl;
                        bookingConfirmationID++;
                        return true;
                    }
                }
            }
        }
        std::cout << "Failed to book the ticket. Flight or seat not found or seat is already booked." << std::endl;
        return false;
    }

    void returnTicket(int confirmationID) {
        auto it = std::find_if(bookedTickets.begin(), bookedTickets.end(),
                               [confirmationID](const Ticket& ticket) {
                                   return ticket.getConfirmationID() == confirmationID;
                               });

        if (it != bookedTickets.end() && !it->getIsReturned()) {
            // Refund the price and make the seat available again
            for (Airplane& airplane : airplanes) {
                if (airplane.getDate() == it->getDate() && airplane.getFlightNumber() == it->getFlightNumber()) {
                    int row = it->getRow();
                    int seatNumber = it->getSeat();
                    airplane.setSeatPrice(row, seatNumber, it->getPrice());
                    airplane.setSeatAvailability(row, seatNumber, true); // Make the seat available again
                    break;
                }
            }

            std::cout << "Confirmed refund for " << it->getPassengerName() << ": $" << it->getPrice() << std::endl;


            it->setReturned();
        } else {
            std::cout << "Confirmation ID not found or ticket already returned." << std::endl;
        }
    }

    void viewBookedTickets(int confirmationID) {
        for (const Ticket& ticket : bookedTickets) {
            if (ticket.getConfirmationID() == confirmationID) {
                std::cout << "Flight " << ticket.getFlightNumber() << ", " << ticket.getDate()
                          << ", seat " << ticket.getRow() << static_cast<char>('A' + ticket.getSeat() - 1)
                          << ", price " << ticket.getPrice() << "$, " << ticket.getPassengerName() << std::endl;
                return;
            }
        }
        std::cout << "Confirmation ID not found." << std::endl;
    }

    void viewBookedTicketsByUsername(const std::string& username) {
        for (const Ticket& ticket : bookedTickets) {
            if (ticket.getPassengerName() == username) {
                std::cout << "Flight " << ticket.getFlightNumber() << ", " << ticket.getDate()
                          << ", seat " << ticket.getRow() << static_cast<char>('A' + ticket.getSeat() - 1)
                          << ", price " << ticket.getPrice() << "$" << std::endl;
            }
        }
    }

};
class CommandLineInterface {
public:
    void showMenu() {
        std::cout << "===== Booking System Menu =====\n"
                  << "1. Check seat availability\n"
                  << "2. Book a ticket\n"
                  << "3. Return a ticket\n"
                  << "4. View booked tickets by ID\n"
                  << "5. View booked tickets by username\n"
                  << "6. Exit\n"
                  << "==============================\n";
    }

    void processCommand(const std::string& command) {
        if (command == "1") {
            std::string date, flightNumber;
            std::cout << "Enter date and flight number (e.g., 1.12.23 FR12): ";
            std::cin >> date >> flightNumber;
            bookingSystem.checkAvailability(date, flightNumber);
        } else if (command == "2") {
            std::string date, flightNumber, seat, username;
            std::cout << "Enter date, flight number, seat, and username: ";
            std::cin >> date >> flightNumber >> seat >> username;
            bookingSystem.bookTicket(date, flightNumber, seat, username);
        } else if (command == "3") {
            int confirmationID;
            std::cout << "Enter confirmation ID: ";
            std::cin >> confirmationID;
            bookingSystem.returnTicket(confirmationID);
        } else if (command == "4") {
            int confirmationID;
            std::cout << "Enter confirmation ID: ";
            std::cin >> confirmationID;
            bookingSystem.viewBookedTickets(confirmationID);
        } else if (command == "5") {
            std::string username;
            std::cout << "Enter username: ";
            std::cin >> username;
            bookingSystem.viewBookedTicketsByUsername(username);
        } else if (command == "6") {
            std::cout << "Exiting the program. Thank you!\n";
            exit(0);
        } else {
            std::cout << "Invalid command. Please enter a number from the menu.\n";
        }
    }

    void run() {
        while (true) {
            showMenu();
            std::string command;
            std::cout << "Enter command number: ";
            std::cin >> command;
            processCommand(command);
        }
    }

private:
    BookingSystem bookingSystem{"airplane_config.txt"};
};

int main() {
    CommandLineInterface cli;
    cli.run();

    return 0;
}
