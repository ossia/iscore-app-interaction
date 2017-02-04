#pragma once
#include <AppInteraction/Process/AppInteractionProcessModel.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegateFactory.hpp>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
#include "../State/Widgets/InteractionTypeWidget.hpp"

#include <State/Unit.hpp>

namespace State
{
class UnitWidget;
}
namespace AppInteraction
{
class ProcessModel;
class PolymorphicEntity;

//! This is the widget that will be shown when clicking on a constraint
//! that has a AppInteraction::ProcessModel process.
class InspectorWidget final :
        public Process::InspectorWidgetDelegate_T<AppInteraction::ProcessModel>,
        public Nano::Observer // For automatic signal - slot destruction
{
    public:
        explicit InspectorWidget(
                const AppInteraction::ProcessModel& object,
                const iscore::DocumentContext& context,
                QWidget* parent);

    private:
//        void on_entityAdded(const PolymorphicEntity&);
//        void on_entityRemoved(const PolymorphicEntity&);
    State::InteractionTypeWidget* m_uw{};
    CommandDispatcher<> m_dispatcher;
};


class InspectorFactory final :
        public Process::InspectorWidgetDelegateFactory_T<ProcessModel, InspectorWidget>
{
        ISCORE_CONCRETE("41b2ea17-9bec-4688-b5e0-d7fbabfd1038")
};
}
