#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <sstream>

struct Demand {
    std::string customer_id;
    int quantity;
    int post_day;
    int start_delivery_day;
    int end_delivery_day;
};
std::vector<Demand> demands;

std::vector<Demand> parseDemands(const std::string& encoded) {
    std::vector<Demand> demands;

    std::istringstream iss(encoded);
    int num_demands;
    std::string demand_data;

    if (!(iss >> num_demands)) {
        std::cerr << "Error reading number of demands." << std::endl;
        return demands; 
    }

    iss.ignore(1);

    std::getline(iss, demand_data);

    // Split the demand data using '@' as the delimiter
    std::istringstream demand_stream(demand_data);
    std::string demand_entry;

    while (std::getline(demand_stream, demand_entry, '@')) {
        // Trim leading and trailing spaces from demand_entry
        size_t start = demand_entry.find_first_not_of(" ");
        size_t end = demand_entry.find_last_not_of(" ");
        if (start == std::string::npos || end == std::string::npos) continue; // Skip empty entries
        demand_entry = demand_entry.substr(start, end - start + 1);

        // Split by ',' to get the individual fields
        std::istringstream field_stream(demand_entry);
        std::string field;

        Demand demand;
        int field_index = 0;
        while (std::getline(field_stream, field, ',')) {
            switch (field_index) {
                case 0:
                    demand.customer_id = field; // CUST_ID
                    break;
                case 1:
                    demand.quantity = std::stoi(field); // AMOUNT
                    break;
                case 2:
                    demand.post_day = std::stoi(field); // POST_DAY
                    break;
                case 3:
                    demand.start_delivery_day = std::stoi(field); // START_DAY
                    break;
                case 4:
                    demand.end_delivery_day = std::stoi(field); // END_DAY
                    break;
                default:
                    break; 
            }
            ++field_index;
        }

        demands.push_back(demand);
    }

    return demands;
}
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    const int PORT = 12345;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::cout << "New connection accepted." << std::endl;

        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = recv(new_socket, buffer, 4096, 0); 

        if (bytes == -1) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            close(new_socket);
            break;
        }
        buffer[bytes] = '\0';

        if(strncmp(buffer, "move", 4) == 0)
        {
            //Run algo get moves
        }
        else if(strncmp(buffer, "quit", 4) == 0)
        {
            close(new_socket);
            break;
        }
        else
        {
            demands = parseDemands(buffer);
        }

    }

    return 0;
}

