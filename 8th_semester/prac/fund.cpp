#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QPainter>

#include <iostream>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

using namespace std;




int RandU(int nMin, int nMax)
    {
    return nMin + (int)((double)rand() / (RAND_MAX+1) * (nMax-nMin+1));
    }





class Investition;
class Stock;
class Deposit;



//class Conjuncture;

class Conjuncture : public std::enable_shared_from_this<Conjuncture>
    {
    std::map<std::string, double> market_cost;
    std::map<std::string, double> market_risks;
    std::map<std::string, double> market_dividents;


    public:
        std::vector<std::shared_ptr<Investition>> investment_list;

        Conjuncture(json config);

        void make_time_step();

        double get_market_cost(std::string asset_name);
    };







/* cost - текущая рыночная стоимость вложения */
class Investition
    {
    protected:
        double cost = 0;            //calculated due to market info
        double risk = 0;            //attached to an asset
        int investition_time = -1;  //when deposit should be closed

        int quantity = 0;

        std::shared_ptr<Conjuncture> conjuncture_ptr = nullptr;

    public:
        Investition( json & invest_json, std::shared_ptr<Conjuncture> conjuncture_ptr)
            {
            //cost = invest_json["COST"];

            risk = invest_json["RISK"];
            investition_time = invest_json["INVEST_TIME"];
            quantity = invest_json["QUANTITY"];

            conjuncture_ptr = conjuncture_ptr;
            }

        double get_cost()
            {
            return cost;
            }

        double get_risk()
            {
            return risk;
            }

        double get_investition_time()
            {
            return investition_time;
            }

        void set_investition_time(double var)
            {
            investition_time = var;
            }

        void set_cost(double var)
            {
            cost = var;
            }

        void set_risk(double var)
            {
            risk = var;
            }

        ~Investition()
            {

            }

        virtual void get_update_from_conjuncture() = 0;
    };


class Deposit : public Investition {

    private:
        std::string bank_name;
        double currency_value = 0; //will be updated by conjuncture if info is available
        std::string currency_name;

        //Bank percent
        double deposit_percent = 0;


    public:
        void get_update_from_conjuncture()
            {
            if( currency_name == "DOLLAR" )
                {
                if( conjuncture_ptr != nullptr )
                    {
                    currency_value = (*conjuncture_ptr).get_market_cost("DOLLAR");
                    }
                }
            }

        Deposit( json invest_json, std::shared_ptr<Conjuncture> conjuncture_ptr )  :
                Investition( invest_json, conjuncture_ptr )
            {
            bank_name = invest_json["BANK"];
            //currency_value = invest_json["BANK"];
            currency_name = invest_json["CURRENCY_NAME"];
            deposit_percent = invest_json["DEPOSIT_PERCENT"];
            }

        std::string get_bank_name()
            {
            return bank_name;
            }

        double get_deposit_percent()
            {
            return deposit_percent;
            }
        void set_deposit_percent(double var)
            {
            deposit_percent = var;
            }


        json close_deposit()
            {
            if(currency_name == "ROUBLE" )
                return json::array({"ROUBLE", quantity});

            else
                return json({currency_name, quantity * currency_value});
            }

        ~Deposit()
            {

            }


};



class Stock : public Investition {
    private:
        std::string company_name;

        //One market share cost
        double share_market_cost  = 0;
        double divident_percent  = 0;

    public:

        Stock(  json invest_json, std::shared_ptr<Conjuncture> conjuncture_ptr ) :
            Investition( invest_json, conjuncture_ptr )
            {
            company_name = invest_json["COMPANY_NAME"];
            //share_market_cost = invest_json["SHARE_COST"];
            divident_percent = invest_json["DIVIDEND_PERCENT"];
            }

        double get_share_market_cost()
            {
            return share_market_cost;
            }

        double set_share_market_cost(double var)
            {
            share_market_cost = var;
            }

        double get_divident_percent()
            {
            return divident_percent;
            }

        double set_divident_percent(double var)
            {
            divident_percent = var;
            }


