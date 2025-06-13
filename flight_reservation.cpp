#include <iostream>
#include <string>
#include <vector>

//Forward declaration
class RegularFlight;
class Person;
class PersonRole;

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
        SpecificFlight* addSpecificFlight(const std::string& date){
            SpecificFlight* s = new SpecificFlight(date,this);
            //將新增的specific flight 儲存起來
            specificFlights.push_back(s);
            return s;
        }

        //顯示此regular flight所有specific flight
        void listSpecificFlights() const {
            std::cout << "Specific flights of " << flightNumber << ":" << std::endl;
            for (const auto& specificFlight : specificFlights) {
                std::cout << "Date: " << specificFlight->getDate() << std::endl;
            }
        }
};


class Airline {
    private:
        std::string name;
        std::vector<RegularFlight* > regularFlights;
        std::vector<Person* > crewMembers;
    public:
        Airline(const std::string& name): name(name) {};
        std::string getName() const { return name; }
        void setName(const std::string& name){this->name=name;}
        std::vector<RegularFlight* > getRegularFlights() const { return regularFlights; }
        std::vector<Person* > getCrewMembers() const { return crewMembers; }
        
        //新增Specific Flight
        RegularFlight* addRegularFlight(const std::string& time, const std::string& flightNumber){
            RegularFlight* r = new RegularFlight(time,flightNumber);
            //將新增的regular flight 儲存起來
            regularFlights.push_back(r);
            return r;
        }

        //顯示此航空公司所有regular flight
        void listRegularFlights() const {
            std::cout << "Regular flights of " << name << ":" << std::endl;
            for (const auto& regularFlight : regularFlights) {
                std::cout << "Flight Number: " << regularFlight->getFlightNumber() << " Time: " << regularFlight->getTime() << std::endl;
            }
        }

        //用航班號碼搜尋特定regular flight
        RegularFlight* findParticularRegular(const std::string& flightNumber) const {
            for (const auto& regularFlight : regularFlights) {
                if (regularFlight->getFlightNumber() == flightNumber) {
                    return regularFlight;
                }else{
                    std::cout << "No such flight" << std::endl;
                    exit(1);
                }
            }
            return nullptr;
        }

        //新增crew member
        void addCrewMember(Person* person){
            crewMembers.push_back(person);
        }

        //顯示此航空公司所有crew member
        void listCrewMembers() const {
            std::cout << "Crew members of " << name << ":" << std::endl;
            for (const auto& crewMember : crewMembers) {
                std::cout << "Name: " << crewMember->getName() << " ID: " << crewMember->getIdNumber() << std::endl;
            }
        }
};

class PersonRole {
    private:
        Person* person;
    public:
        PersonRole(Person* person): person(person) {};
        virtual ~PersonRole() = default;
        Person* getPerson() const { return person; }
        void setPerson(Person* person){this->person=person;}
        //純虛函數供子類別繼承後override
        virtual void displayRole() = 0;

}; 

//兩個職位class都繼承自PersonRole
class PassengerRole: public PersonRole{
    public:
        PassengerRole(Person* person): PersonRole(person) {};
        void displayRole(){
            std::cout << "Passenger" << std::endl;
        }
};

class EmployeeRole: public PersonRole{
    private:
        std::string jobFunction;
        EmployeeRole* supervisor;
    public:
        EmployeeRole(Person* person, const std::string& jobFunction): PersonRole(person), jobFunction(jobFunction) {};
        void displayRole(){
            std::cout << "Employee" << std::endl;
        }
        
        //設定直屬上司
        void setSupervisor  (EmployeeRole* supervisor){
            if(this->supervisor == nullptr){
                this->supervisor = supervisor;
            }else{
                std::cout << "A person can have at most one supervisor" << std::endl;
                exit(1);
            }
        }

        //顯示直屬上司，實作於最後
        void displaySupervisor ();                                                                                                                              
};

class Person {
    private:
        std::string name;
        std::string idNumber;
        //一個人最多可以有兩個職位
        std::vector<PersonRole* > roles;

    public:
        Person(const std::string& name, const std::string& idNumber): name(name), idNumber(idNumber) {};

        std::string getName() const { return name; }
        std::string getIdNumber() const { return idNumber; }

