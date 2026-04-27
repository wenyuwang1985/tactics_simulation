#include "MessageSender.h"

#include <fastcdr/Cdr.h>
#include <fastcdr/CdrSizeCalculator.hpp>
#include <fastcdr/FastBuffer.h>

#include <fastdds/dds/core/ReturnCode.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TopicDataType.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/topic/qos/TopicQos.hpp>
#include <fastdds/rtps/common/CdrSerialization.hpp>
#include <fastdds/rtps/common/SerializedPayload.hpp>

#include <cstdint>
#include <iostream>
#include <new>
#include <string>

namespace {

struct DdsSmokeMessage
{
    uint32_t seq {0};
    std::string text;

    void serialize(
            eprosima::fastcdr::Cdr& cdr) const
    {
        cdr << seq;
        cdr << text;
    }

    void deserialize(
            eprosima::fastcdr::Cdr& cdr)
    {
        cdr >> seq;
        cdr >> text;
    }
};

class DdsSmokeMessagePubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    DdsSmokeMessagePubSubType()
    {
        set_name("DdsSmokeMessage");
        max_serialized_type_size =
                static_cast<uint32_t>(eprosima::fastdds::rtps::SerializedPayload_t::representation_header_size + 4 + 4 + 256);
        is_compute_key_provided = false;
    }

    bool serialize(
            const void* const data,
            eprosima::fastdds::rtps::SerializedPayload_t& payload,
            eprosima::fastdds::dds::DataRepresentationId_t) override
    {
        const auto* message = static_cast<const DdsSmokeMessage*>(data);

        try {
            eprosima::fastcdr::FastBuffer fast_buffer(
                reinterpret_cast<char*>(payload.data),
                payload.max_size);
            eprosima::fastcdr::Cdr serializer(
                fast_buffer,
                eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                eprosima::fastdds::rtps::DEFAULT_XCDR_VERSION);

            payload.encapsulation =
                    serializer.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS
                    ? CDR_BE
                    : CDR_LE;

            serializer.serialize_encapsulation();
            message->serialize(serializer);
            payload.length = static_cast<uint32_t>(serializer.get_serialized_data_length());
            return true;
        } catch (const eprosima::fastcdr::exception::Exception& exception) {
            std::cout << "[dds-test] serialize failed: " << exception.what() << std::endl;
            return false;
        }
    }

    bool deserialize(
            eprosima::fastdds::rtps::SerializedPayload_t& payload,
            void* data) override
    {
        auto* message = static_cast<DdsSmokeMessage*>(data);

        try {
            eprosima::fastcdr::FastBuffer fast_buffer(
                reinterpret_cast<char*>(payload.data),
                payload.length);
            eprosima::fastcdr::Cdr deserializer(
                fast_buffer,
                eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                eprosima::fastdds::rtps::DEFAULT_XCDR_VERSION);

            deserializer.read_encapsulation();
            message->deserialize(deserializer);
            return true;
        } catch (const eprosima::fastcdr::exception::Exception& exception) {
            std::cout << "[dds-test] deserialize failed: " << exception.what() << std::endl;
            return false;
        }
    }

    uint32_t calculate_serialized_size(
            const void* const data,
            eprosima::fastdds::dds::DataRepresentationId_t) override
    {
        const auto* message = static_cast<const DdsSmokeMessage*>(data);
        eprosima::fastcdr::CdrSizeCalculator calculator(eprosima::fastdds::rtps::DEFAULT_XCDR_VERSION);
        size_t current_alignment = eprosima::fastdds::rtps::SerializedPayload_t::representation_header_size;
        size_t total_size = current_alignment;

        total_size += calculator.calculate_serialized_size(message->seq, current_alignment);
        total_size += calculator.calculate_serialized_size(message->text, current_alignment);
        return static_cast<uint32_t>(total_size);
    }

    void* create_data() override
    {
        return new DdsSmokeMessage();
    }

    void delete_data(
            void* data) override
    {
        delete static_cast<DdsSmokeMessage*>(data);
    }

    bool compute_key(
            eprosima::fastdds::rtps::SerializedPayload_t&,
            eprosima::fastdds::rtps::InstanceHandle_t&,
            bool) override
    {
        return false;
    }

    bool compute_key(
            const void* const,
            eprosima::fastdds::rtps::InstanceHandle_t&,
            bool) override
    {
        return false;
    }

