#include <signal.h>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <thread>
#include <coap/coap.h>

int main_cpp(const std::vector<std::string>& args)
{

    //coap_startup();

    coap_address_t dst;
    if (resolve_address("192.168.1.4", "5683", &dst) < 0) {
        std::cout << "failed to resolve address\n";
        return EXIT_FAILURE;
    }

    auto ctx(coap_dtls_new_context(nullptr));
    if(!ctx)
    {
        std::cout << "failed to create new context\n";
        return EXIT_FAILURE;
    }

    auto session(coap_dtls_new_session(ctx, nullptr, &dst));
    if (!session)
     {
        std::cout << "failed to create new client session\n";
        return EXIT_FAILURE;
    }

    coap_register_response_handler(
        ctx,
        [](auto, auto, auto, coap_pdu_t *received, auto)
        {
            coap_show_pdu(LOG_INFO, received);
        }
    );
    
    auto pdu(
        coap_pdu_init(
            COAP_MESSAGE_CON,
            COAP_REQUEST_GET,
            0 /* message id */,
            coap_session_max_pdu_size(session)
        )
    );
    if (!pdu) 
    {
        std::cout << "failed to init PDU\n";
        return EXIT_FAILURE;
    }

    coap_add_option(
        pdu,
        COAP_OPTION_URI_PATH,
        5,
        reinterpret_cast<const uint8_t *>("hello")
    );

    coap_send(session, pdu);

    coap_run_once(ctx, 0);

    coap_session_release(session);
    coap_free_context(ctx);
    coap_cleanup();

    return EXIT_SUCCESS;

}

int main(int argc, char *argv[])
{
	
	auto signal_handler(
		[](int signum)
		{
			std::cout << "Received " << strsignal(signum) << std::endl;
		}
	);

	struct sigaction signal_action = {};
	signal_action.sa_handler = signal_handler;
	sigaction(SIGINT, &signal_action, NULL);
	sigaction(SIGTERM, &signal_action, NULL);

	std::vector<std::string> args;
	for (int i = 1; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	auto return_code(main_cpp(args));

	std::cout.flush();
	
	return return_code;

}