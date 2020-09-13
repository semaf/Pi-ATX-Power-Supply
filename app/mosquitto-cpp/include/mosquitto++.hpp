/*
 * mosquitto++.hpp
 *
 */

#ifndef MOSQUITTO_HPP_
#define MOSQUITTO_HPP_

#include <mosquitto.h>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include "IMosquittoCpp.hpp"

namespace MosquittoCpp {

class MosquittoCpp : public IMosquittoCpp {
 private:
  static std::once_flag init_once_;
  struct mosquitto *m_mosq;

  static void onConnectWrapper(struct mosquitto *mosq, void *userdata, int rc) {
    class MosquittoCpp *m = (class MosquittoCpp *)userdata;
    m->onConnect(rc);
  }

  static void onDisconnectWrapper(struct mosquitto *mosq, void *userdata, int rc) {
    class MosquittoCpp *m = (class MosquittoCpp *)userdata;
    m->onDisconnect(rc);
  }

  static void onPublishWrapper(struct mosquitto *mosq, void *userdata, int mid) {
    class MosquittoCpp *m = (class MosquittoCpp *)userdata;
    m->onPublish(mid);
  }

  static void onMessageWrapper(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    class MosquittoCpp *m = (class MosquittoCpp *)userdata;
    m->onMessage(message);
  }

  static void onSubscribeWrapper(struct mosquitto *mosq, void *userdata, int mid, int qos_count,
                                   const int *granted_qos) {
    class MosquittoCpp *m = (class MosquittoCpp *)userdata;
    m->onSubscribe(mid, qos_count, granted_qos);
  }

  static void onUnsubscribeWrapper(struct mosquitto *mosq, void *userdata, int mid) {
    class MosquittoCpp *m = (class MosquittoCpp *)userdata;
    m->onUnsubscribe(mid);
  }

  static void onLogWrapper(struct mosquitto *mosq, void *userdata, int level, const char *str) {
    class MosquittoCpp *m = (class MosquittoCpp *)userdata;
    m->onLog(level, str);
  }

 public:
  // Constructor
  MosquittoCpp(const char *id = nullptr, bool clean_session = true) : m_mosq(nullptr) {
    std::call_once(init_once_, mosquitto_lib_init);
    m_mosq = mosquitto_new(id, clean_session, this);

    max_inflight_messages_set(5); // maximum 5 messages 

    mosquitto_connect_callback_set(m_mosq, onConnectWrapper);
    mosquitto_disconnect_callback_set(m_mosq, onDisconnectWrapper);
    mosquitto_publish_callback_set(m_mosq, onPublishWrapper);
    mosquitto_message_callback_set(m_mosq, onMessageWrapper);
    mosquitto_subscribe_callback_set(m_mosq, onSubscribeWrapper);
    mosquitto_unsubscribe_callback_set(m_mosq, onUnsubscribeWrapper);
    mosquitto_log_callback_set(m_mosq, onLogWrapper);
  }
  virtual ~MosquittoCpp() { mosquitto_destroy(m_mosq); }

  virtual void reinitialise(const char *id, bool clean_session) {
    mosquitto_reinitialise(m_mosq, id, clean_session, this);

    mosquitto_connect_callback_set(m_mosq, onConnectWrapper);
    mosquitto_disconnect_callback_set(m_mosq, onDisconnectWrapper);
    mosquitto_publish_callback_set(m_mosq, onPublishWrapper);
    mosquitto_message_callback_set(m_mosq, onMessageWrapper);
    mosquitto_subscribe_callback_set(m_mosq, onSubscribeWrapper);
    mosquitto_unsubscribe_callback_set(m_mosq, onUnsubscribeWrapper);
    mosquitto_log_callback_set(m_mosq, onLogWrapper);
  }

  virtual void threaded_set(bool threaded = true) { mosquitto_threaded_set(m_mosq, threaded); }

  virtual int socket() { return mosquitto_socket(m_mosq); }

  virtual void will_set(const char *topic, int payloadlen = 0, const void *payload = nullptr, int qos = 0,
                        bool retain = false) {
    mosquitto_will_set(m_mosq, topic, payloadlen, payload, qos, retain);
  }

  virtual void will_clear() { mosquitto_will_clear(m_mosq); }

  virtual void connect(const char *host, int port, int keepalive = 60) {
    mosquitto_connect(m_mosq, host, port, keepalive);
  }

  virtual void connect(const char *host, int port, int keepalive, const char *bind_address) {
    mosquitto_connect_bind(m_mosq, host, port, keepalive, bind_address);
  }

