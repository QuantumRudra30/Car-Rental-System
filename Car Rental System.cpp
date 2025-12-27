#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Car {
    int id;
    string model;
    float price;
    bool rented;

public:
    Car(int i, string m, float p, bool r=false) {
        id=i; model=m; price=p; rented=r;
    }

    int getId() { return id; }
    bool isRented() { return rented; }

    void rent() { rented = true; }
    void giveBack() { rented = false; }

    void display() {
        cout << id << "\t" << model << "\t" << price << "\t";
        cout << (rented ? "Rented" : "Available") << endl;
    }

    string toFile() {
        return to_string(id) + "|" + model + "|" + to_string(price) + "|" + to_string(rented);
    }

    static Car fromFile(string line) {
        int p1=line.find('|');
        int p2=line.find('|',p1+1);
        int p3=line.find('|',p2+1);

        int id = stoi(line.substr(0,p1));
        string model = line.substr(p1+1,p2-p1-1);
        float price = stof(line.substr(p2+1,p3-p2-1));
        bool rent = stoi(line.substr(p3+1));

        return Car(id,model,price,rent);
    }
};

class Rental {
    vector<Car> cars;

public:
    void load() {
        ifstream f("cars.txt");
        string line;
        while(getline(f,line))
            cars.push_back(Car::fromFile(line));
        f.close();
    }

    void save() {
        ofstream f("cars.txt");
        for(auto &c:cars)
            f << c.toFile() << endl;
        f.close();
    }

    void addCar() {
        int id; string m; float p;
        cout<<"Car ID: "; cin>>id;
        cin.ignore();
        cout<<"Model: "; getline(cin,m);
        cout<<"Price per day: "; cin>>p;

        cars.push_back(Car(id,m,p));
        save();
        cout<<"Car added.\n";
    }

    void showCars() {
        cout<<"ID\tModel\tPrice\tStatus\n";
        for(auto &c:cars)
            c.display();
    }

    void rentCar() {
        int id;
        cout<<"Enter Car ID: "; cin>>id;
        for(auto &c:cars) {
            if(c.getId()==id && !c.isRented()) {
                c.rent();
                save();
                cout<<"Car rented.\n";
                return;
            }
        }
        cout<<"Car not available.\n";
    }

    void returnCar() {
        int id;
        cout<<"Enter Car ID: "; cin>>id;
        for(auto &c:cars) {
            if(c.getId()==id && c.isRented()) {
                c.giveBack();
                save();
                cout<<"Car returned.\n";
                return;
            }
        }
        cout<<"Invalid car.\n";
    }
};

int main() {
    Rental r;
    r.load();

    int ch;
    do {
        cout<<"\n1.Add 2.Show 3.Rent 4.Return 0.Exit\n";
        cin>>ch;

        if(ch==1) r.addCar();
        else if(ch==2) r.showCars();
        else if(ch==3) r.rentCar();
        else if(ch==4) r.returnCar();
    } while(ch!=0);

    return 0;
}