        void addRole(PersonRole* role){
            //如果已經有兩個職位，則不能新增
            if(roles.size() >= 2){
                std::cout << "A person can have at most two roles" << std::endl;
                exit(1);
            }
            if (roles.size() == 1) {
                // 當已有一個角色時，檢查是否與新角色為相同類別
                if (typeid(*role) == typeid(*roles.front())) {
                    std::cout << "A person cannot have duplicate role types." << std::endl;
                    exit(1);
                }
            }
                roles.push_back(role);
        }

        //顯示這位使用者的所有職位
        void displayRoles() const {
            std::cout << name << " (" << idNumber << ") has roles :" << std::endl;
            for (const auto& role : roles) {
                role->displayRole();
            }
        }

        //顯示直屬上司
        void displaySupervisor(){
            std::cout<<name<<" has supervisor:"<<std::endl;
            for (auto& role : roles) {
                EmployeeRole* employeeRole = dynamic_cast<EmployeeRole*>(role);
                if(employeeRole != nullptr){
                    employeeRole->displaySupervisor();
                }
            }
        }
};

// EmployeeRole的displaySupervisor實作
void EmployeeRole::displaySupervisor(){
    if(supervisor == nullptr){
        std::cout << "No supervisor" << std::endl<< std::endl;
    }else{
        Person* supervisorPerson = supervisor->getPerson();
        std::cout << "Supervisor: " << supervisorPerson->getName() << std::endl<< std::endl;
    }
}
int main(){

    Airline ncku("NCKU Airlines");
    //Creating a new regular flight
    RegularFlight* r1 = ncku.addRegularFlight("09:00", "111");
    RegularFlight* r2 = ncku.addRegularFlight("10:00", "222");
    RegularFlight* r3 = ncku.addRegularFlight("11:00", "333");
    //Creating a specific flight
    SpecificFlight* s1 = r1->addSpecificFlight("20250101");
    SpecificFlight* s2 = r1->addSpecificFlight("20250102");
    SpecificFlight* s3 = r1->addSpecificFlight("20250103");

    SpecificFlight* s4 = r2->addSpecificFlight("20250101");
    SpecificFlight* s5 = r2->addSpecificFlight("20250102");
    SpecificFlight* s6 = r2->addSpecificFlight("20250103");

    SpecificFlight* s7 = r3->addSpecificFlight("20250101");
    SpecificFlight* s8 = r3->addSpecificFlight("20250102");
    SpecificFlight* s9 = r3->addSpecificFlight("20250103");

    //Listing all regular flights
    ncku.listRegularFlights();  

    //Listing all specific flights
    r1->listSpecificFlights();
    r2->listSpecificFlights();
    r3->listSpecificFlights();

    //Modifying attributes of a flight
    std::cout << "修改後航班" << std::endl;
    r1->setTime("10:00");
    r1->setFlightNumber("112");

    ncku.listRegularFlights();  

    std::string flightNumber;
    std::cout << "請輸入航班號碼:";
    std::cin >> flightNumber;   

    //Searching for a flight
    std::cout << "航班" << flightNumber << "的時間是: " << ncku.findParticularRegular(flightNumber)->getTime() << std::endl;

    std::cout << "--------------------------------" << std::endl;

    //Creating a new person (crew member)
    Person* p1 = new Person("John", "1234567890");
    EmployeeRole* er1 = new EmployeeRole(p1,"CEO");
    p1->addRole(er1);
    p1->displayRoles();
    Person* p2 = new Person("Nick", "0987654321");
    EmployeeRole* er2 = new EmployeeRole(p2,"Manager");
    PassengerRole* pr2 = new PassengerRole(p2);
    p2->addRole(er2);
    p2->addRole(pr2);
    p2->displayRoles();
    Person* p3 = new Person("Tom", "1111111111");
    EmployeeRole* er3 = new EmployeeRole(p3,"Pilot");
    PassengerRole* pr3 = new PassengerRole(p3);

    //Creating a new person (passenger)
    Person* p4 = new Person("Curry", "3030303030");
    PassengerRole* pr4 = new PassengerRole(p4);
    p4->addRole(pr4);

    er2->setSupervisor(er1);
    er3->setSupervisor(er2);

    p3->addRole(er3);
    p3->addRole(pr3);
    p3->displayRoles();

    p1->displaySupervisor();
    p2->displaySupervisor();
    p3->displaySupervisor();

    std::cout << "--------------------------------" << std::endl;

    ncku.addCrewMember(p1);
    ncku.addCrewMember(p2);
    ncku.addCrewMember(p3);


    ncku.listCrewMembers();


    return 0;
}