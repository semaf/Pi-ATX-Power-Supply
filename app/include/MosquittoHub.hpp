#include <string>
#include <mosquitto++.hpp>

class MosquittoHub : public MosquittoCpp::MosquittoCpp {
	public:
		MosquittoHub(const char *id);
		virtual ~MosquittoHub() {}
		virtual void connect(std::string serverAddress, int port);
		virtual void publish(const std::string& topic, const std::string& payload);
		virtual void onConnect(int rc) override;
		virtual void subscribeTopic(const std::string& topic);
	private:
		std::string server_address_;
};
