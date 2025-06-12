#include <iostream>
#include <string>
#include <vector>

//Forward declaration
class RegularFlight;
class Person;

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
        RegularFlight(const std::string& time, const std::string& flightNumber): time(time), flightNumber(flightNumber) {};
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
        Airline(const std::string& name): name(name) {};
};

class PersonRole {
    private:
        Person* person;
    public:
        PersonRole(Person* person): person(person) {};
        Person* getPerson() const { return person; }
        void setPerson(Person* person){this->person=person;}
        //純虛函數供子類別繼承後override
        virtual void displayRole() = 0;
}; 

class Person {
    private:
        std::string name;
        std::string idNumber;
        //一個人可以有多個職位
        std::vector<PersonRole* > roles;

    public:
        Person(const std::string& name, const std::string& idNumber): name(name), idNumber(idNumber) {};
        std::string getName() const { return name; }
        std::string getIdNumber() const { return idNumber; }


        void addRole(PersonRole* role){
            roles.push_back(role);
        }

        //顯示這位使用者的所有職位
        void displayRoles() const {
            std::cout << name << " (" << idNumber << ") has roles :\n";
            for (const auto& role : roles) {
                role->displayRole();
            }
        }
    };


//兩個職位class都繼承自PersonRole
class PassegerRole: public PersonRole{
    public:
        PassegerRole(Person* person): PersonRole(person) {};
        void displayRole(){
            std::cout << "Passenger" << std::endl;
        }
};

class EmployeeRole: public PersonRole{
    public:
        EmployeeRole(Person* person): PersonRole(person) {};
        void displayRole(){
            std::cout << "Employee" << std::endl;
        }
};

int main(){

    // Airline ootumlia("Ootumlia Airlines");
    // //Creating a new regular flight
    // RegularFlight* r1 = ootumlia.addRegularFlight("09:00", "111");
    // RegularFlight* r2 = ootumlia.addRegularFlight("10:00", "222");
    // RegularFlight* r3 = ootumlia.addRegularFlight("11:00", "333");
    // RegularFlight* r4 = ootumlia.addRegularFlight("12:00", "444");
    // //Creating a specific flight
    // r1->addSpecificFlight("20250101");
    // r1->addSpecificFlight("20250102");
    // r1->addSpecificFlight("20250103");

    // r2->addSpecificFlight("20250101");
    // r2->addSpecificFlight("20250102");
    // r2->addSpecificFlight("20250103");

    // r3->addSpecificFlight("20250101");
    // r3->addSpecificFlight("20250102");
    // r3->addSpecificFlight("20250103");

    // //Listing all regular flights
    // ootumlia.listRegularFlights();  

    // std::string flightNumber;
    // std::cin >> flightNumber;   

    // //Searching for a flight
    // std::cout << ootumlia.findParticularRegular(flightNumber) << std::endl;


    // //Modifying attributes of a flight
    // std::cout << "修改後航班" << std::endl;
    // r1->setTime("10:00");
    // r1->setFlightNumber("112");

    // ootumlia.listRegularFlights();  
    //Searching for a flight
    // std::cout << ootumlia.findParticularRegular(flightNumber) << std::endl;

    //Creating a new person
    Person* p1 = new Person("John", "1234567890");
    EmployeeRole* er1 = new EmployeeRole(p1);
    
    p1->addRole(er1);
    p1->displayRoles();

    std::cout << "--------------------------------" << std::endl;
    Person* p2 = new Person("Nick", "0987654321");
    EmployeeRole* er2 = new EmployeeRole(p2);
    PassegerRole* pr2 = new PassegerRole(p2);
    p2->addRole(er2);
    p2->addRole(pr2);
    p2->displayRoles();




    return 0;
}