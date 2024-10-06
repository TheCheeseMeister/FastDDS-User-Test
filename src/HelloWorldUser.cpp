#include "HelloWorldPublisher.hpp"
#include "HelloWorldSubscriber.hpp"

void publish(HelloWorldPublisher* pub)
{
    pub->run(10);
}

void receive(HelloWorldSubscriber* sub)
{
    sub->run(10);
}

int main(int argc, char** argv) {
    std::cout << "Initializing publisher..." << std::endl;
    uint32_t samples = 10;
    std::string username = "";
    std::string other_user = "";

    std::cout << "What's your name? ";
    std::getline(std::cin, username, '\n');

    std::cout << "Who do you want to talk to? ";
    std::getline(std::cin, other_user, '\n');

    HelloWorldPublisher* mypub = new HelloWorldPublisher(username);
    if (mypub->init())
    {
        std::cout << "Initializing subscriber..." << std::endl;
        //mypub->run(samples);
    }

    HelloWorldSubscriber* mysub = new HelloWorldSubscriber(other_user);
    if (mysub->init())
    {
        std::cout << "Starting threads..." << std::endl;
        //mysub->run(samples);
    }

    std::thread publishThread(publish, std::ref(mypub));
    std::thread receiveThread(receive, std::ref(mysub));

    publishThread.join();
    receiveThread.join();

    delete mypub;

    std::cout << "Thanks for chatting." << std::endl;
    return 0;
}