#ifndef __LIBED2K_ALERT_TYPES__
#define __LIBED2K_ALERT_TYPES__

// for print_endpoint
#include <libtorrent/socket.hpp>

#include "libed2k/alert.hpp"
#include "libed2k/types.hpp"
#include "libed2k/error_code.hpp"
#include "libed2k/packet_struct.hpp"


namespace libed2k
{
    struct server_name_resolved_alert : alert
    {
        const static int static_category = alert::status_notification;
        server_name_resolved_alert(const std::string& strServer) : m_strServer(strServer){}
        virtual int category() const { return static_category; }

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new server_name_resolved_alert(*this));
        }

        virtual std::string message() const { return std::string("server name was resolved"); }
        virtual char const* what() const { return "server notification"; }

        std::string m_strServer;
    };

    /**
      * emit when server connection was initialized
     */
    struct server_connection_initialized_alert : alert
    {
        const static int static_category = alert::status_notification;

        server_connection_initialized_alert(boost::uint32_t nClientId,
                boost::uint32_t nTCPFlags, boost::uint32_t nAuxPort) :
                    m_nClientId(nClientId),
                    m_nTCPFlags(nTCPFlags),
                    m_nAuxPort(nAuxPort)
        {}

        virtual int category() const { return static_category; }

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new server_connection_initialized_alert(*this));
        }

        virtual std::string message() const { return std::string("server connection was initialized"); }
        virtual char const* what() const { return "server notification"; }

        boost::uint32_t m_nClientId;
        boost::uint32_t m_nTCPFlags;
        boost::uint32_t m_nAuxPort;
    };

    /**
      * emit on OP_SERVERSTATUS
     */
    struct server_status_alert : alert
    {
        const static int static_category = alert::status_notification | alert::server_notification;

        server_status_alert(boost::uint32_t nFilesCount, boost::uint32_t nUsersCount) :
            m_nFilesCount(nFilesCount), m_nUsersCount(nUsersCount)
        {
        }

        virtual int category() const { return static_category; }

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new server_status_alert(*this));
        }

        virtual std::string message() const { return std::string("server status information"); }
        virtual char const* what() const { return "server status information"; }

        boost::uint32_t m_nFilesCount;
        boost::uint32_t m_nUsersCount;
    };

    /**
      * emit on OP_SERVERIDENT
     */

    struct server_identity_alert : alert
    {
        const static int static_category = alert::status_notification | alert::server_notification;

        server_identity_alert(const md4_hash& hServer , net_identifier address, const std::string& strName, const std::string& strDescr) :
            m_hServer(hServer), m_address(address), m_strName(strName), m_strDescr(strDescr)
        {
        }

        virtual int category() const { return static_category; }

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new server_identity_alert(*this));
        }

        virtual std::string message() const { return std::string("server identity information"); }
        virtual char const* what() const { return "server identity information"; }

        md4_hash        m_hServer;
        net_identifier  m_address;
        std::string     m_strName;
        std::string     m_strDescr;
    };

    /**
      * emit for every server message
     */
    struct server_message_alert: alert
    {
        const static int static_category = alert::server_notification;

        server_message_alert(const std::string& strMessage) : m_strMessage(strMessage){}
        virtual int category() const { return static_category; }

        virtual std::string message() const { return m_strMessage; }
        virtual char const* what() const { return "server message incoming"; }

        virtual std::auto_ptr<alert> clone() const
        {
            return (std::auto_ptr<alert>(new server_message_alert(*this)));
        }

        std::string m_strMessage;
    };

    /**
      * emit when server connection closed
     */
    struct server_connection_closed : alert
    {
        const static int static_category = alert::status_notification | alert::server_notification;

        server_connection_closed(const error_code& error) : m_error(error){}
        virtual int category() const { return static_category; }

        virtual std::string message() const { return m_error.message(); }
        virtual char const* what() const { return "server connection closed"; }

        virtual std::auto_ptr<alert> clone() const
        {
            return (std::auto_ptr<alert>(new server_connection_closed(*this)));
        }

        error_code m_error;
    };

    struct mule_listen_failed_alert: alert
    {
        mule_listen_failed_alert(tcp::endpoint const& ep, error_code const& ec):
            endpoint(ep), error(ec)
        {}

        tcp::endpoint endpoint;
        error_code error;

        virtual std::auto_ptr<alert> clone() const
        { return std::auto_ptr<alert>(new mule_listen_failed_alert(*this)); }
        virtual char const* what() const { return "listen failed"; }
        const static int static_category = alert::status_notification | alert::error_notification;
        virtual int category() const { return static_category; }
        virtual std::string message() const
        {
            char ret[200];
            snprintf(ret, sizeof(ret), "mule listening on %s failed: %s"
                , libtorrent::print_endpoint(endpoint).c_str(), error.message().c_str());
            return ret;
        }

    };

    struct peer_alert : alert
    {
        const static int static_category = alert::peer_notification;
        peer_alert(const net_identifier& np, const md4_hash& hash) : m_np(np), m_hash(hash)
        {}

        virtual int category() const { return static_category; }

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new peer_alert(*this));
        }

        virtual std::string message() const { return std::string("peer alert"); }
        virtual char const* what() const { return "peer alert"; }

        net_identifier  m_np;
        md4_hash        m_hash;
    };

    /**
      * this alert throws on server search results and on user shared files
     */
    struct shared_files_alert : peer_alert
    {
        const static int static_category = alert::server_notification | alert::peer_notification;

        shared_files_alert(const net_identifier& np, const md4_hash& hash, const shared_files_list& files, bool more) :
            peer_alert(np, hash),
            m_files(files),
            m_more(more){}
        virtual int category() const { return static_category; }

        virtual std::string message() const { return "search result from string"; }
        virtual char const* what() const { return "search result from server"; }

        virtual std::auto_ptr<alert> clone() const
        {
            return (std::auto_ptr<alert>(new shared_files_alert(*this)));
        }

        shared_files_list       m_files;
        bool                    m_more;
    };

    struct shared_directories_alert : peer_alert
    {
        const static int static_category = alert::peer_notification;

        shared_directories_alert(const net_identifier& np, const md4_hash& hash, const client_shared_directories_answer& dirs) :
            peer_alert(np, hash)
        {
            for (size_t n = 0; n < dirs.m_dirs.m_collection.size(); ++n)
            {
                m_dirs.push_back(dirs.m_dirs.m_collection[n].m_collection);
            }
        }

        virtual int category() const { return static_category; }

        virtual std::string message() const { return "search result from string"; }
        virtual char const* what() const { return "search result from server"; }

        virtual std::auto_ptr<alert> clone() const
        {
            return (std::auto_ptr<alert>(new shared_directories_alert(*this)));
        }

        std::vector<std::string>    m_dirs;
    };

    /**
      * this alert throws on server search results and on user shared files
     */
    struct shared_directory_files_alert : shared_files_alert
    {
        const static int static_category = alert::peer_notification;

        shared_directory_files_alert(const net_identifier& np,
                const md4_hash& hash,
                const std::string& strDirectory,
                const shared_files_list& files) :
            shared_files_alert(np, hash, files, false), m_strDirectory(strDirectory)
        {
        }

        virtual int category() const { return static_category; }

        virtual std::string message() const { return "search result for directory from peer"; }
        virtual char const* what() const { return "search result for directory from peer"; }

        virtual std::auto_ptr<alert> clone() const
        {
            return (std::auto_ptr<alert>(new shared_directory_files_alert(*this)));
        }

        std::string m_strDirectory;
    };

    struct peer_connected_alert : peer_alert
    {

        virtual int category() const { return static_category | alert::status_notification; }
        peer_connected_alert(const net_identifier& np, const md4_hash& hash,
                            bool bActive) : peer_alert(np, hash),
                                            m_active(bActive)
        {}

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new peer_connected_alert(*this));
        }

        virtual std::string message() const { return std::string("peer connected alert"); }
        virtual char const* what() const { return "peer connected alert"; }
        bool m_active;
    };

    struct peer_disconnected_alert : public peer_alert
    {
        virtual int category() const { return static_category | alert::status_notification; }
        peer_disconnected_alert(const net_identifier& np,
                const md4_hash& hash,
                const error_code& ec) : peer_alert(np, hash), m_ec(ec)
        {}

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new peer_disconnected_alert(*this));
        }

        virtual std::string message() const { return std::string("peer disconnected alert"); }
        virtual char const* what() const { return "peer disconnected alert"; }
        error_code m_ec;
    };

    struct peer_message_alert : peer_alert
    {
        peer_message_alert(const net_identifier& np, const md4_hash& hash, const std::string& strMessage) :
            peer_alert(np, hash), m_strMessage(strMessage)
        {}

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new peer_message_alert(*this));
        }

        virtual std::string message() const { return std::string("peer message"); }
        virtual char const* what() const { return "peer notification"; }

       std::string m_strMessage;
    };

    struct peer_captcha_request_alert : peer_alert
    {
        peer_captcha_request_alert(const net_identifier& np, const md4_hash& hash, const std::vector<unsigned char>& captcha) :
            peer_alert(np, hash), m_captcha(captcha)
        {}

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new peer_captcha_request_alert(*this));
        }

        virtual std::string message() const { return std::string("peer captcha request"); }
        virtual char const* what() const { return "peer captcha request"; }

        std::vector<unsigned char>  m_captcha;
    };

    struct peer_captcha_result_alert : peer_alert
    {
        peer_captcha_result_alert(const net_identifier& np, const md4_hash& hash, boost::uint8_t nResult) :
            peer_alert(np, hash), m_nResult(nResult)
        {}

        virtual std::auto_ptr<alert> clone() const
        {
            return std::auto_ptr<alert>(new peer_captcha_result_alert(*this));
        }

        virtual std::string message() const { return std::string("peer captcha result"); }
        virtual char const* what() const { return "peer captcha result"; }

        boost::uint8_t  m_nResult;
    };

    struct shared_files_access_denied : peer_alert
    {
        shared_files_access_denied(const net_identifier& np, const md4_hash& hash) :
            peer_alert(np, hash)
        {}
        virtual int category() const { return static_category; }

        virtual std::string message() const { return "shared files access denied"; }
        virtual char const* what() const { return "shared files access denied"; }

        virtual std::auto_ptr<alert> clone() const
        {
            return (std::auto_ptr<alert>(new shared_files_access_denied(*this)));
        }
    };

}


#endif //__LIBED2K_ALERT_TYPES__