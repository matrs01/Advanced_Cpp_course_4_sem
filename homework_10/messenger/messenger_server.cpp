#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <future>
#include <atomic>
#include <utility>


#include <boost/asio.hpp>


std::string read_data_until(boost::asio::ip::tcp::socket & socket)
{
    boost::asio::streambuf buffer;

    boost::asio::read_until(socket, buffer, char(4));

    std::string message;

    // Because buffer 'buf' may contain some other data
    // after '\n' symbol, we have to parse the buffer and
    // extract only symbols before the delimiter.
    std::istream input_stream(&buffer);
    std::getline(input_stream, message, char(4));

    return message;
}

void write_data(boost::asio::ip::tcp::socket & socket, const std::string &data)
{
    boost::asio::write(socket, boost::asio::buffer(data));
}


class User{
public:
    explicit User(boost::asio::io_service &io_service, size_t id) : socket_(io_service),
                                                                    id_(id){};

    boost::asio::ip::tcp::socket& socket(){
        return socket_;
    }

    void set_id(size_t id){ id_ = id; }
    void set_name(const std::string &name){ name_ = name; }

    auto get_id() const{ return id_; }
    auto get_name() const{ return name_; }

private:
    size_t id_;
    std::string name_ = "NONAME";
    boost::asio::ip::tcp::socket socket_;
};


class Session{
public:

    explicit Session(boost::asio::ip::tcp::endpoint endpoint) : endpoint_(std::move(endpoint)){
        acceptor_.bind(endpoint_);
    };

    ~Session(){
        stop();
    }

    void start(){
        listener_future = std::async(std::launch::async, &Session::Listener, this);
        writer_future = std::async(std::launch::async, &Session::Writer, this);

        while(!is_session_ended){
            acceptor_.listen(max_listen_connections_);
            User u{io_service_, unique_ids++};
            acceptor_.accept(u.socket());
            auto name = read_data_until(u.socket());
            u.set_name(name);

            std::scoped_lock lock(users_m);
            users_.push_back(std::move(u));
        }
    }

    void Listener(){
        while(!is_session_ended){
            std::this_thread::yield();
            std::scoped_lock users_lock(users_m);
            for (auto &user : users_){
                if (user.socket().available()){
                    auto msg = read_data_until(user.socket());
                    if (msg == "/shutdown"){ // TODO: treat msg "/exit"
                        stop();
                    }
                    std::scoped_lock message_queue_lock(message_queue_m);
                    message_queue.push(std::make_pair(std::make_pair(
                            user.get_id(),
                            user.get_name()),
                                                      msg));
                    writer_condition.notify_one();
                }
            }
        }
    }

    void Writer(){
        while(!is_session_ended){
            std::unique_lock lock(message_queue_m);
            writer_condition.wait(lock, [&](){ return !message_queue.empty(); });

            auto [id_name, msg] = message_queue.back();
            auto [id, name] = id_name;
            message_queue.pop();

            std::scoped_lock users_lock(users_m);
            std::cout << id << ", " << name << ", " << msg << '\n';
            for (auto &user : users_){
                if (user.get_id() != id){
                    write_data(user.socket(), name + ": " + msg + char(4));
                }
            }
        }

    }

    void stop(){
        is_session_ended = true;
        listener_future.get();
        writer_future.get();
        users_.erase(users_.begin(), users_.end());
    }

private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::acceptor acceptor_{io_service_, endpoint_.protocol()};
    int max_listen_connections_ = 10;
    std::vector<User> users_;
    std::mutex users_m;
    std::queue<std::pair<std::pair<size_t, std::string>, std::string>> message_queue;
    std::mutex message_queue_m;
    std::condition_variable writer_condition;
    std::atomic<bool> is_session_ended = false;
    size_t unique_ids = 0;
    std::future<void> listener_future;
    std::future<void> writer_future;
};



int main(int argc, char ** argv)
{
    system("chcp 1251");

    auto port = 3333;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

    Session session(endpoint);
    session.start();


    system("pause");

    return EXIT_SUCCESS;
}