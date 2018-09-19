#pragma once
#include <sdbusplus/bus.hpp>
<%
    namespaces = interface.name.split('.')
    classname = namespaces.pop()

    def setOfPropertyTypes():
        return set("std::string" if p.is_enum()
                else p.cppTypeParam(interface.name)
                for p in interface.properties);
%>
namespace sdbusplus
{
    % for s in namespaces:
namespace ${s}
{
    % endfor
namespace client
{

class ${classname}
{
    public:
        virtual ~${classname}() = default;

        /** @brief Constructor to connect to default bus.
         *  @param[in] service - Service name of dbus server service.
         *  @param[in] path - Dbus path location of server service.
         */
        ${classname}(const char* service, const char* path);

        /** @brief Constructor to connect to provided bus.
         *  This is helpful for signal monitoring, as access to the bus
         *  object is needed to implement an event loop.
         *
         *  @param[in] bus - Bus to attach to.
         *  @param[in] service - Service name of dbus server service.
         *  @param[in] path - Dbus path location of server service.
         */
        ${classname}(std::shared_ptr<bus::bus> bus, const char* service, const char* path);

###     // TODO: Split into some kind of common class?
        % for e in interface.enums:
        enum class ${e.name}
        {
            % for v in e.values:
            ${v.name},
            % endfor
        };
        % endfor

        % if interface.properties:
        using PropertiesVariant = sdbusplus::message::variant<
                ${",\n                ".join(setOfPropertyTypes())}>;

        /** @brief Constructor to initialize the object from a map of
         *         properties.
         *
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         *  @param[in] vals - Map of property name to value for initialization.
         */
        ${classname}(bus::bus& bus, const char* path,
                     const std::map<std::string, PropertiesVariant>& vals,
                     bool skipSignal = false);
    % endif
###
### Methods
###
    % for m in interface.methods:
        ${ m.cpp_prototype(loader, interface=interface, ptype='client-header') }
    % endfor
###
### Properties
###
    % for p in interface.properties:
        ${ p.cpp_prototype(loader, interface=interface, ptype='client-headers') }
    % endfor
###
### Signals
###
    % for s in interface.signals:
        ${ s.cpp_prototype(loader, interface=interface, ptype='client-headers') }
    % endfor

    private:
        // Members
        std::shared_ptr<bus::bus> _bus;
        static constexpr auto _interface = "${interface.name}";
        const char* _service;
        const char* _path;

        // Enum helpers
    % for e in interface.enums:
        static ${e.name} convert${e.name}FromString(const std::string& s);
    % endfor
};

    % for e in interface.enums:
/* Specialization of sdbusplus::client::bindings::details::convertForMessage
 * for enum-type ${classname}::${e.name}.
 *
 * This converts from the enum to a constant c-string representing the enum.
 *
 * @param[in] e - Enum value to convert.
 * @return C-string representing the name for the enum value.
 */
std::string convertForMessage(${classname}::${e.name} e);
    % endfor


} // namespace client
    % for s in reversed(namespaces):
} // namespace ${s}
    % endfor
} // namespace sdbusplus
