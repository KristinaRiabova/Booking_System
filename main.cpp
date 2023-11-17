
#include <string>


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