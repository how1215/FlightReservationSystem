#include <iostream>
#include <string>
#include <vector>

class RegularFlight;

class SpecificFlight{
        private:
            std::string date;
            RegularFlight* regularFlight;
        public:
            SpecificFlight(const std::string& date,RegularFlight* regularFlight):date(date),regularFlight(regularFlight){}        

            std::string getDate() const { return date; }
            RegularFlight* getRegularFlight() const { return regularFlight; }
};

class RegularFlight {
    private:
        std::string time;
        std::string flightNumber;
        std::vector<SpecificFlight* > specificFlights;
    public:
        RegularFlight(const std::string& time,const std::string&  flightNumber): time(time), flightNumber(flightNumber) {};
        ~RegularFlight(){
            for(auto& s:specificFlights){
                delete s;
            }
        }

        std::string getTime() const { return time; }
        void setTime(const std::string& time){this->time=time;}
        std::string getFlightNumber() const { return flightNumber; }
        void setFlightNumber(const std::string& flightNumber){this->flightNumber=flightNumber;}
        std::vector<SpecificFlight* > getSpecificFlights() const { return specificFlights; }

        //新增Specific Flight
        void addSpecificFlight(const std::string& date){
            SpecificFlight* s = new SpecificFlight(date,this);
            //將新增的specific flight 儲存起來
            specificFlights.push_back(s);
        }
};


class Airline {
    private:
        std::string name;
        std::vector<RegularFlight* > regularFlights;
    public:
        Airline(const std::string& name): name(name) {}
        ~Airline(){
            for(auto& r:regularFlights){
                delete r;
            }
        }
        //新增某航空公司的regular flight
        RegularFlight* addRegularFlight(const std::string& time,const std::string& flightNumber){
            RegularFlight* r = new RegularFlight(time,flightNumber);
            regularFlights.push_back(r);
            return r;
        };

        //利用班次號碼來查詢某班regular flight的時間
        std::string findParticularRegular(const std::string& flightNumber){
            for(auto& r:regularFlights){
                if(r->getFlightNumber()==flightNumber){
                    return r->getTime();
                }
            }
            return "查無此航班";
        }

};

class PersonRole {
    
}; 

class Person {
    private:
        std::string name;
        std::string idNumber;

    public:
        Person(const std::string& name, const std::string& idNumber): name(name), idNumber(idNumber) {};
        std::string getName() const { return name; }
        std::string getIdNumber() const { return idNumber; }
};

int main(){

    Airline ootumlia("Ootumlia Airlines");
    
    RegularFlight* r1 = ootumlia.addRegularFlight("09:00", "111");
    RegularFlight* r2 = ootumlia.addRegularFlight("10:00", "222");
    RegularFlight* r3 = ootumlia.addRegularFlight("11:00", "333");
    RegularFlight* r4 = ootumlia.addRegularFlight("12:00", "444");
    
    r1->addSpecificFlight("20250101");
    r2->addSpecificFlight("20250102");
    r3->addSpecificFlight("20250103");
    r4->addSpecificFlight("20250104");

    std::string flightNumber;
    std::cin >> flightNumber;   
    std::cout << ootumlia.findParticularRegular(flightNumber) << std::endl;

    return 0;
}