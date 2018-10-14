#include <memory>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <sstream>
#include <random>
#include <algorithm>
#include <functional>
#include <ifaddrs.h>
#include <coap/coap.h>
#include <coap/address.h>
#include "support.h"
#include "gateway.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <map>

std::pair<std::string, std::string> connect(const std::string& gateway_key)
{

    const auto log_level(coap_log_t::LOG_WARNING);
    coap_dtls_set_log_level(log_level);
    coap_set_log_level(log_level);

    if(0==coap_dtls_is_supported())
    {
        throw std::runtime_error("coap does not support dtls");
    }

    if (0 >= gateway_key.length())
    {
        throw std::runtime_error("invalid gateway key provided");
    }

    std::mutex mutex;
    std::unique_lock<std::mutex> unique_lock(mutex);
    std::condition_variable condition_variable;

    auto result(
        std::make_pair(
            generate_random_string(16),
            std::string()
        )
    );

    std::unique_ptr<
        coap_context_t,
        decltype(&coap_free_context)
    > ctx(
        coap_new_context(nullptr),
        coap_free_context
    );
    if(!ctx)
    {
        throw std::runtime_error("failed to create new context");
    }

    coap_address_t client_address;
    coap_address_init(&client_address);
    {
        sockaddr_in socket;
        socket.sin_family = AF_INET;
        auto local_address(get_local_address());
        if (0 == inet_aton(local_address.c_str(), &socket.sin_addr))
        {
            throw std::runtime_error("inet_aton call failed");
        }
        srand(time(NULL));
        auto port(49152 + (rand() % 16383));
        socket.sin_port         = (port & 0xFF00 >> 8) | (port & 0x00FF << 8);
        client_address.size     = sizeof(socket);
        client_address.addr.sin = socket;
    }

    auto client_endpoint(
        coap_new_endpoint(&client_address, COAP_ENDPOINT_DTLS)
    );
    if(!client_endpoint)
    {
        throw std::runtime_error("failed to create local endpoint");
    }

    coap_attach_endpoint(
        ctx.get(),
        client_endpoint
    );

    std::string init_username("Client_identity");
    auto keystore_item(
        coap_keystore_new_psk(
            NULL,
            0,
            &init_username.at(0),
            init_username.length(),
            const_cast<char*>(gateway_key.c_str()),
            gateway_key.length(),
            0
        )
    );
    if(!keystore_item)
    {
        throw std::runtime_error("failed to create new keystore item");
    }

    coap_keystore_store_item(
        ctx->keystore, 
        keystore_item, 
        NULL
    );

    coap_register_option(ctx.get(), COAP_OPTION_BLOCK2);

    coap_set_app_data(ctx.get(), &result.second);
    coap_register_response_handler(
        ctx.get(),
        [](
            struct coap_context_t *ctx,
            const coap_endpoint_t *client_endpoint,
            const coap_address_t *remote,
            coap_pdu_t *sent,
            coap_pdu_t *received,
            coap_tid_t id
        )
        {
            
            coap_show_pdu(sent);
            coap_show_pdu(received);

            std::string response(
                reinterpret_cast<char*>(
                    received->hdr
                ),
                received->length
            );
            
            std::cout << "The handler received message of " << received->length << " bytes\n"
                      << "  code    " << coap_response_phrase(received->hdr->code) << "\n"
                      << "  raw     " << response << std::endl;

            std::map<unsigned,std::string> options;
            {
                std::string payload(
                    std::find(response.begin(), response.end(), '{')+1,
                    std::find(response.begin(), response.end(), '}')
                );
                std::stringstream ss(payload);
                for(std::string option; std::getline(ss, option, ',');)
                {
                    auto seperator(std::find(option.begin(), option.end(), ':'));
                    options.emplace(
                        std::stoul(
                            std::string(
                                option.begin()+1, 
                                seperator-1
                            )
                        ),
                        std::string(
                            seperator+2,
                            option.end()-1
                        )
                    );
                }
            }

            std::for_each(
                options.begin(), options.end(),
                [](const decltype(options)::value_type& option)
                {
                    std::cout << "  " << std::left << std::setfill(' ') << std::setw(8) << option.first << option.second << '\n';
                }
            );

            auto iterator(options.find(9091));
            if (options.end() != iterator)
            {
                *static_cast<std::string*>(
                    coap_get_app_data(ctx)
                ) = iterator->second;
            }
            
        }
    );

    coap_address_t server_address;
    coap_address_init(&server_address);
    {
        sockaddr_in socket;
        socket.sin_family = AF_INET;
        if (0 == inet_aton("192.168.1.9", &socket.sin_addr))
        {
            throw std::runtime_error("inet_aton call failed");
        }
        socket.sin_port         = 0x3416; // Endianess! 0x1634==5684;
        server_address.size     = sizeof(socket);
        server_address.addr.sin = socket;
    }

    auto pdu(coap_new_pdu());
    
    pdu->hdr->type = COAP_MESSAGE_CON;
    pdu->hdr->id   = coap_new_message_id(ctx.get());
    pdu->hdr->code = COAP_REQUEST_POST;

    const std::string path0("15011");
    coap_add_option(
        pdu,
        COAP_OPTION_URI_PATH,
        path0.length(),
        reinterpret_cast<const unsigned char*>(path0.c_str())
    );

    const std::string path1("9063");
    coap_add_option(
        pdu,
        COAP_OPTION_URI_PATH,
        path1.length(),
        reinterpret_cast<const unsigned char*>(path1.c_str())
    );

    auto payload("{\"9090\":\""+result.first+"\"}");
    coap_add_data(
        pdu,
        payload.length(),
        reinterpret_cast<const unsigned char*>(payload.c_str())
    );
    
    auto message_id(
        coap_send_confirmed(
            ctx.get(),
            client_endpoint,
            &server_address,
            pdu
        )
    );
    if(COAP_INVALID_TID == message_id)
    {
        throw std::runtime_error("failed to send message");
    }

    while(result.second.empty())
    {
        auto response(
            coap_run_once(
                ctx.get(), 
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::seconds(5)
                ).count()
            )
        );
        if(0 > response)
        {
            throw std::runtime_error("failed to communicate with the gateway");
        }
    }

    return result;

}