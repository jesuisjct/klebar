#include <cstdio>
#include <iostream>
#include "../cpp-httplib/httplib.h"

int main(void)
{
    printf("klebar command started\n");

    bool done = false;
    std::string input;
    
    httplib::Client client("localhost", 8080);

    while (!done) {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);

        if (input == "quit") 
        {   done = true;
            printf("quit\n"); 
        }
        else if (input == "do") 
        {   httplib::Params params;
            params.emplace("action", "mon action");
            params.emplace("param", "mon param");
            auto res = client.Post("/do", params);
            printf("post\n"); 
        }
        else if (input == "halt") 
        {   printf("halt\n");
            auto res = client.Get("/halt");
            // if (res) 
            // {   cout << res->status << endl;
            //     cout << res->get_header_value("Content-Type") << endl;
            //     cout << res->body << endl;
            // } 
        } 
        else if (input == "help") 
        {   std::cout
                << "\nCommand List:\n"
                << "help: Display this help text\n"
                << "halt: halt the server\n"
                << "quit: Exit the program\n"
                << std::endl;
        } else 
        {   std::cout << "> Unrecognized Command" << std::endl;
        }
    }

    printf("c'est fini\n\n");
}