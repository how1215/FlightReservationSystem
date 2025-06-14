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
        PersonRole() = default;
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
        PassengerRole() = default;

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
        EmployeeRole(const std::string& jobFunction): jobFunction(jobFunction), supervisor(nullptr) {};
        
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
                PersonRole* existingRole = roles.front();
                if (typeid(*role) == typeid(*existingRole)) {
                    std::cout << "A person cannot have duplicate role types." << std::endl;
                    exit(1);
                }
            }
                roles.push_back(role);
                role->setPerson(this);
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
    std::cout << "Duty members of flight " << regularFlight->getFlightNumber() << " on " << date << ":" << std::endl<< std::endl;
    for (const auto& dutyMember : dutyMembers) {
        std::cout << "Name: " << dutyMember->getPerson()->getName() << " / ID: " << dutyMember->getPerson()->getIdNumber()<< std::endl;
        std::cout << "Job Function: " << dutyMember->getJobFunction() << std::endl;
    }
    std::cout << "--------------------------------" << std::endl<< std::endl;
}
//主程式
int main(){
    //建立人物
    Person* p1 = new Person("John", "1234567890");
    Person* p2 = new Person("Manny", "0987654321");
    Person* p3 = new Person("Tom", "1111111111");
    Person* p4 = new Person("Nick", "2222222222");
    //建立EmployeeRole
    EmployeeRole* er1 = new EmployeeRole("Pilot");
    EmployeeRole* er2 = new EmployeeRole("Co-Pilot");
    EmployeeRole* er3 = new EmployeeRole("Flight Attendant");
    //建立EmployeeRole的supervisor
    er2->setSupervisor(er1);
    er3->setSupervisor(er1);
    //建立PassengerRole
    PassengerRole* pr1 = new PassengerRole();
    PassengerRole* pr2 = new PassengerRole(); 
    //將人物賦予role
    p1->addRole(er1);
    p2->addRole(er2);
    p3->addRole(er3);
    p3->addRole(pr1);
    p4->addRole(pr2);
    //列出人物的role
    std::cout << "--------------------------------" << std::endl;
    p1->displayRoles();
    p2->displayRoles();
    p3->displayRoles();
    p4->displayRoles();
    std::cout << "--------------------------------" << std::endl;
    //列出人物的supervisor
    std::cout << "--------------------------------" << std::endl;
    p1->displaySupervisor();
    p2->displaySupervisor();
    p3->displaySupervisor();
    std::cout << "--------------------------------" << std::endl;
    //建立航空公司
    Airline ncku("NCKU Airlines");
    //新增航空公司職員
    ncku.addCrewMember(p1);
    ncku.addCrewMember(p2);
    ncku.addCrewMember(p3);
    //列出航空公司職員
    std::cout << "--------------------------------" << std::endl;
    ncku.listCrewMembers();
    std::cout << "--------------------------------" << std::endl;
    //建立regular flight
    RegularFlight* r1 = ncku.addRegularFlight("09:00", "111");
    //建立specific flight
    SpecificFlight* s1 = r1->addSpecificFlight("20250101");
    //建立booking
    Booking* b1 = new Booking("1A");
    Booking* b2 = new Booking("1B");
    Booking* b3 = new Booking("1C");
    //將booking加入specific flight
    s1->addBooking(b1);
    s1->addBooking(b2);
    s1->addBooking(b3);

    //將機組人員加入specific flight
    s1->addDutyMember(er1);
    s1->addDutyMember(er2);
    s1->addDutyMember(er3);
    
    //列出specific flight的機組人員
    s1->listDutyMembers();
    return 0;
}