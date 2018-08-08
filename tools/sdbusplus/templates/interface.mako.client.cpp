#include <${"/".join(interface.name.split('.') + [ 'client.hpp' ])}>
<%
    namespaces = interface.name.split('.')
    classname = namespaces.pop()

    def interface_instance():
        return "_".join(interface.name.split('.') + ['interface'])
%>
namespace sdbusplus
{
    % for s in namespaces:
namespace ${s}
{
    % endfor
namespace client
{

${classname}::${classname}(const char* service, const char* path)
        : _bus(std::make_shared<bus::bus>(sdbusplus::bus::new_default())),
          _service(service),
          _path(path)
{
}

    % for m in interface.methods:
${ m.cpp_prototype(loader, interface=interface, ptype='client-cpp') }
    % endfor

    // TODO: Signals

    // TODO: Properties

} // namespace client
    % for s in reversed(namespaces):
} // namespace ${s}
    % endfor
} // namespace sdbusplus
