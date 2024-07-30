#include "abstract_obj.h"

#include <nx/sdk/helpers/uuid_helper.h>

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {

                AbstractObj::AbstractObj(const std::string& typeId, Attributes attributes) :
                    m_typeId(typeId),
                    m_attributes(std::move(attributes)),
                    m_id(nx::sdk::UuidHelper::randomUuid())
                {

                }

                Attributes AbstractObj::attributes() const
                {
                    return m_attributes;
                }

                nx::sdk::Uuid AbstractObj::id() const
                {
                    return m_id;
                }

                std::string AbstractObj::typeId() const
                {
                    return m_typeId;
                }

            } // namespace stub
        } // namespace analytics
    } // namespace vms_server_plugins
} // namespace nx