        void get_update_from_conjuncture()
            {
            share_market_cost = (*conjuncture_ptr).get_market_cost("DOLLAR");
            }

};





class InvestFund
{
    private:
        double total_cost = 0; //Money on fund's accont + market cost of all property
        double money_on_account = 0; //Only free money

        std::vector<std::shared_ptr<Investition>> fund_briefcase;
        std::vector<double> briefcase_total_profits;


        double profit_tax_percent; //налог на прибыль

        std::shared_ptr<Conjuncture> conjuncture_ptr;

    public:
        InvestFund( nlohmann::json  invest_fund_config, std::shared_ptr<Conjuncture> conjuncture_ptr_passed)
            {
            conjuncture_ptr = conjuncture_ptr_passed;


            money_on_account = invest_fund_config["FUND_ACCOUNT"];
            profit_tax_percent = invest_fund_config["TAX"];

            for (auto el : invest_fund_config["BRIEFCASE"] )
                {
                if( el.at("ASSET") == "CURRENCY")
                    {
                    std::shared_ptr<Investition> i_ptr  = std::make_shared<Deposit> (  el, conjuncture_ptr );

                    fund_briefcase.push_back(i_ptr);
                    }
                else
                    {
                    std::shared_ptr<Investition> i_ptr  = std::make_shared<Stock> (  el, conjuncture_ptr );

                    fund_briefcase.push_back(i_ptr);
                    }
                }
            }


        void make_time_step()
            {
            //For all assets, call making time steps
            std::cout << "InvestFund::make_time_step" << std::endl;
            }

};





/* Adding properties */
Conjuncture::Conjuncture(json  config)
    {
    for (auto& el : config["MARKET"].items())
        {
        market_cost[el.key()] = el.value();
        }

    for (auto& el : config["RISK"].items())
        {
        market_risks[el.key()] = el.value();
        }

    for (auto& el : config["DIVIDENDS"].items())
        {
        market_dividents[el.key()] = el.value();
        }
    }

void Conjuncture::make_time_step()
    {
    std::cout << "Conjuncture::make_time_step" << std::endl;


    std::random_device dev;
    std::mt19937_64 rng(dev());

    std::uniform_int_distribution<size_t> idDist(0, market_cost.size() - 1);
    auto elementId= market_cost.begin();
    std::advance(elementId, idDist(rng));

    std::string key = (*elementId).first;

    //Creating new opportunities
    if(key == "DOLLAR")
        {
        std::shared_ptr<Investition> i_ptr = std::make_shared<Deposit>( json( { {"RISK", 30}, {"INVEST_TIME", 5}, {"QUANTITY", 100}, {"BANK", key}, {"CURRENCY_NAME" , "DOLLAR"}, {"DEPOSIT_PERCENT" , rand()} }  ) , shared_from_this() );
        investment_list.push_back(   i_ptr    );
        }
    else
        {
        std::shared_ptr<Investition> i_ptr = std::make_shared<Stock>( json( { {"RISK", 30}, {"INVEST_TIME" , 5}, {"QUANTITY", 100}, {"COMPANY_NAME", key},  {"DIVIDEND_PERCENT" , rand()} }  ) , shared_from_this() );
        investment_list.push_back(   i_ptr    );
        }

    //Fluctuating previous ones:

    }

double Conjuncture::get_market_cost(std::string asset_name)
    {
    return market_cost[asset_name];
    }





class Environment
    {
    unique_ptr<InvestFund> investfund_ptr = nullptr;
    shared_ptr<Conjuncture> conjuncture_ptr = nullptr;

    public:

        Environment(json  got_on_input)
            {
            conjuncture_ptr = std::make_shared<Conjuncture>(got_on_input);
            investfund_ptr = std::make_unique<InvestFund>(got_on_input,  conjuncture_ptr );
            }

        void step()
            {
            //Change the prices depending on the risk
            (*conjuncture_ptr).make_time_step();

            //Update the costs of all shares and currencies, calculate profit, tax, etc
            (*investfund_ptr).make_time_step();
            }

    };






