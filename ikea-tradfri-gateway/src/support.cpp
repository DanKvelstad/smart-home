#include <memory>
#include <iostream>
#include <ifaddrs.h>
#include "support.h"
#include <random>
#include <algorithm>
#include <functional>
#include <cstring>
#include <sstream>

std::string get_local_address()
{
    const bool verbose(false);
    std::string local_address;
    ifaddrs* ifaddr_ptr;
    if(0 == getifaddrs(&ifaddr_ptr))
    {
        std::unique_ptr<ifaddrs, void(*)(ifaddrs*)> ifaddr_head(
            ifaddr_ptr,
            [](ifaddrs* ifaddr)
            {
                freeifaddrs(ifaddr);
            }
        );
        for(auto ifa(ifaddr_ptr); NULL!=ifa; ifa = ifa->ifa_next)
        {
            if(NULL == ifa->ifa_addr)
            {
                if(verbose)
                {
                    std::cout << "found a local interface named " << ifa->ifa_name << "\n"
                              << "  it cannot be evaluated because ifa_addr is null\n";
                }
                continue;
            }
            auto family(ifa->ifa_addr->sa_family);
            std::vector<unsigned> address;
            for(unsigned i=0; i < 13; i++)
            {
                address.push_back(
                    static_cast<unsigned>(
                        static_cast<unsigned char>(
                            ifa->ifa_addr->sa_data[i]
                        )
                    )
                );
            }
            if(verbose)
            {
                std::cout << "found a local interface named " << ifa->ifa_name << "\n"
                          << "  of type " << family << "\n"
                          << "  with address " << address.at(0);
                for(uint i=1; i < 13; i++)
                {
                    std::cout << ":" << address.at(i);
                }
                std::cout << "\n";
            }
            
            if(0==strcmp("tun0", ifa->ifa_name) && AF_INET == family)
            {
                std::stringstream ss;
                ss << address.at(2) << '.' 
                   << address.at(3) << '.' 
                   << address.at(4) << '.'
                   << address.at(5);
                local_address = ss.str();
            }
        }
        if(local_address.empty())
        {
            throw std::runtime_error("failed to find local interfaces");
        }
        return local_address;
    }
    else
    {
        throw std::runtime_error("failed to list local interfaces");
    };
}

std::string generate_random_string(uint length)
{
    std::string string(length, '\0');
    std::random_device random_device;
    std::uniform_int_distribution<char> distribution('a', 'z');
    std::generate(
        string.begin(), string.end(),
        [&]()
        {
            return distribution(random_device);
        }
    );
    return string;
}
