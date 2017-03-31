#include "AppInteractionProcessExecutor.hpp"
#include <AppInteraction/Process/AppInteractionProcessModel.hpp>

#include <Device/Protocol/DeviceList.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <Engine/iscore2OSSIA.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <AppInteraction/DocumentPlugin/AppInteractionDocumentPlugin.hpp>

//#include <State/Value.hpp>
#include <State/ValueConversion.hpp>



namespace AppInteraction
{

ProcessExecutor::ProcessExecutor(AppInteraction::ProcessModel& element,
                                 const Device::DeviceList& devices,
                                 const iscore::DocumentContext& context):
    m_devices{devices},
    m_mobileDevice{element.mobileDevice()},
    m_connectionManager{context.plugin<AppInteraction::DocumentPlugin>().connectionManager()},
    m_address{element.address()},
    m_interaction{element.interactionType()},
    m_duration{element.duration().sec()}
{
    qDebug("ProcessExecutor : m_mobileDevice index : %d",m_mobileDevice);
    if(m_mobileDevice != 0)
    {
        std::vector<connectionFaussaire::ConnectionFaussaire*> m_connections = m_connectionManager->getDevices();
        QObject::connect(
                    m_connections[m_mobileDevice-1], &connectionFaussaire::ConnectionFaussaire::interactionValueReturned,
                [=] (const auto& val) { this->interactionValueReceived(val); }); // activated when new ossia value received from app
    }
}


void ProcessExecutor::start()
{
    qDebug("START ...");

    if (m_interaction == 0)
    {
        qDebug("No interaction type chosen");
        return;
    }
    char* interaction;
    sprintf(interaction,"%d:%f:%d", m_interaction-1, m_duration,1);

    if (m_mobileDevice>0)
        m_connectionManager->getDevices()[m_mobileDevice-1]->sendInteraction(interaction);
    else
        qDebug("No Mobile device selected -> no interaction sent.");
}

void ProcessExecutor::stop()
{
}

void ProcessExecutor::pause()
{
}

void ProcessExecutor::resume()
{
}

ossia::state_element ProcessExecutor::offset(
        ossia::time_value off)
{
    return {};
}

ossia::state_element ProcessExecutor::state()
{
    State::Address address{"OSCdevice", {"particle", "density"}};

    State::Value value = State::Value::fromValue(std::abs(qrand()) % 100);

    State::Message m;
    m.address = address;
    m.value = value;

    if(auto res = Engine::iscore_to_ossia::message(m, m_devices))
    {
        if(unmuted())
            return *res;
        return {};
    }
    else
    {
        return {};
    }
    //    return {};
}

ossia::state_element ProcessExecutor::interactionValueReceived(const ossia::value& val){
    qDebug("Ossia value received (from connectionFaussaire)");

    State::Value value = State::Value::fromValue(/*State::fromOSSIAValue(val*/ (std::abs(qrand()) % 100));
    State::Message m;
    m.address = m_address;
    m.value = value;
    //qDebug("Ossia value : %f", val.get<float>());
    if(auto res = Engine::iscore_to_ossia::message(m, m_devices)) //segfault at the second play
    {
        if(unmuted())
        {
            qDebug("msg sent ok");
            return *res;
        }
        return {};
    }
    else
    {
        qDebug("error while sending msg");
        return {};
    }
}

ProcessExecutorComponent::ProcessExecutorComponent(
        Engine::Execution::ConstraintComponent& parentConstraint,
        AppInteraction::ProcessModel& element,
        const Engine::Execution::Context& ctx,
        const Id<iscore::Component>& id,
        QObject* parent):
    ProcessComponent_T{
        parentConstraint, element, ctx, id, "AppInteractionExecutorComponent", parent}
{
    m_ossia_process = std::make_shared<ProcessExecutor>(element, ctx.devices.list(), ctx.doc);
}

}
