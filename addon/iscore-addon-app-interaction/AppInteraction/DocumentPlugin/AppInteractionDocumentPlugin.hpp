#pragma once
#include <iscore/plugins/documentdelegate/plugin/DocumentPlugin.hpp>
#include <AppInteraction/Connection/ConnectionManagerFaussaire.hpp>

namespace AppInteraction
{
class DocumentPlugin;
}

UUID_METADATA(,
    iscore::DocumentPluginFactory,
    AppInteraction::DocumentPlugin,
    "a6f13d05-c54b-4d77-ae23-7dc89ac9f2c9")


namespace AppInteraction
{
/**
 * @brief The DocumentPlugin class
 *
 * The DocumentPlugin is a class that will be instantiated once per document.
 * It is accessible through a \ref iscore::DocumentContext.
 *
 * For instance:
 *
 * \code
 * iscore::DocumentContext& ctx = ...;
 * auto& my_plugin = ctx.plugin<AppInteraction::DocumentPlugin>();
 * \endcode
 *
 * If (and only if) there is no easy access to a \ref iscore::DocumentContext instance,
 * one can get it from an object that is part of the i-score object tree :
 *
 * \code
 * iscore::DocumentContext& ctx = iscore::IDocument::documentContext(some_object);
 * \endcode
 *
 * But this should seldom be used, and especially not in resource-intensive parts of
 * the application. If it is going to be reused, the context should be cached somewhere.
 *
 */
class DocumentPlugin final :
        public iscore::SerializableDocumentPlugin
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS

        SERIALIZABLE_MODEL_METADATA_IMPL(DocumentPlugin)

        private:
            connectionFaussaire::ConnectionManagerFaussaire m_connectionManager = connectionFaussaire::ConnectionManagerFaussaire();
        public:
            connectionFaussaire::ConnectionManagerFaussaire& connectionManager();

            explicit DocumentPlugin(
                    const iscore::DocumentContext& ctx,
                    Id<iscore::DocumentPlugin> id,
                    QObject* parent);

        template<typename Impl>
        DocumentPlugin(
                const iscore::DocumentContext& ctx,
                Impl& vis,
                QObject* parent):
            iscore::SerializableDocumentPlugin{ctx, vis, parent}
        {
            vis.writeTo(*this);
        }


};

using DocumentPluginFactory = iscore::DocumentPluginFactory_T<DocumentPlugin>;
}
