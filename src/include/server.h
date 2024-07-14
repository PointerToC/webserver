#include "event_loop_thread_pool.h"
#include "channel.h"
#include "event_loop.h"

class Server {
  public:
    Server(int port, int thread_num, EventLoop *base_loop);
    ~Server();
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    void Init();
    void Start();

  private:
    int port_;
    int thread_num_;
    int listen_fd_;
    bool started_{false};
    EventLoop *base_loop_{nullptr};
    std::shared_ptr<Channel> listen_channel_{nullptr};
    std::unique_ptr<EventLoopThreadPool> thread_pool_{nullptr};

    void HandleNewConnect();
    void HandleThisConnect();
};
