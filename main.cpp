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
