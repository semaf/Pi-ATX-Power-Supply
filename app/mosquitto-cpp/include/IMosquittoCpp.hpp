/*
 * IMosquittoCpp.hpp
 *
 *      API-Docu: https://mosquitto.org/api/files/mosquitto-h.html
 */

#ifndef IMOSQUITTOCPP_HPP_
#define IMOSQUITTOCPP_HPP_

#include <mosquitto.h>
#include <string>

namespace MosquittoCpp {

	class IMosquittoCpp {
		public:
			virtual ~IMosquittoCpp() {}

			virtual void reinitialise(const char *id, bool clean_session) = 0;
			virtual void threaded_set(bool threaded = true) = 0;
			virtual int socket() = 0;
			virtual void will_set(const char *topic, int payloadlen = 0, const void *payload = nullptr, int qos = 0,
					bool retain = false) = 0;
			virtual void will_clear() = 0;
			virtual void connect(const char *host, int port, int keepalive = 60) = 0;
			virtual void connect(const char *host, int port, int keepalive, const char *bind_address) = 0;
			virtual void connect_async(const char *host, int port = 1883, int keepalive = 60) = 0;
			virtual void connect_async(const char *host, int port, int keepalive, const char *bind_address) = 0;
			virtual void reconnect() = 0;
			virtual void reconnect_async() = 0;
			virtual void disconnect() = 0;
			virtual void username_pw_set(const char *username, const char *password = nullptr) = 0;
			virtual void publish(int *mid, const char *topic, int payloadlen = 0, const void *payload = nullptr, int qos = 0,
					bool retain = false) = 0;
			virtual void subscribe(int *mid, const char *sub, int qos = 0) = 0;
			virtual void unsubscribe(int *mid, const char *sub) = 0;
			virtual void reconnect_delay_set(unsigned int reconnect_delay, unsigned int reconnect_delay_max,
					bool reconnect_exponential_backoff) = 0;
			virtual void max_inflight_messages_set(unsigned int max_inflight_messages) = 0;
			virtual void message_retry_set(unsigned int message_retry) = 0;
			virtual void user_data_set(void *userdata) = 0;
			virtual void tls_set(const char *cafile, const char *capath = nullptr, const char *certfile = nullptr,
					const char *keyfile = nullptr,
					int (*pw_callback)(char *buf, int size, int rwflag, void *userdata) = nullptr) = 0;
			virtual void tls_opts_set(int cert_reqs, const char *tls_version = nullptr, const char *ciphers = nullptr) = 0;
			virtual void tls_insecure_set(bool value) = 0;
			virtual void tls_psk_set(const char *psk, const char *identity, const char *ciphers = nullptr) = 0;
			virtual void opts_set(enum mosq_opt_t option, int value) = 0;
			virtual bool want_write() = 0;
			virtual void loop(int timeout = -1, int max_packets = 1) = 0;
			virtual void loop_forever(int timeout = -1, int max_packets = 1) = 0;
			virtual void loop_start() = 0;
			virtual void loop_stop(bool force = false) = 0;
			virtual void socks5_set(const char *host, int port = 1080, const char *username = nullptr,
					const char *password = nullptr) = 0;
			static bool topic_match(const std::string &sub, const std::string &topic);

			virtual void onConnect(int rc) = 0;
			virtual void onDisconnect(int rc) = 0;
			virtual void onPublish(int mid) = 0;
			virtual void onMessage(const struct mosquitto_message *message) = 0;
			virtual void onSubscribe(int mid, int qos_count, const int *granted_qos) = 0;
			virtual void onUnsubscribe(int mid) = 0;
			virtual void onLog(int level, const char *str) = 0;
			virtual void onError() = 0;
	};
}
#endif // IMOSQUITTOCPP_HPP_
