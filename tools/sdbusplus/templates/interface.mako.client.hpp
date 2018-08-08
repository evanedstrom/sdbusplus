#pragma once
#include <sdbusplus/bus.hpp>
<%
    namespaces = interface.name.split('.')
    classname = namespaces.pop()

    def setOfPropertyTypes():
        return set(p.cppTypeParam(interface.name) for p in
                interface.properties);
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
         * @param[in] service - Service name of dbus server service.
         * @param[in] path - Dbus path location of server service.
         */
        ${classname}(const char* service, const char* path);

    % for m in interface.methods:
${ m.cpp_prototype(loader, interface=interface, ptype='client-header') }
    % endfor

    // TODO: Signals

    // TODO: Properties

    private:
        std::shared_ptr<bus::bus> _bus;
        static constexpr auto _interface = "${interface.name}";
        const char* _service;
        const char* _path;
};

} // namespace client
    % for s in reversed(namespaces):
} // namespace ${s}
    % endfor
} // namespace sdbusplus
