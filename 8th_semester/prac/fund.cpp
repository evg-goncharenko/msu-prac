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


#define BEAR_DECAY 0.55
#define BULL_DECAY 0.95



int RandU(int nMin, int nMax)
    {
    return nMin + (int)( (double)rand() / (RAND_MAX ) * (nMax-nMin+1));
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

    char trend = 'B';

    public:
        std::vector<std::shared_ptr<Investition>> investment_list;

        Conjuncture(json config, char trend_passed = 'B');

        void make_time_step();

        double get_market_cost(std::string asset_name);


        ~Conjuncture()
            {
            std::cout << "ALARM" << std::endl;
            }
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
        Investition( json & invest_json, std::shared_ptr<Conjuncture> conjuncture_ptr_passed)
            {
            //cost = invest_json["COST"];

            try
                {
                risk = invest_json.at("RISK");
                }
            catch(...)
                {
                //Asset can not have the RISK bound to this asset
                }

            investition_time = invest_json["INVEST_TIME"];
            quantity = invest_json["QUANTITY"];


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

        virtual std::string get_name() = 0;

        virtual double extract_profit() = 0;

    };


class Deposit : public Investition {

    private:
        std::string bank_name;
        double currency_value = 0; //will be updated by conjuncture if info is available
        std::string currency_name;

        //Bank percent
        double deposit_percent = 0;


    public:

        Deposit( json invest_json, std::shared_ptr<Conjuncture> conjuncture_ptr_passed )  :
                Investition( invest_json, conjuncture_ptr_passed )
            {
            conjuncture_ptr = conjuncture_ptr_passed;


            bank_name = invest_json["BANK"];
            //currency_value = invest_json["BANK"];
            currency_name = invest_json["CURRENCY_NAME"];
            deposit_percent = invest_json["DEPOSIT_PERCENT"];
            }

        std::string get_name()
            {
            return bank_name + currency_name;
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


        double extract_profit()
            {
            std::cout << get_name() <<  " EXTRACTED : " << deposit_percent * cost << std::endl;

            return deposit_percent * cost;
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


        void get_update_from_conjuncture()
            {
            if( currency_name != "ROUBLE" )
                {
                if( conjuncture_ptr != nullptr )
                    {
                    currency_value = conjuncture_ptr->get_market_cost("DOLLAR");
                    }

                cost = quantity * currency_value;
                }
            }


};



class Stock : public Investition {
    private:
        std::string company_name;

        //One market share cost
        double share_market_cost  = 0;
        double divident_percent  = 0;

    public:

        Stock(  json invest_json, std::shared_ptr<Conjuncture> conjuncture_ptr_passed ) :
            Investition( invest_json, conjuncture_ptr_passed )
            {
            conjuncture_ptr = conjuncture_ptr_passed;


            company_name = invest_json["COMPANY_NAME"];
            //share_market_cost = invest_json["SHARE_COST"];
            divident_percent = invest_json["DIVIDEND_PERCENT"];
            }

        std::string get_name()
            {
            return company_name;
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

        double extract_profit()
            {
            std::cout << get_name() <<  "COST : " << cost << std::endl;
            std::cout << get_name() <<  "EXTRACTED : " << divident_percent * cost << std::endl;
            return divident_percent * cost;
            }

        void get_update_from_conjuncture()
            {
            share_market_cost = conjuncture_ptr->get_market_cost(company_name);

            cost = quantity * share_market_cost;
            }

};





class InvestFund
{
    private:
        double total_cost = 0; //Money on fund's accont + market cost of all property
        double money_on_account = 0; //Only free money

        std::vector<std::shared_ptr<Investition>> fund_briefcase;
        std::vector<double> briefcase_total_profits;

        std::vector<double> initial_costs;

        double profit_tax_percent; //налог на прибыль

        std::shared_ptr<Conjuncture> conjuncture_ptr = nullptr;

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

                //Initialize with zeros
                briefcase_total_profits.push_back(0);
                }

            make_time_step();

            //Initial vector of costs
            initial_costs = get_costs();
            }

        void make_time_step()
            {
            //For all assets, call making time steps
            std::cout << "InvestFund::make_time_step  BRIEFCASE SIZE = " <<  fund_briefcase.size() << std::endl;
            //Updating all assets
            for(int i = 0; i < fund_briefcase.size(); i++)
                {
                (*fund_briefcase[i]).get_update_from_conjuncture();
                }

            for(int i = 0; i < fund_briefcase.size(); i++)
                {
                briefcase_total_profits[i] += (*fund_briefcase[i]).extract_profit();
                }

            }

        std::vector<std::string> get_names()
            {
            std::vector<std::string> ret_vector;

            for(int i = 0; i < fund_briefcase.size(); i++)
                {
                ret_vector.push_back( (*fund_briefcase[i]).get_name() );
                }

            return ret_vector;
            }

        int get_q_briefcase()
            {
            return fund_briefcase.size();
            }

        std::vector<double> get_costs()
            {
            std::vector<double> ret_vector;

            for(int i = 0; i < fund_briefcase.size(); i++)
                {
                ret_vector.push_back( (*fund_briefcase[i]).get_cost() );
                }

            return ret_vector;
            }

        std::vector<double> get_profits()
            {
            return briefcase_total_profits;
            }

        std::vector<double> get_initial_costs()
            {
            return initial_costs;
            }

        std::vector<double> get_won_by_buying()
            {
            std::vector<double> costs = get_costs();

            std::vector<double> ret_vector;
            for(int i = 0; i < fund_briefcase.size(); i++)
                {
                ret_vector.push_back( costs[i] - initial_costs[i] + briefcase_total_profits[i] );
                }

            return ret_vector;
            }

};





/* Adding properties */
Conjuncture::Conjuncture(json  config, char trend_passed)
    {
    trend = trend_passed;

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
    /*if(key == "DOLLAR")
        {
        std::shared_ptr<Investition> i_ptr = std::make_shared<Deposit>( json( { {"RISK", 30}, {"INVEST_TIME", 5}, {"QUANTITY", 100}, {"BANK", key}, {"CURRENCY_NAME" , "DOLLAR"}, {"DEPOSIT_PERCENT" , rand()} }  ) , shared_from_this() );
        investment_list.push_back(   i_ptr    );
        }
    else
        {
        std::shared_ptr<Investition> i_ptr = std::make_shared<Stock>( json( { {"RISK", 30}, {"INVEST_TIME" , 5}, {"QUANTITY", 100}, {"COMPANY_NAME", key},  {"DIVIDEND_PERCENT" , rand()} }  ) , shared_from_this() );
        investment_list.push_back(   i_ptr    );
        }*/


    /*
    std::map<std::string, double> market_cost;
    std::map<std::string, double> market_risks;
    std::map<std::string, double> market_dividents;
     */

    //Risk of devalvation
        for (auto const& x : market_cost)
            {
            std::string key = x.first;

            std::cout << "KEY = " << key << std::endl;

            if( market_risks.find(key) != market_risks.end() )
                {
                int randU = RandU(0, 100);

                std::cout << "RandU " << randU << " | " << market_risks.at(key)  << std::endl;

                //In PERCENTS
                if( randU < market_risks.at(key) )
                    {
                    if(trend == 'B')
                        market_cost[key] = market_cost[key]  * BEAR_DECAY;
                    else if (trend == 'M')
                       market_cost[key] = market_cost[key] * BULL_DECAY;

                    std::cout << "DECAY" << std::endl;
                    }
                }

            }
    }

double Conjuncture::get_market_cost(std::string asset_name)
    {
    //return market_cost[asset_name];

    if( market_cost.find(asset_name) != market_cost.end() )
        {
        return market_cost[asset_name];
        }
    else
        {
        return 0;
        }


    }





class Environment
    {
    unique_ptr<InvestFund> investfund_ptr = nullptr;
    shared_ptr<Conjuncture> conjuncture_ptr = nullptr;

    public:

        Environment(json  got_on_input, char trend)
            {
            conjuncture_ptr = std::make_shared<Conjuncture>(got_on_input, trend);
            investfund_ptr = std::make_unique<InvestFund>(got_on_input,  conjuncture_ptr );
            }

        void step()
            {
            //Change the prices depending on the risk
            (*conjuncture_ptr).make_time_step();

            //Update the costs of all shares and currencies, calculate profit, tax, etc
            (*investfund_ptr).make_time_step();
            }


        std::vector<std::string> get_names()
            {
            return (*investfund_ptr).get_names();
            }

        int get_q_briefcase()
            {
            return (*investfund_ptr).get_q_briefcase();
            }


        std::vector<double> get_costs()
            {
            return (*investfund_ptr).get_costs();
            }


        std::vector<double> get_profits()
            {
            return (*investfund_ptr).get_profits();
            }

        std::vector<double> get_initial_costs()
            {
            return (*investfund_ptr).get_initial_costs();
            }

        std::vector<double> get_won_by_buying()
            {
            return (*investfund_ptr).get_won_by_buying();
            }


    };






