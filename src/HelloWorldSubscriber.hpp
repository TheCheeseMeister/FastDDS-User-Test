/**
 * @file HelloWorldSubscriber.hpp
 */

#include "HelloWorldPubSubTypes.hpp";
#include <chrono>
#include <thread>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

using namespace eprosima::fastdds::dds;

class HelloWorldSubscriber
{
    private:
        DomainParticipant* participant_;
        Subscriber* subscriber_;
        DataReader* reader_;
        Topic* topic_;
        TypeSupport type_;

        std::string topic_name;

        class SubListener : public DataReaderListener
        {
            public:
                SubListener() : samples_(0)
                {
                }

                ~SubListener() override
                {
                }

                void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override
                {
                    if (info.current_count_change == 1) {
                        std::cout << "Subscriber matched." << std::endl;
                    }
                    else if (info.current_count_change == -1) {
                        std::cout << "Subscriber unmatched." << std::endl;
                    }
                    else {
                        std::cout << info.current_count_change << " is not a valid value for SubscriptionMatchedStatus current count change." << std::endl;
                    }
                }

                void on_data_available(DataReader* reader) override
                {
                    SampleInfo info;
                    if (reader->take_next_sample(&hello_, &info) == eprosima::fastdds::dds::RETCODE_OK) {
                        if (info.valid_data)
                        {
                            samples_++;
                            //std::cout << "Message: " << hello_.message() << " with index: " << hello_.index() << " RECEIVED." << std::endl;

                            if (hello_.username() != "" && hello_.message() != "") {
                                std::cout << hello_.username() << ": " << hello_.message() << std::endl;
                            }
                        }
                    }
                }

                HelloWorld hello_;

                std::atomic_int samples_;
        }
        listener_;
    
    public:
        HelloWorldSubscriber(std::string topic_name) : participant_(nullptr), subscriber_(nullptr), topic_(nullptr), reader_(nullptr), type_(new HelloWorldPubSubType())
        {
            this->topic_name = topic_name;
        }

        virtual ~HelloWorldSubscriber()
        {
            if (reader_ != nullptr)
            {
                subscriber_->delete_datareader(reader_);
            }
            if (topic_ != nullptr)
            {
                participant_->delete_topic(topic_);
            }
            if (subscriber_ != nullptr)
            {
                participant_->delete_subscriber(subscriber_);
            }
            DomainParticipantFactory::get_instance()->delete_participant(participant_);
        }

        bool init()
        {
            DomainParticipantQos participantQos;
            participantQos.name("Participant_subscriber");
            participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

            if (participant_ == nullptr) {
                return false;
            }

            type_.register_type(participant_);

            //topic_ = participant_->create_topic("HelloWorldTopic", "HelloWorld", TOPIC_QOS_DEFAULT);
            topic_ = participant_->create_topic(topic_name, "HelloWorld", TOPIC_QOS_DEFAULT);

            if (topic_ == nullptr) {
                return false;
            }

            subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

            if (subscriber_ == nullptr)
            {
                return false;
            }

            reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

            if (reader_ == nullptr)
            {
                return false;
            }

            return true;
        }

        void run(uint32_t samples) {
            while (listener_.samples_ < samples) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
};

/*int main(int argc, char** argv)
{
    std::cout << "Starting subscriber." << std::endl;
    uint32_t samples = 10;

    HelloWorldSubscriber* mysub = new HelloWorldSubscriber();
    if (mysub->init())
    {
        mysub->run(samples);
    }

    delete mysub;
    return 0;
}*/