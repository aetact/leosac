#pragma once

#include <zmqpp/zmqpp.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace Leosac
{

    class Kernel;

    /**
    * This class handle the remote control of leosac.
    *
    * It's responsibility is to expose a secure socket on the network and filter
    * request from the world, passing legit message to the rest of the application.
    *
    * Configuration option:
    *    + port
    *    + z85 encoded private key
    *
    * Commands:
    * MODULE_LIST
    * response: module_name*
    * module_name: a-Z+
    *
    * MODULE_CONFIG module_name
    * response: boost serialization
    *
    * SYNC_FROM ENDPOINT
    * endpoint: tcp://IP:PORT
    *
    * Attempt to Fetch the config from an other Leosac unit and apply it to itself.
    */
    class RemoteControl
    {
    public:
        RemoteControl(zmqpp::context &ctx, Kernel &kernel, const boost::property_tree::ptree &cfg);

    private:

        void module_list(zmqpp::message *message_out);

        void module_config(const std::string &module, zmqpp::message *message_out);

        void sync_from(const std::string &endpoint, zmqpp::message *message_out);

        /**
        * Register by core and called when message arrives.
        */
        void handle_msg();

        void process_config(const boost::property_tree::ptree &cfg);


        /**
        * Build the list of modules (their name) running on the remote host.
        * We issue the MODULE_LIST command in order to do that.
        *
        * @return false if we can't get this list.
        */
        bool gather_remote_module_list(zmqpp::socket &s, std::list<std::string> &remote_modules);

        /**
        * After sending a bunch of MODULE_CONFIG command, wait for reply.
        *
        * Update the configuration manager object with the newly received configuration.
        * Also update additional configuration if needed.
        *
        * If receiving command timeout (>3s) return false;
        */
        bool receive_remote_config(zmqpp::socket &s, std::map<std::string, bool> &cfg);

        /**
        * Will retrieve the config for a remote leosac unit.
        *
        * The socket shall be already connected to the remote endpoint.
        *
        * It will populate the stop_list and start_list with a list of to respectively stop and start.
        */
        bool gather_remote_config(zmqpp::socket &s, std::list<std::string> &start_list, std::list<std::string> &stop_list);

        Kernel &kernel_;

        /**
        * Public ROUTER
        */
        zmqpp::socket socket_;

        /**
        * z85 encoded public curve key
        */
        std::string public_key_;

        /**
        * z85 encoded private curve key
        */
        std::string secret_key_;

        zmqpp::auth auth_;
        std::vector<std::string> test_;

        zmqpp::context &context_;

        // Allow kernel full access to this class.
        friend class Kernel;
    };
}
