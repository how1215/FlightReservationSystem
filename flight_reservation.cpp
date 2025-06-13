#include <iostream>
#include <string>
#include <vector>

//Forward declaration
class Person;
class PersonRole;
class PassengerRole;
class EmployeeRole;
class Airline;
class RegularFlight;
class SpecificFlight;
class Booking;


class SpecificFlight{
        private:
            std::string date;
            RegularFlight* regularFlight;
            std::vector<Booking* > bookings;
            std::vector<EmployeeRole* > dutyMembers;
        public:
            SpecificFlight(const std::string& date,RegularFlight* regularFlight):date(date),regularFlight(regularFlight){}        

            std::string getDate() const { return date; }
            RegularFlight* getRegularFlight() const { return regularFlight; }

            //新增這特殊班次可訂位的座位
            void addBooking(Booking* booking);

            //新增會在Specific Flight上工作的機組人員
            void addDutyMember(EmployeeRole* person);
            //顯示這特殊班次可訂位的座位
            void listBookings();

            //顯示這特殊班次上的機組人員
            void listDutyMembers();
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
                }
            }
            std::cout << "No such flight" << std::endl;
            exit(1);
            return nullptr;
        }

        //新增crew member
        void addCrewMember(Person* person){
            crewMembers.push_back(person);
        }

        //顯示此航空公司所有crew member
        void listCrewMembers();
};

class PersonRole {
    private:
        Person* person;
    public:
        PersonRole(Person* person): person(person) {};
        virtual ~PersonRole() = default;
        Person* getPerson() const { return person; }
        void setPerson(Person* person){this->person=person;}
        virtual void displayRole() const = 0;  // 添加純虛擬函數
}; 

//兩個職位class都繼承自PersonRole
class PassengerRole: public PersonRole{
    private:
        std::vector<Booking* > bookings;
    public:
        PassengerRole(Person* person): PersonRole(person) {};

        void bookSeat(Booking* booking){
            bookings.push_back(booking);
        }
        //列出訂票紀錄
        void listAllReservations();
        
        // 實現 displayRole 函數
        void displayRole() const override {
            std::cout << "Passenger" << std::endl;
        }
};

class EmployeeRole: public PersonRole{
    private:
        std::string jobFunction;
        EmployeeRole* supervisor;
    public:
        EmployeeRole(Person* person, const std::string& jobFunction): PersonRole(person), jobFunction(jobFunction) {};
        
        //設定直屬上司
        void setSupervisor  (EmployeeRole* supervisor){
            if(this->supervisor == nullptr){
                this->supervisor = supervisor;
            }else{
                std::cout << "A person can have at most one supervisor" << std::endl;
                exit(1);
            }
        }
        std::string getJobFunction() const { return jobFunction; }

        //顯示直屬上司，實作於最後
        void displaySupervisor ();

        // 實現 displayRole 函數
        void displayRole() const override {
            std::cout << "Employee - " << jobFunction << std::endl;
        }
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

class Booking{
    private:
        std::string seatNumber;
        RegularFlight* regularFlight;
        SpecificFlight* specificFlight;

    public:
        Booking(const std::string& seatNumber): seatNumber(seatNumber) {};

        std::string getSeatNumber() const { return seatNumber; }
        void setSeatNumber(const std::string& seatNumber){this->seatNumber=seatNumber;}