    bool construct_sample(
            void* memory) const override
    {
        new (memory) DdsSmokeMessage();
        return true;
    }
};

const char* return_code_to_string(
        eprosima::fastdds::dds::ReturnCode_t code)
{
    using namespace eprosima::fastdds::dds;

    switch (code) {
        case RETCODE_OK:
            return "RETCODE_OK";
        case RETCODE_ERROR:
            return "RETCODE_ERROR";
        case RETCODE_UNSUPPORTED:
            return "RETCODE_UNSUPPORTED";
        case RETCODE_BAD_PARAMETER:
            return "RETCODE_BAD_PARAMETER";
        case RETCODE_PRECONDITION_NOT_MET:
            return "RETCODE_PRECONDITION_NOT_MET";
        case RETCODE_OUT_OF_RESOURCES:
            return "RETCODE_OUT_OF_RESOURCES";
        case RETCODE_NOT_ENABLED:
            return "RETCODE_NOT_ENABLED";
        case RETCODE_IMMUTABLE_POLICY:
            return "RETCODE_IMMUTABLE_POLICY";
        case RETCODE_INCONSISTENT_POLICY:
            return "RETCODE_INCONSISTENT_POLICY";
        case RETCODE_ALREADY_DELETED:
            return "RETCODE_ALREADY_DELETED";
        case RETCODE_TIMEOUT:
            return "RETCODE_TIMEOUT";
        case RETCODE_NO_DATA:
            return "RETCODE_NO_DATA";
        case RETCODE_ILLEGAL_OPERATION:
            return "RETCODE_ILLEGAL_OPERATION";
        default:
            return "RETCODE_UNKNOWN";
    }
}

void print_step_result(
        const char* step,
        eprosima::fastdds::dds::ReturnCode_t code)
{
    std::cout << "[dds-test] " << step << ": "
              << return_code_to_string(code)
              << " (" << code << ')' << std::endl;
}

} // namespace

void test_dds()
{
    using namespace eprosima::fastdds::dds;

    std::cout << "[dds-test] start Fast DDS smoke test" << std::endl;

    DomainParticipantFactory* factory = DomainParticipantFactory::get_instance();
    if (factory == nullptr) {
        std::cout << "[dds-test] failed: DomainParticipantFactory is nullptr" << std::endl;
        return;
    }

    DomainParticipant* participant = factory->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    if (participant == nullptr) {
        std::cout << "[dds-test] create_participant failed" << std::endl;
        return;
    }
    std::cout << "[dds-test] participant created" << std::endl;

    TypeSupport type(new DdsSmokeMessagePubSubType());
    print_step_result("register_type", type.register_type(participant));

    Topic* topic = participant->create_topic(
        "dds_smoke_topic",
        type.get_type_name(),
        TOPIC_QOS_DEFAULT);
    if (topic == nullptr) {
        std::cout << "[dds-test] create_topic failed" << std::endl;
        print_step_result("delete_participant", factory->delete_participant(participant));
        return;
    }
    std::cout << "[dds-test] topic created" << std::endl;

    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);
    if (publisher == nullptr) {
        std::cout << "[dds-test] create_publisher failed" << std::endl;
        print_step_result("delete_topic", participant->delete_topic(topic));
        print_step_result("delete_participant", factory->delete_participant(participant));
        return;
    }
    std::cout << "[dds-test] publisher created" << std::endl;

    DataWriter* writer = publisher->create_datawriter(topic, DATAWRITER_QOS_DEFAULT);
    if (writer == nullptr) {
        std::cout << "[dds-test] create_datawriter failed" << std::endl;
        print_step_result("delete_publisher", participant->delete_publisher(publisher));
        print_step_result("delete_topic", participant->delete_topic(topic));
        print_step_result("delete_participant", factory->delete_participant(participant));
        return;
    }
    std::cout << "[dds-test] datawriter created" << std::endl;

    DdsSmokeMessage message;
    message.seq = 1;
    message.text = "hello fast dds";
    print_step_result("write", writer->write(&message));

    print_step_result("delete_datawriter", publisher->delete_datawriter(writer));
    print_step_result("delete_publisher", participant->delete_publisher(publisher));
    print_step_result("delete_topic", participant->delete_topic(topic));
    print_step_result("delete_participant", factory->delete_participant(participant));

    std::cout << "[dds-test] Fast DDS smoke test finished" << std::endl;
}
