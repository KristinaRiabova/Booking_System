#include <iostream>
#include <string>
#include <vector>


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
