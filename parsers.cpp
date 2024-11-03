#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

int node_num;
struct Customer {
    int dec_id;
    std::string id;
    std::string name;
    int max_input;
    int over_input_penalty;
    int late_delivery_penalty;
    int early_delivery_penalty;
    std::string node_type;
};
std::vector<Customer> customers;
void parse_customer_csv()
{
    std::ifstream file("/Users/dariadragomir/Documents/GitHub/Sorting-Algorithms-Analysis/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/customers.csv");

    std::string line;
    

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        Customer cust;


        std::getline(ss, cust.id, ';');
        std::getline(ss, cust.name, ';');
        std::getline(ss, token, ';');
        cust.max_input = std::stoi(token);
        std::getline(ss, token, ';');
        cust.over_input_penalty = std::stoi(token);
        std::getline(ss, token, ';');
        cust.late_delivery_penalty = std::stoi(token);
        std::getline(ss, token, ';');
        cust.early_delivery_penalty = std::stoi(token);
        std::getline(ss, cust.node_type, ';');

        cust.dec_id = node_num++;

        customers.push_back(cust);
    }


    file.close();
}

struct Refinery {
    std::string id;
    std::string name;
    int capacity;
    int max_output;
    int production;
    int overflow_penalty;
    int underflow_penalty;
    int over_output_penalty;
    double production_cost;
    double production_co2;
    int initial_stock;
    std::string node_type;
    int dec_id;
};

std::vector<Refinery> refineries;
void parse_refineries_csv()
{
    std::string line;

    std::ifstream file("/Users/dariadragomir/Documents/GitHub/Sorting-Algorithms-Analysis/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/refineries.csv");
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        Refinery refinery;


        std::getline(ss, refinery.id, ';');
        std::getline(ss, refinery.name, ';');
        
        std::getline(ss, token, ';');
        refinery.capacity = std::stoi(token);
        
        std::getline(ss, token, ';');
        refinery.max_output = std::stoi(token);
        
        std::getline(ss, token, ';');
        refinery.production = std::stoi(token);
        
        std::getline(ss, token, ';');
        refinery.overflow_penalty = std::stoi(token);
        
        std::getline(ss, token, ';');
        refinery.underflow_penalty = std::stoi(token);
        
        std::getline(ss, token, ';');
        refinery.over_output_penalty = std::stoi(token);
        
        std::getline(ss, token, ';');
        refinery.production_cost = std::stod(token);
        
        std::getline(ss, token, ';');
        refinery.production_co2 = std::stod(token);
        
        std::getline(ss, token, ';');
        refinery.initial_stock = std::stoi(token);
        
        std::getline(ss, refinery.node_type, ';');
        refinery.dec_id = node_num++;


        refineries.push_back(refinery);
    }


    file.close();

}


struct Tank {
    std::string id;
    std::string name;
    int capacity;
    int max_input;
    int max_output;
    int overflow_penalty;
    int underflow_penalty;
    int over_input_penalty;
    int over_output_penalty;
    int initial_stock;
    std::string node_type;
    int dec_id;
};

std::vector<Tank> tanks;
void parse_tanks_csv()
{
    std::ifstream file("/Users/dariadragomir/Documents/GitHub/Sorting-Algorithms-Analysis/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/tanks.csv");

    std::string line;

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        Tank tank;

        // Read each field and store it in the struct
        std::getline(ss, tank.id, ';');
        std::getline(ss, tank.name, ';');
        
        std::getline(ss, token, ';');
        tank.capacity = std::stoi(token);
        
        std::getline(ss, token, ';');
        tank.max_input = std::stoi(token);
        
        std::getline(ss, token, ';');
        tank.max_output = std::stoi(token);
        
        std::getline(ss, token, ';');
        tank.overflow_penalty = std::stoi(token);
        
        std::getline(ss, token, ';');
        tank.underflow_penalty = std::stoi(token);
        
        std::getline(ss, token, ';');
        tank.over_input_penalty = std::stoi(token);
        
        std::getline(ss, token, ';');
        tank.over_output_penalty = std::stoi(token);
        
        std::getline(ss, token, ';');
        tank.initial_stock = std::stoi(token);
        
        std::getline(ss, tank.node_type, ';');

        tank.dec_id = node_num++;

        tanks.push_back(tank);
    }

    file.close();

}


struct Connection {
    int dec_id;
    std::string from_id;
    std::string to_id;
    int distance;
    int lead_time_days;
    std::string connection_type;
    int max_capacity;
};

std::vector<Connection> connections;
void parse_connection_csv() {
    std::ifstream file("/Users/dariadragomir/Documents/GitHub/Sorting-Algorithms-Analysis/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/connections.csv");

    std::string line;

    std::getline(file, line);
    int lineNum = 1;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        Connection conn;

        std::getline(ss, token, ';'); // id - not used
        std::getline(ss, conn.from_id, ';');
        std::getline(ss, conn.to_id, ';');
        std::getline(ss, token, ';');
        conn.distance = std::stoi(token);
        std::getline(ss, token, ';');
        conn.lead_time_days = std::stoi(token);
        std::getline(ss, conn.connection_type, ';');
        std::getline(ss, token, ';');
        conn.max_capacity = std::stoi(token);
        conn.dec_id = lineNum++;

        connections.push_back(conn);
    }

    file.close();
}

struct Movement {
    std::string id;
    int amount;

    Movement() : id(""), amount(0) {}

    Movement(const std::string& id, int amount) : id(id), amount(amount) {}
};

std::string encodeMovements(const std::vector<Movement>& movements) {
    std::ostringstream oss;

    oss << movements.size() << " ";

    for (size_t i = 0; i < movements.size(); ++i) {
        oss << movements[i].id << "," << movements[i].amount;
        if (i < movements.size() - 1) {
            oss << "@"; // Add separator if not the last element
        }
    }

    return oss.str();
}