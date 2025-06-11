#include <iostream>
#include <string>
#include <vector>

class RegularFlight {
    private:
        std::string time;
        std::string flightNumber;
    public:
        RegularFlight(std::string time, std::string flightNumber): time(time), flightNumber(flightNumber) {};
        std::string getTime() const { return time; }
        std::string getFlightNumber() const { return flightNumber; }
};

class Airline {
    private:
        std::string name;
        std::vector<RegularFlight> regularFlights;
    public:
        Airline(std::string name): name(name) {};
};