  virtual void connect_async(const char *host, int port = 1883, int keepalive = 60) {
    mosquitto_connect_async(m_mosq, host, port, keepalive);
  }

  virtual void connect_async(const char *host, int port, int keepalive, const char *bind_address) {
    mosquitto_connect_bind_async(m_mosq, host, port, keepalive, bind_address);
  }

  virtual void reconnect() { mosquitto_reconnect(m_mosq); }
  virtual void reconnect_async() { mosquitto_reconnect_async(m_mosq); }

  virtual void disconnect() { mosquitto_disconnect(m_mosq); }

  virtual void username_pw_set(const char *username, const char *password = nullptr) {
    mosquitto_username_pw_set(m_mosq, username, password);
  }

  virtual void publish(int *mid, const char *topic, int payloadlen = 0, const void *payload = nullptr, int qos = 0,
                       bool retain = false) {
    mosquitto_publish(m_mosq, mid, topic, payloadlen, payload, qos, retain);
  }

  virtual void subscribe(int *mid, const char *sub, int qos = 0) {
    mosquitto_subscribe(m_mosq, mid, sub, qos);
  }

  virtual void unsubscribe(int *mid, const char *sub) { mosquitto_unsubscribe(m_mosq, mid, sub); }

  virtual void reconnect_delay_set(unsigned int reconnect_delay, unsigned int reconnect_delay_max,
                                   bool reconnect_exponential_backoff) {
    mosquitto_reconnect_delay_set(m_mosq, reconnect_delay, reconnect_delay_max, reconnect_exponential_backoff);
  }

  virtual void max_inflight_messages_set(unsigned int max_inflight_messages) {
    mosquitto_max_inflight_messages_set(m_mosq, max_inflight_messages);
  }

  virtual void message_retry_set(unsigned int message_retry) { mosquitto_message_retry_set(m_mosq, message_retry); }

  virtual void user_data_set(void *userdata) { mosquitto_user_data_set(m_mosq, userdata); }

  virtual void tls_set(const char *cafile, const char *capath = nullptr, const char *certfile = nullptr,
                       const char *keyfile = nullptr,
                       int (*pw_callback)(char *buf, int size, int rwflag, void *userdata) = nullptr) {
    mosquitto_tls_set(m_mosq, cafile, capath, certfile, keyfile, pw_callback);
  }

  virtual void tls_opts_set(int cert_reqs, const char *tls_version = nullptr, const char *ciphers = nullptr) {
    mosquitto_tls_opts_set(m_mosq, cert_reqs, tls_version, ciphers);
  }

  virtual void tls_insecure_set(bool value) { mosquitto_tls_insecure_set(m_mosq, value); }

  virtual void tls_psk_set(const char *psk, const char *identity, const char *ciphers = nullptr) {
    mosquitto_tls_psk_set(m_mosq, psk, identity, ciphers);
  }

  virtual void opts_set(enum mosq_opt_t option, int value) {
    mosquitto_opts_set(m_mosq, option, static_cast<void *>(&value));
  }

  virtual bool want_write() { return mosquitto_want_write(m_mosq); }

  virtual void loop(int timeout = -1, int max_packets = 1) {
    mosquitto_loop(m_mosq, timeout, max_packets);
  }

  virtual void loop_forever(int timeout = -1, int max_packets = 1) {
    mosquitto_loop_forever(m_mosq, timeout, max_packets);
  }

  virtual void loop_start() { mosquitto_loop_start(m_mosq); }

  virtual void loop_stop(bool force = false) { mosquitto_loop_stop(m_mosq, force); }

  virtual void socks5_set(const char *host, int port = 1080, const char *username = nullptr,
                          const char *password = nullptr) {
    mosquitto_socks5_set(m_mosq, host, port, username, password);
  }

  static bool topic_match(const std::string &sub, const std::string &topic) {
    bool result;
    mosquitto_topic_matches_sub(sub.c_str(), topic.c_str(), &result);
    return result;
  }

  virtual void onConnect(int /*rc*/) { return; }
  virtual void onDisconnect(int /*rc*/) { return; }
  virtual void onPublish(int /*mid*/) { return; }
  virtual void onMessage(const struct mosquitto_message * /*message*/) { return; }
  virtual void onSubscribe(int /*mid*/, int /*qos_count*/, const int * /*granted_qos*/) { return; }
  virtual void onUnsubscribe(int /*mid*/) { return; }
  virtual void onLog(int /*level*/, const char * /*str*/) { return; }
  virtual void onError() { return; }
};

}  // namespace MosquittoCpp

#endif  // MOSQUITTO_HPP
	
