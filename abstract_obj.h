#pragma once
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <nx/sdk/helpers/attribute.h>
#include <nx/sdk/ptr.h>

#include "../define_var.h"

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {

                using Attributes = std::vector<nx::sdk::Ptr<nx::sdk::Attribute>>;

                class AbstractObj
                {
                public:
                    AbstractObj(const std::string& typeId, Attributes attributes);
                     ~AbstractObj() = default;

                    Attributes attributes() const;
                    nx::sdk::Uuid id() const;
                    std::string typeId() const;

                protected:
                    std::string m_typeId;
                    Attributes m_attributes;
                    nx::sdk::Uuid m_id;
                };

            } // namespace stub
        } // namespace analytics
    } // namespace vms_server_plugins
} // namespace nx
