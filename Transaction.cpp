#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;


struct Time{
    int year;
    int month;
    int day;

    Time(){
        this->year = 0;
        this->month = 0;
        this->day = 0;
    }

    void display(){
        cout << "year: " << year << endl;
        cout << "month: " << month << endl;
        cout << "day: " << day << endl;
    }

    string get_data_as_string() const{
        return (to_string(year) + to_string(month) + to_string(day));
    }

    explicit Time(const string& field){ // bc stringstream returns a string, so we do the work of separate the values here.
        stringstream sso(field);
        string year_;
        string month_;
        string day_;
        getline(sso, year_, '-');
        getline(sso, month_, '-');
        getline(sso, day_); // delimiter by default is '\n' newline
        this->year = stoi(year_);
        this->month = stoi(month_);
        this->day = stoi(day_);
    }
};

struct Transaction{ 
    Time date; //opening time (year, month, day) //bc we store data by day
    float open; // price in open time
    float high; // highest price in the period (in that day in binance) 
    float lowest; //lowest price in the period (in that day in binance)
    float close; // closing price
    float volume; // number of units traded in the market in that period (in that day in binance)
    double close_time; // closing time [large number formatted]
    double quote_asset_volume; // en cristiano, 2 monedas se cotizan (una contra la otra), en este caso bitcoin contra USTD (stable cryptocurrency based on the US dollar), el resultado es el volumen de este segundo (USTD)... creo...
    int number_of_trades; // number of trades :)
    double taker_buy_base_asset_volume; // this quantity is expressed as the amount of coins that were received.
    double taker_buy_quote_asset_volume; // this quantity is expressed as the amount of the second part of the pair (USTD) that were paid.
    // float ignore; // ignore this field :)

    Transaction(){ // onnly for genesis
        Time date_;
        date = date_;
        open = 0; 
        high = 0; 
        lowest = 0; 
        close = 0; 
        volume = 0; 
        close_time = 0; 
        quote_asset_volume = 0; 
        number_of_trades = 0; 
        taker_buy_base_asset_volume = 0;
        taker_buy_quote_asset_volume = 0;
    }

    string get_data_as_string() const{
        return (date.get_data_as_string() + to_string(open) + to_string(high) + to_string(lowest) + to_string(close) + to_string(volume) + to_string(close_time) + to_string(quote_asset_volume) + to_string(number_of_trades) + to_string(taker_buy_base_asset_volume) + to_string(taker_buy_quote_asset_volume));
    }

    void display(){
        date.display();
        cout << "open: " << open << endl;
        cout << "high: " << high << endl;
        cout << "lowest: " << lowest << endl;
        cout << "close: " << close << endl;
        cout << "volume: " << volume << endl;
        cout << "close_time: " << close_time << endl;
        cout << "quote_asset_volume: " << quote_asset_volume << endl;
        cout << "number_of_trades: " << number_of_trades << endl;
        cout << "taker_buy_base_asset_volume: " << taker_buy_base_asset_volume << endl;
        cout << "taker_buy_quote_asset_volume: " << taker_buy_quote_asset_volume << endl; 
        cout << endl;
    }

};