        SpecificFlight* getSpecificFlight() const { return specificFlight; }
        RegularFlight* getRegularFlight() const { return regularFlight; }
        void setSpecificFlight(SpecificFlight* specificFlight){this->specificFlight=specificFlight;}
        void setRegularFlight(RegularFlight* regularFlight){this->regularFlight=regularFlight;}
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
//Airline的listCrewMembers實作
void Airline::listCrewMembers(){
    std::cout << "Crew members of " << name << ":" << std::endl;
    for (const auto& crewMember : crewMembers) {
        std::cout << "Name: " << crewMember->getName() << " /ID: " << crewMember->getIdNumber()<< std::endl;
    }
}

//PassengerRole的listBookings實作
void PassengerRole::listAllReservations(){
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Bookings of " << getPerson()->getName() << ":" << std::endl<< std::endl;
    for (const auto& booking : bookings) {
        std::cout << "Flight Number: " << booking->getRegularFlight()->getFlightNumber() << std::endl<< "Time: " << booking->getRegularFlight()->getTime() << std::endl;
        std::cout << "Seat Number: " << booking->getSeatNumber() << std::endl<< std::endl;
    }
    std::cout << "--------------------------------" << std::endl<< std::endl;
}
//SpecificFlight的listBookings實作
void SpecificFlight::listBookings(){
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Bookings of " << date << ":" << std::endl<< std::endl;
    for (const auto& booking : bookings) {
        std::cout << "Seat Number: " << booking->getSeatNumber() <<std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}
//SpecificFlight的addBooking實作
void SpecificFlight::addBooking(Booking* booking){
    bookings.push_back(booking);
    booking->setSpecificFlight(this);
    booking->setRegularFlight(regularFlight);
}
//SpecificFlight的addDutyMember實作
void SpecificFlight::addDutyMember(EmployeeRole* person){
    dutyMembers.push_back(person);
}
//SpecificFlight的listDutyMembers實作
void SpecificFlight::listDutyMembers(){
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Duty members of " << date << ":" << std::endl<< std::endl;
    for (const auto& dutyMember : dutyMembers) {
        std::cout << "Name: " << dutyMember->getPerson()->getName() << " /ID: " << dutyMember->getPerson()->getIdNumber()<< std::endl;
        std::cout << "Job Function: " << dutyMember->getJobFunction() << std::endl;
    }
    std::cout << "--------------------------------" << std::endl<< std::endl;
}
//主程式
int main(){

    // Airline ncku("NCKU Airlines");
    // //Creating a new regular flight
    // RegularFlight* r1 = ncku.addRegularFlight("09:00", "111");
    // RegularFlight* r2 = ncku.addRegularFlight("10:00", "222");
    // RegularFlight* r3 = ncku.addRegularFlight("11:00", "333");
    // //Creating a specific flight
    // SpecificFlight* s1 = r1->addSpecificFlight("20250101");
    // SpecificFlight* s2 = r1->addSpecificFlight("20250102");
    // SpecificFlight* s3 = r1->addSpecificFlight("20250103");

    // SpecificFlight* s4 = r2->addSpecificFlight("20250101");
    // SpecificFlight* s5 = r2->addSpecificFlight("20250102");
    // SpecificFlight* s6 = r2->addSpecificFlight("20250103");

    // SpecificFlight* s7 = r3->addSpecificFlight("20250101");
    // SpecificFlight* s8 = r3->addSpecificFlight("20250102");
    // SpecificFlight* s9 = r3->addSpecificFlight("20250103");

    // //Listing all regular flights
    // ncku.listRegularFlights();  

    // //Listing all specific flights
    // r1->listSpecificFlights();
    // r2->listSpecificFlights();
    // r3->listSpecificFlights();

    // //Modifying attributes of a flight
    // std::cout << "修改後航班" << std::endl;
    // r1->setTime("10:00");
    // r1->setFlightNumber("112");

    // ncku.listRegularFlights();  

    // std::string flightNumber;
    // std::cout << "請輸入航班號碼:";
    // std::cin >> flightNumber;   

    // //Searching for a flight
    // std::cout << "航班" << flightNumber << "的時間是: " << ncku.findParticularRegular(flightNumber)->getTime() << std::endl;

    // std::cout << "--------------------------------" << std::endl<< std::endl;

    // //Creating a new person (crew member)
    // Person* p1 = new Person("John", "1234567890");
    // EmployeeRole* er1 = new EmployeeRole(p1,"CEO");
    // p1->addRole(er1);
    // p1->displayRoles();
    // Person* p2 = new Person("Nick", "0987654321");
    // EmployeeRole* er2 = new EmployeeRole(p2,"Manager");
    // PassengerRole* pr2 = new PassengerRole(p2);
    // p2->addRole(er2);
    // p2->addRole(pr2);
    // p2->displayRoles();
    // Person* p3 = new Person("Tom", "1111111111");
    // EmployeeRole* er3 = new EmployeeRole(p3,"Pilot");
    // PassengerRole* pr3 = new PassengerRole(p3);

    // //Creating a new person (passenger)
    // Person* p4 = new Person("Curry", "3030303030");
    // PassengerRole* pr4 = new PassengerRole(p4);
    // p4->addRole(pr4);

    // er2->setSupervisor(er1);
    // er3->setSupervisor(er2);

    // p3->addRole(er3);
    // p3->addRole(pr3);
    // p3->displayRoles();

    // p1->displaySupervisor();
    // p2->displaySupervisor();
    // p3->displaySupervisor();

    // std::cout << "--------------------------------" << std::endl;

    // ncku.addCrewMember(p1);
    // ncku.addCrewMember(p2);
    // ncku.addCrewMember(p3);


    // ncku.listCrewMembers();

    //訂位流程
    //建立航空公司
    Airline ncku("NCKU Airlines");
    //建立regular flight
    RegularFlight* r1 = ncku.addRegularFlight("09:00", "111");
    RegularFlight* r2 = ncku.addRegularFlight("10:00", "222");
    //建立specific flight
    SpecificFlight* s1 = r1->addSpecificFlight("20250101");
    SpecificFlight* s2 = r2->addSpecificFlight("20250108");
    //建立specific flight機組人員
    Person* p1 = new Person("John", "1234567890");
    Person* p2 = new Person("Nick", "0987654321");
    Person* p3 = new Person("Tom", "1111111111");
    EmployeeRole* er1 = new EmployeeRole(p1,"Pilot");
    EmployeeRole* er2 = new EmployeeRole(p2,"Co-Pilot");
    EmployeeRole* er3 = new EmployeeRole(p3,"Flight Attendant");
    
    //設定值班人員
    s1->addDutyMember(er1);
    s1->addDutyMember(er2);
    s1->addDutyMember(er3);
    s2->addDutyMember(er1);
    s2->addDutyMember(er2);
    s2->addDutyMember(er3);
    //列出specific flight機組人員
    s1->listDutyMembers();
    s2->listDutyMembers();
    // //建立specific flight 的booking
    // Booking* b1 = new Booking("1A");
    // Booking* b2 = new Booking("1B");
    // Booking* b3 = new Booking("1C");

    // Booking* b4 = new Booking("1A");
    // Booking* b5 = new Booking("1B");
    // Booking* b6 = new Booking("1C");
    // //將booking加入specific flight
    // s1->addBooking(b1);
    // s1->addBooking(b2);
    // s1->addBooking(b3);
    // s2->addBooking(b4);
    // s2->addBooking(b5);
    // s2->addBooking(b6);
    // //列出specific flight可訂位的座位
    // s1->listBookings();
    // s2->listBookings();
    // //建立人物
    // Person* p1 = new Person("Nick", "1234567890");
    // //將人物加上乘客腳色
    // PassengerRole* pr1 = new PassengerRole(p1);
    // //乘客訂位
    // pr1->bookSeat(b1);    
    // pr1->bookSeat(b4);
    // pr1->bookSeat(b5);
    // pr1->bookSeat(b6);
    // //列出乘客訂票紀錄
    // pr1->listAllReservations();


    return 0;
}